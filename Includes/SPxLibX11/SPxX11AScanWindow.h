/*********************************************************************
*
* (c) Copyright 2008 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Ascan windows in X11.
*
* Revision Control:
*   05/09/22 v1.23   AGC	Suppress spurious cppcheck ODR warning.
*
* Previous Changes:
*   24/06/22 1.22   BTB	Derive from new Ascan interface.
*   09/03/18 1.21   AGC	Improve X-axis tick positions.
*   12/04/17 1.20   AGC	Improve mutex protection.
*   13/10/16 1.19   SP 	Support log X axis.
*                       Add SetXAxisWorld().
*   15/10/15 1.18   AJH Add SetXAxis() and related variables.
*   31/07/14 1.17   SP  Fix N/C #307 - A-Scan trace misaligned 
*                       wrt range scale when start range is non-zero.
*   17/02/14 1.16   AGC	Use const where appropriate.
*   04/10/13 1.15   AGC	Simplify headers.
*   21/05/13 1.14   SP 	Support Y-axis cursor (disabled by default).
*   11/01/13 1.13   SP 	Buffer is resized automatically
*                       therefore SetBufferLen() is no longer
*                       needed and will cause a crash if set >=4096. 
*                       Left as a stub for compatibility. Bug #226.
*   23/11/12 1.12   SP 	Support multiple auxiliary traces.
*                       Support configurable Y axis.
*                       Add range combination control.
*   14/11/12 1.11   SP 	Copy aux trace functions from Windows.
*                       Add ClearReturn().
*   17/11/11 1.10   AGC	Prevent shadowing in SPxIntegerPoint.
*   24/05/11 1.9    AGC	Fix mouse interaction #139 with ReportXEvent().
*   24/03/11 1.8    AGC	Add SetBackgroundColour().
*   13/01/11 1.7    AGC	Support configurable X screen.
*   03/12/10 1.6    AGC	Do not draw highlight edges.
*   10/11/10 1.5    SP 	Add GetViewAsWorld().
*   08/11/10 1.4    AGC	Add mouse interaction support.
*			Support resizing of window.
*		      	Support SetTitle().
*   15/09/10 1.3    REW	Make destructor virtual.
*   19/02/09 1.2    DGJ	Support SetRangeUnits()
*   25/09/08 1.1    DGJ	Initial Version
**********************************************************************/

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xrender.h>

#include "SPxLibData/SPxAscanIface.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"

#define SPX_ASCAN_SHOW_AZIMUTH 0
#define SPX_ASCAN_SHOW_SCAN    1
#define SPX_ASCAN_SHOW_RANGE 2

#define SPX_ASCAN_OVERWRITE 0
#define SPX_ASCAN_FADE 1

/* Margins used in the display of the A-scan in the window */
#define ASCOPE_DEFAULT_MARGIN 30

/* The maximum length of the (optional) title string */
#define SPX_ASCAN_MAX_LENGTH_TITLE 64

#define SPX_ASCAN_XEVENT_MASK (ButtonPressMask | ButtonReleaseMask | PointerMotionMask | LeaveWindowMask)

#define SPX_ASCAN_ABSELECT_AUTO 0
#define SPX_ASCAN_ABSELECT_MANUAL 1

/* Azimuth combination. */
#define SPX_ASCAN_AZI_COMBINE_ALL       0
#define SPX_ASCAN_AZI_COMBINE_HIGHEST   1
#define SPX_ASCAN_AZI_COMBINE_AVERAGE   2
#define SPX_ASCAN_AZI_COMBINE_LAST_ONLY 3

/* Range combination. */
#define SPX_ASCAN_RAN_COMBINE_LOWEST    0
#define SPX_ASCAN_RAN_COMBINE_HIGHEST   1
#define SPX_ASCAN_RAN_COMBINE_AVERAGE   2
#define SPX_ASCAN_RAN_COMBINE_SUBSAMPLE 3

/* For backward compatibility. */
#define SPX_ASCAN_COMBINE_ALL       (SPX_ASCAN_AZI_COMBINE_ALL)
#define SPX_ASCAN_COMBINE_HIGHEST   (SPX_ASCAN_AZI_COMBINE_HIGHEST) 
#define SPX_ASCAN_COMBINE_AVERAGE   (SPX_ASCAN_AZI_COMBINE_AVERAGE)
#define SPX_ASCAN_COMBINE_LAST_ONLY (SPX_ASCAN_AZI_COMBINE_LAST_ONLY)

#define DEFAULT_MAX_BUFFER_LEN 4096

#define SPX_ASCAN_VIEWMODE_SAMPLES 0
#define SPX_ASCAN_VIEWMODE_WORLD 1

#define SPX_ASCAN_SHOW_LABEL_X	    (1 << 0)
#define SPX_ASCAN_SHOW_LABEL_Y	    (1 << 1)
#define SPX_ASCAN_SHOW_GRATICULE    (1 << 2)
#define SPX_ASCAN_SHOW_XAXIS	    (1 << 3)
#define SPX_ASCAN_SHOW_YAXIS	    (1 << 4)

#define SPX_ASCAN_AUX_MODE_NONE        0
#define SPX_ASCAN_AUX_MODE_SUBTRACT    1
#define SPX_ASCAN_AUX_MODE_DIFFERENCE  2

/* Number of auxiliary traces supported. */
#define SPX_ASCAN_NUM_AUX_TRACES       2

#define SPX_ASCAN_AXIS_TYPE_LINEAR     0
#define SPX_ASCAN_AXIS_TYPE_LOG        1

struct SPxIntegerPoint
{
    SPxIntegerPoint(){}
    SPxIntegerPoint(int xPos, int yPos)
    { 
	x = xPos; 
	y = yPos; 
    }
    bool operator==(SPxIntegerPoint& other)
    {
	return ((x == other.x) && (y == other.y));
    }
    int x;
    int y;
};

class SPxAScanWindow :public SPxObj, public SPxAscanIface
{
private:

        /* Private types. */

        /* Auxiliary trace info. */
        // Suppression required due to differing definitions on Win/Linux.
        // cppcheck-suppress ctuOneDefinitionRuleViolation
        typedef struct
        {
            int inUse;                  // Using auxillary channel?
            int setFromNextRtn;         // Set next return as auxilary trace
            int mode;                   // How auxilary is combined with live
            UINT32 traceColour;         // Trace colour defined as 0xAARRGGBB
            unsigned char *buffer;	// Buffer sized to m_bufferLen.
        
        } AuxInfo_t;

        /* Private variables. */
	int m_leftMargin, m_rightMargin;		// Left and right margins for window display.
	int m_topMargin, m_bottomMargin;		// Top and bottom margins.
	int m_displayMode;				// Display mode.
	int m_updated;					// Flag to indicate that new data has arrived.
	int m_enableHighlight;				// Is highlight enabled ?
        int m_bufferLen;                                // The requested buffer length
        int m_bufferLenUsed;                                // The actual buffer length used.
	unsigned char *m_returnBuffer;			// The buffer that's used to hold a return.
	char m_title[SPX_ASCAN_MAX_LENGTH_TITLE];	// Title string.

	UINT16 m_AScanUpdateLUT16[256*256];

	SPxIntegerPoint *m_points;
	void DrawBackground(void);
	int m_backgroundNeedsDraw;
	int m_displayAzimuth;				// Specific azimuth t0 display
	int m_displayAzimuthMin, m_displayAzimuthMax;
	int m_labelFlags;				// How to label the contents										

	double 	m_rangeHighlight, m_rangeHighlightWidth;
								// Range highlight position and width in metres.
	int m_fadeMode;						// Current fade mode.
	int m_abSelectMode;					// AUTO or MANUAL AB select mode.
	double m_fadeRate;					// Fade rate.
	int m_shiftHighlight;					// How much to shift the primary trace in highlight area.
	int m_highlightStartPixels, m_highlightEndPixels;	
	int m_showHighlightRegion;
								// Highlight start and end in window coordinates.
	int m_showSweepLine;					// Show rotating PPI sweep ?
	int m_dragHighlight;

	int m_lastAzimuthForCrossing;
	int m_minSample, m_maxSample;
	double m_minRangeWorld, m_maxRangeWorld;
								// Min and max range in world (Metres)
	int m_x, m_y;							// x,y position is destination drawable.
	int m_width, m_height;					// Window width and height.
	double m_setX, m_setY, m_setW, m_setH;
	SPxAScanWindow *m_highlightSlave;		// The slave window, if any.
	int m_cursorInWindow;
	int m_lastReturnLength;
	int m_viewMode;
	SPxIntegerPoint m_Acursor, m_Bcursor, m_movingCursor;
	double m_movingCursorMetres;	/* The range of the moving cursor in metres */
	double m_AcursorMetres, m_BcursorMetres;
	int m_AcursorLocked, m_BcursorLocked;
	int m_lastAzimuthReceived;
	int m_aziCombineMode;       /* Azimuth combination. */
        int m_ranCombineMode;       /* Range combination. */
	int m_returnAverageTimeConstant;
        int m_enableMovingCursorX;
        int m_enableMovingCursorY;
	void UpdateFadeLUT(void);
	SPxCriticalSection m_updateMutex;
	void AddOverlays(void);
	void ClearHistory();
	int IsPointInHighlight(SPxIntegerPoint);
	void DrawReturn(SPxIntegerPoint *points, int numPoints, UINT32 colour);
	void DrawIntoHistory(void);
	void CheckWindowSize(void);
	int m_createdOK;					// Set to 1 after successfull call of Create().
	XImage *m_historyImage;
	Display *m_xdisplay;
	int m_xscreen;
	XShmSegmentInfo m_sharedMemory;
	Pixmap m_backgroundPixmap, m_historyPixmap, m_composingPixmap;
	int m_mouseX, m_mouseY;
	int m_mask;
	SPxIntegerPoint m_lastDragPoint;
	GC m_gc, m_gcDash;
	XFontStruct *m_font1;
	int m_maxWidth;
	int m_maxHeight;
	Picture m_historyPicture, m_composingPicture;
	Drawable m_destinationDrawable;
	SPxUnitsDist_t m_rangeUnits;
	double m_rangeMetresToDisplay;		/* Convert metres to display units */
	char *m_rangeUnitsLabel;		/* Label for range units */
	AuxInfo_t m_aux[SPX_ASCAN_NUM_AUX_TRACES];
	UINT32 m_highlightColour;
	UINT32 m_backgroundColour;
	UINT32 m_backgroundGratPenColour;
	UINT32 m_cursorColourA;
	UINT32 m_cursorColourB;
	UINT32 m_backgroundLabelColour;
	UINT32 m_backgroundAxisColour;
	UINT32 m_primaryTraceColour;

        int m_xAxisType;                    /* Linear or log axis. */
	unsigned int m_xAxisNumGridLines;   /* Num grid lines on X axis. */
        double m_xAxisGridLineMetres;       /* Spacing between grid lines. */
	unsigned int m_xAxisDecimalPlaces;  /* X axis label precision. */
	int m_xAxisRelativeLabelling;	    /* X axis label relative to centre range. */
        int m_xAxisNumPixels;               /* Number of X pixels on axis (between margins). */
        double m_logXAxisMin;               /* Used to calculate scaling of log X axis. */
        double m_logXAxisMax;               /* Used to calculate scaling of log X axis. */
        double m_logXAxisScale;             /* Scaling of log X axis. */

        double m_yAxisMinVal;               /* Start value on Y axis. */
        double m_yAxisMaxVal;               /* End value on Y axis. */
        unsigned int m_yAxisNumGridLines;   /* Num grid lines on Y axis. */
        unsigned int m_yAxisDecimalPlaces;  /* Y axis label precision. */
        char m_yAxisUnits[16];              /* Y axis units string. */

	/* Private functions. */
	void lineHoriz(int x1, int y1, int x2, UINT32 val);
	void lineVert(int x1, int y1, int y2, UINT32 val);
	int isDrawableValid(Drawable xid);
        void checkResizeBuffers(int sizeBytes,
                             unsigned int auxIndex=SPX_ASCAN_NUM_AUX_TRACES);
        unsigned char correlateRangeSamples(unsigned char *buffer,
                                            int leftRange, 
                                            int rightRange,
                                            int *showSample);
        int getXSampleIndex(int pixels);
        int metresToLogXPixels(double metres);
        double logXPixelsToMetres(int pixels);
        void updateRenderVars(void);

	void setHighlightShift(int shift);
	void setHighlight(double range, double rangeWidth);
	void setHighlightAB();
	void moveHighlightAB(double xMetres);
	void newAuxData(unsigned char *data, int numBytes,
			unsigned int index);
	double getRangeMetresToDisplay(void) const;
	const char *getRangeUnitsLabel(void) const;
	
public:
	// Construction
	SPxAScanWindow(void);   		// standard constructor
	virtual ~SPxAScanWindow(void);
	int Create(Display *display, Drawable destinationDrawable, 
		   double x, double y, double w, double h, 
		   int maxWidth=-1, int maxHeight=-1, int screen=0);

        void SetBufferLen(int len) { /* No longer needed. */ }
        int GetBufferLen(int len) {return m_bufferLenUsed;}

        /* Set/Get title */
	void SetTitle(const char *title);
	char *GetTitle(void) {return m_title;}
	
	/* Set colours */
	void SetBackgroundColour(UINT8 red, UINT8 green, UINT8 blue);
        void SetTraceColour(UINT8 red, UINT8 green, UINT8 blue);
        void SetTraceColour(UINT32 rgb);
        UINT32 GetTraceColour(void) { return m_primaryTraceColour; }
	void SetRangeUnits(SPxUnitsDist_t units);
	void SetRangeUnits(double multiplier, const char *label);
	void SetCursorA(int x);
	void SetCursorB(int x);
	void SetCursor(int x);
	void SetCursorWorld(double xMetres);
	void SetCursorAWorld(double xMetres);
	void SetCursorBWorld(double xMetres);
	void SetDestination(Drawable destination);
	void SetLabelFlag(int flag) {m_labelFlags |= flag;}
	void ClearLabelFlag(int flag) {m_labelFlags &= (~flag);}
	void ShowSweepIndicator(int enable) {m_showSweepLine = enable;}
	void SetViewAsSamples(int minSample, int maxSample);
	void SetViewAsWorld(double startRange, double endRange);
        void GetViewAsWorld(double *startRange, double *endRange);
	void SetABselectMode(int mode) {m_abSelectMode = mode;}
	void SetMargins(unsigned l, unsigned r, unsigned t, unsigned b);
        void ShowMovingCursorX(int enable) { m_enableMovingCursorX = enable; }
        void ShowMovingCursorY(int enable) { m_enableMovingCursorY = enable; }

	/* Set/Get state of ShowHighlight */
	void SetShowHighlight(int state) {m_showHighlightRegion = state;}
	int GetShowHighlight(void) {return m_showHighlightRegion;}

	SPxAScanWindow* GetSlave(void) {return m_highlightSlave;}
	void SetHighlightShift(int shift);
	int GetHighlightShift(void) {return (m_shiftHighlight);}
	void SetHighlightSlave(SPxAScanWindow *);
	void ShowCursor(int state) {m_cursorInWindow = state;}
	void SetUpdateInterval(int updateIntervalMS);
	void SetCombineMode(int mode) {SetAziCombineMode(mode);}
        SPxErrorCode SetAziCombineMode(int mode);
        int GetAziCombineMode(void) {return m_aziCombineMode;}
        SPxErrorCode SetRanCombineMode(int mode);
        int GetRanCombineMode(void) {return m_ranCombineMode;}       
        void ConvertWindowToWorld(int xPixels, double *world);
        void ConvertWindowToIntensity(int yPixels, double *intensity);
	void ConvertWorldToWindow(double , int*);
	void NewReturn(SPxReturn *newReturn);
	void Update(void);
	void SetHighlightAB(void);
	void MoveHighlightAB(double xMetres);
	void SetDisplayMode(int mode) {m_displayMode = mode;}
	void SetDisplayRange(int startAzi, int endAzi) {m_displayAzimuthMin = startAzi; m_displayAzimuthMax = endAzi;}
	int GetDisplayMode(void) {return m_displayMode;}
	void SetDisplayAzimuth(int azi) {m_displayAzimuth = azi;}
	int GetDisplayAzimuth(void) {return m_displayAzimuth;}
	void SetHighlight(double range, double rangeWidth);
	void SetFadeMode(int fadeMode);
	void SetFadeRate(double fadeRate);
	void SetAveragerTC(int tc);
        void ClearReturn(void);
        SPxErrorCode SetXAxisType(int type);
	void SetXAxis(unsigned int numGridLines,
		      unsigned int numDecimalPlaces=1,
		      int relativeLabelling=FALSE);
        void SetXAxisWorld(double gridLineMetres,
                           unsigned int numDecimalPlaces=1,
                           int relativeLabelling=FALSE);
        void SetYAxis(double minVal, 
                      double maxVal,
                      unsigned int numGridLines,
                      unsigned int numDecimalPlaces=0,
                      const char *units=NULL,
                      int adjustLeftMargin=TRUE);

        /* Auxilary trace. */
	void NewAux(SPxReturn *newReturn, unsigned int index=0);
        void NewAuxData(unsigned char *data, int numBytes, 
                        unsigned int index=0);
        int IsAuxSet(unsigned int index=0);
        void SetAuxFromNextReturn(unsigned int index=0);
        void ClearAux(unsigned int index=0);
        void SetAuxColour(UINT8 red, UINT8 green, UINT8 blue, 
                          unsigned int index=0);
        void SetAuxColour(UINT32 rgb, unsigned int index=0);
        UINT32 GetAuxColour(unsigned int index=0);
        void SetAuxMode(int mode);

	/* Handle all XEvents - calls On... functions. */
	SPxErrorCode ReportXEvent(XEvent *event);

	SPxErrorCode OnButtonPress(XEvent *event);
	SPxErrorCode OnButtonRelease(XEvent *event);
	SPxErrorCode OnMouseMotion(XEvent *event);
        SPxErrorCode OnMouseLeave(XEvent *event);

}; /* SPxAScanWindow */


/*********************************************************************
*
* End of file
*
**********************************************************************/

