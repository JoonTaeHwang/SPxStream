/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Ascan windows on Windows.
*
* Revision Control:
*   02/09/22 v1.46   AGC	Suppress spurious cppcheck ODR warning.
*
* Previous Changes:
*   24/06/22 1.45   BTB	Derive from new Ascan interface.
*   07/11/18 1.44   AGC	High DPI improvements.
*   29/08/18 1.43   AGC	Improve graticules.
*   09/03/18 1.42   AGC	Improve X-axis tick positions.
*   08/04/16 1.41   SP 	Add GetYAxisMinVal() and GetYAxisMaxVal().
*   11/03/16 1.40   SP 	Use library DPI scaling functions.
*   03/03/16 1.39   AGC	Fix clang warnings.
*   24/02/16 1.38   SP 	Fix some DPI scaling issues.
*   15/10/15 1.37   AJH	Add SetXAxis() and related variables.
*   14/10/15 1.36   AGC	Avoid warnings when included in AV application.
*   22/08/14 1.35   SP 	Add support for slant range correction.
*   31/07/14 1.34   SP 	Remove unused m_firstSampleMetres.
*   23/07/14 1.33   SP 	Support configurable moving cursor size.
*   18/07/14 1.32   SP 	Fix N/C #307 - A-Scan trace misaligned 
*                       wrt range scale when start range is non-zero.
*   01/07/14 1.31   SP 	Add functions to get cursor position.
*                       Rename m_cursorInWindow to m_showCursors.
*                       Add GetIntensityAtRangeCursor().
*   17/02/14 1.30   AGC	Use const where appropriate.
*   21/05/13 1.29   SP 	Support Y-axis cursor (disabled by default).
*   11/01/13 1.28   SP 	Buffer is resized automatically
*                       therefore SetBufferLen() is no longer
*                       needed will cause a crash if set >=4096. 
*                       Left as a stub for compatibility. Bug #226.
*   23/11/12 1.27   SP 	Support multiple auxiliary traces.
*   15/02/12 1.26   REW	Support freeing resources on exit.
*   17/11/11 1.25   SP 	Support AUX return as reference trace.
*			Add controls for custom Y axis labelling.
*   24/03/11 1.24   AGC	Add SetBackgroundColour().
*   04/02/11 1.23   SP 	Add GetCursorAWorld() & GetCursorBWorld().
*   08/11/10 1.22   AGC	Restrict slave view on view change.
*   15/09/10 1.21   REW	Make destructor virtual.
*   29/06/10 1.20   SP 	For clarity make m_historyData char pointer.
*   23/04/10 1.19   SP 	N/C #081 - Fix A-Scan on Windows7 64-bit.
*   14/02/10 1.18   DGJ	Bring winTemplate into class to allow delete on exit
*   27/03/09 1.17   DGJ	Use per-window timer.
*   22/03/09 1.16   DGJ	Fix fading when A-Scan window is not black.
*			Fix highlight changing on drag near edge
*			Fix range when nominalLength != actualLength
*   08/03/09 1.15   DGJ	Limit drag of AB region to visible window
*			Conrol visibility of Highlight area.
*   23/02/09 1.14   SP 	New functions to set cursor position in metres.
*			New function to move highlighted region.
*   19/02/09 1.13   DGJ	Support resize. Fix bug with corrupted
*			background. Allow displayed units to be set.
*			Move critical section from class to instance.
*			Code tidy-up.
*   26/10/08 1.12   DGJ	Add GetWidth(), GetHeight() access functions.
*			Option for title.
*   25/09/08 1.11   DGJ	New construction API, new buffer allocation
*   05/12/07 1.10   DGJ	Add GetSlave()
*   30/09/07 1.9    DGJ	Add aux fields.
*   17/09/07 1.8    DGJ	Remove Set/Get active (now in SPxObj)
*   31/08/07 1.7    DGJ	Fix bug handling azimuth crossings
*   20/08/07 1.6    DGJ	Range now in metres.
*   29/06/07 1.5    DGJ	Derive from SPxObj.  Reformat.
*   27/06/07 1.4    DGJ	Changed labelling slightly.
*   07/06/07 1.3    REW	Headers in subdirs.
*   24/05/07 1.2    REW Added header, changed to SPx etc.
*   28/04/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_WIN_ASCAN_WINDOW_H_
#define	_SPX_WIN_ASCAN_WINDOW_H_

// AscanWindow.h : header file
//
#include "afxmt.h"
#include "SPxLibData/SPxAscanIface.h"
#include "SPxLibData/SPxPim.h"

#define SPX_ASCAN_SHOW_AZIMUTH 0
#define SPX_ASCAN_SHOW_SCAN    1
#define SPX_ASCAN_SHOW_RANGE 2

#define SPX_ASCAN_OVERWRITE 0
#define SPX_ASCAN_FADE 1

/* Margins used in the display of the A-scan in the wqindow */
#define ASCOPE_DEFAULT_MARGIN 20

/* Windows Timer ID */
#define ASCAN_UPDATE_TIMER 5

// The AScan default timer interval in ms.
#define ASCAN_UPDATE_INTERVAL 50

/* The maximum length of the (optional) title string */
#define MAX_LENGTH_TITLE 64

/* The default highlight shift */
#define ASCAN_DEFAULT_HIGHLIGHT_SHIFT 12

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

#define MAX_TEMP_BUF_SZ 4096
#define MAX_NUM_ASCAN_WINDOWS 10
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

/* By default, show the units in km */
#define DEFAULT_METRES_TO_DISPLAY 0.001
#define DEFAULT_UNITS_STRING "km"

/* Number of auxiliary traces supported. */
#define SPX_ASCAN_NUM_AUX_TRACES       2

class SPxAScanWindow : public CDialog, public SPxObj, public SPxAscanIface
{
    /* Private types. */
    using CDialog::Create;
    
    /* Auxiliary trace info. */
    // Suppression required due to differing definitions on Win/Linux.
    // cppcheck-suppress ctuOneDefinitionRuleViolation
    typedef struct
    {
        int inUse;              // Using auxillary channel?
        int setFromNextRtn;     // Set next return as auxilary trace.
        int mode;               // How auxilary is combined with live.
        CPen *pen;              // Pen used to draw trace.
        COLORREF penCr;         // Current pen colour.
        unsigned char *buffer;	// Buffer sized to m_bufferLen.
        
    } AuxInfo_t;

    int m_leftMargin, m_rightMargin;	// Margins for window display.
    int m_topMargin, m_bottomMargin;	// Top and bottom margins.
    int m_displayMode;			// Display mode.
    BOOL m_updated;			// Flag to indicate new data arrived.
    BOOL m_enableHighlight;		// Is highlight enabled ?
    int m_bufferLen;		        // The requested buffer length
    int m_bufferLenUsed;	        // The actual buffer length used.
    unsigned char *m_returnBuffer;	// Buffer that's used to hold a return.
    char m_title[MAX_LENGTH_TITLE];	// Title string.
    CBitmap m_historyBitmap, m_workingBitmap, m_backgroundBitmap;
    unsigned char *m_historyData;
    unsigned char m_AScanUpdateLUTred[256], m_AScanUpdateLUTgreen[256];
    unsigned char m_AScanUpdateLUTblue[256];
    POINT *m_points;
    CDC m_historyDC, m_workingDC, m_backgroundDC;
    COLORREF m_newDataCr;
    CPen *m_newDataPen, *m_axisPen, *m_graticuleDashPen, *m_graticuleSolidPen, *m_rotatingPPPIpen;
    CPen *m_backgroundPen, *m_AcursorPen, *m_BcursorPen;
    CBrush *m_highlightBrush, *m_backgroundBrush, *m_cursorBrush;
    CFont m_labelFont;
    HBITMAP m_hBitmapForHistory;
    HDC m_hdcForHistory;

    DLGTEMPLATE *m_winTemplate;
    int m_displayAzimuth;		// Specific azimuth to display
    int m_displayAzimuthMin, m_displayAzimuthMax; // Ranges of azis to display.
    int m_labelFlags;			// How to label the contents
    double m_rangeHighlight, m_rangeHighlightWidth;	// Range highlight position and width in metres.
    int m_fadeMode;			// Current fade mode.
    int m_abSelectMode;			// AUTO or MANUAL AB select mode.
    double m_fadeRate;			// Fade rate.
    int m_shiftHighlight;		// How much to shift the primary trace in highlight area.
    int m_highlightStartPixels, m_highlightEndPixels;	
					// Highlight start and end in window coordinates.
    BOOL m_showSweepLine;		// Show rotating PPI sweep ?
    BOOL m_dragHighlight;
    CPoint m_lastDragPoint;
    int m_lastAzimuthForCrossing;
    int m_minSample, m_maxSample;
    double m_minRangeWorld, m_maxRangeWorld;
    double m_metresPerSample;
    double m_startRangeMetres;
					// Min and max range in world (Metres)
    int m_x, m_y;			// Window x and y
    int m_width, m_height;		// Window width and height.
    double m_setX, m_setY;		/* Pixels or fraction of width/height for x/y */
    double m_setW, m_setH;		/* Pixels of fraction of parent width/height for w/g */
    SPxAScanWindow *m_highlightSlave;	// The slave window, if any.
    int m_autoResizeWidth, m_autoResizeHeight;  /* Auto resize on parent window change? */
    BOOL m_showCursors;
    int m_lastReturnLength;
    int m_viewMode;
    int m_showHighlightRegion;		/* Shall we show the AB region as a highlight? */
    CWnd *m_parent;			// Parent window 
    CPoint m_Acursor, m_Bcursor, m_movingCursor;
    double m_movingCursorMetres;	/* The range of the moving cursor in metres */
    double m_movingCursorIntensity;     /* The intensity of the moving cursor. */
    double m_intensityAtRangeCursor;    /* Intensity at the range cursor position. */
    double m_AcursorMetres, m_BcursorMetres;
    BOOL m_AcursorLocked, m_BcursorLocked;
    int m_maxWidth, m_maxHeight;	/* The maximum permitted size of the window */
    SPxUnitsDist_t m_rangeUnits;	/* Range units */
    double m_rangeMetresToDisplay;	/* Convert metres to display units */
    char *m_rangeUnitsLabel;		/* Label for range units */
    int m_lastAzimuthReceived;
    int m_aziCombineMode;               /* Azimuth combination. */
    int m_ranCombineMode;               /* Range combination. */
    int m_returnAverageTimeConstant;
    int m_enableMovingCursorX;
    int m_enableMovingCursorY;
    unsigned int m_movingCursorXSizePercent;
    unsigned int m_movingCursorYSizePercent;
    int m_isMouseTracking;
    unsigned int m_xAxisNumGridLines;   /* Num grid lines on X axis. */
    unsigned int m_xAxisDecimalPlaces;  /* X axis label precision. */
    int m_xAxisRelativeLabelling;	/* X axis labelling relative to centre range. */
    double m_yAxisMinVal;               /* Start value on Y axis. */
    double m_yAxisMaxVal;               /* End value on Y axis. */
    unsigned int m_yAxisNumGridLines;   /* Num grid lines on Y axis. */
    unsigned int m_yAxisDecimalPlaces;  /* Y axis label precision. */
    char m_yAxisUnits[16];              /* Y axis units string. */
    double m_radarHeightMetres;         /* Radar height ABL in metres. */
    
    /* Original items to restore on exit. */
    HGDIOBJ m_origWorkingDCBitmap;
    HGDIOBJ m_origBackgroundDCBitmap;
    HGDIOBJ m_origBitmapForHistory;

    /* Auxiliary trace data. */
    AuxInfo_t m_aux[SPX_ASCAN_NUM_AUX_TRACES];

    void UpdateFadeLUT();
    SPxCriticalSection m_updateMutex;
    void AddOverlays(void);
    void ClearHistory();
    BOOL IsPointInHighlight(CPoint);
    void DrawReturn(CDC *dc, POINT *points, int numPoints);
    void DrawIntoHistory(void);
    POINT *m_tempPointBuffer;
    void DrawBackground(void);
    void ConvertWorldToWindow(double worldRangeMetres, int *x);
    void ConvertWindowToWorld(int xPixels, double *world);
    void ConvertWindowToIntensity(int yPixels, double *intensity);
    void CheckResizeBuffers(int sizeBytes,
                            unsigned int auxIndex=SPX_ASCAN_NUM_AUX_TRACES);
    unsigned char correlateRangeSamples(unsigned char *buffer,
                                        int leftRange, 
                                        int rightRange,
                                        int *showSample);
    double slantToGroundMetres(double slantRangeMetres);
    double groundToSlantMetres(double groundRangeMetres);
    int getSampleIndexForWindowX(int xPixels);
    double getRangeMetresToDisplay(void) const;
    const char *getRangeUnitsLabel(void) const;

public:

    void SetBufferLen(int /*len*/) { /* No longer needed. */ }
    int GetBufferLen(int len) {return m_bufferLenUsed;}
    void SetRangeUnits(SPxUnitsDist_t units);
    void SetRangeUnits(double multiplier, const char *label);
    void SetLabelFlag(int flag) {m_labelFlags |= flag;}
    void ClearLabelFlag(int flag) {m_labelFlags &= (~flag);}
    void ShowSweepIndicator(BOOL enable) {m_showSweepLine = enable;}
    void SetViewAsSamples(int minSample, int maxSample);
    void SetViewAsWorld(double startRange, double endRange);
    void GetViewAsWorld(double *startRange, double *endRange);
    void SetABselectMode(int mode) {m_abSelectMode = mode;}
    void SetMargins(unsigned l, unsigned r, unsigned t, unsigned b); 
    void ShowMovingCursorX(int enable) { m_enableMovingCursorX = enable; }
    void ShowMovingCursorY(int enable) { m_enableMovingCursorY = enable; }
    void SetMovingCursorXSize(unsigned int percent);
    void SetMovingCursorYSize(unsigned int percent);
    void OnPaint();
    void OnTimer(UINT_PTR);

    /* Set/Get state of ShowHighlight */
    void SetShowHighlight(int state) {m_showHighlightRegion = state;}
    int GetShowHighlight(void) {return m_showHighlightRegion;}

    /* Construction, Create and Destructor */
    SPxAScanWindow(void);
    virtual int Create(CWnd *parent, double x, double y, double w, double h,
						int maxW = -1, int maxH = -1);
    virtual ~SPxAScanWindow(void);

    /* Set/Get title */
    void SetTitle(const char *title);
    char *GetTitle(void) {return m_title;}
    
    /* Set colours */
    void SetBackgroundColour(UINT8 red, UINT8 green, UINT8 blue);
    void SetTraceColour(UINT8 red, UINT8 green, UINT8 blue);
    void SetTraceColour(UINT32 rgb);
    UINT32 GetTraceColour(void);

    SPxAScanWindow* GetSlave(void) {return m_highlightSlave;}
    void SetHighlightShift(int shift)
    {
	if (shift < 0) m_shiftHighlight =0;
	else m_shiftHighlight = shift;
    }
    int GetHighlightShift(void) {return (m_shiftHighlight);}
    void SetHighlightSlave(SPxAScanWindow *);
    void ShowCursor(BOOL state) {m_showCursors = state;}
    void SetCursor(int x);
    void SetCursorA(int x);
    void SetCursorB(int x);
    void SetCursorWorld(double xMetres);
    void SetCursorAWorld(double xMetres);
    void SetCursorBWorld(double xMetres);
    double GetCursorAWorld(void);
    double GetCursorBWorld(void);
    void SetUpdateInterval(int updateIntervalMS);
    void SetCombineMode(int mode) {SetAziCombineMode(mode);}
    SPxErrorCode SetAziCombineMode(int mode);
    int GetAziCombineMode(void) {return m_aziCombineMode;}
    SPxErrorCode SetRanCombineMode(int mode);
    int GetRanCombineMode(void) {return m_ranCombineMode;}     
    void NewReturn(SPxReturn *newReturn);
    void Update(void);
    int GetWidth(void) {return m_width;}
    int GetHeight(void) {return m_height;}
    void SetHighlightAB(void);
    void MoveHighlightAB(double xMetres);
    void SetDisplayMode(int mode) {m_displayMode = mode;}
    void SetDisplayRange(int startAzi, int endAzi)
    {
	m_displayAzimuthMin = startAzi;
	m_displayAzimuthMax = endAzi;
    }
    void ConvertWindowDeltaToWorld(int windowDistance, double *worldDistance);
    int GetDisplayMode(void) {return m_displayMode;}
    void SetDisplayAzimuth(int azi) {m_displayAzimuth = azi;}
    int GetDisplayAzimuth(void) {return m_displayAzimuth;}
    void SetHighlight(double range, double rangeWidth);
    void SetFadeMode(int fadeMode);
    void SetFadeRate(double fadeRate);
    void SetAveragerTC(int tc);
    void ClearReturn(void);

    /* Auxilary trace. */
    void NewAux(SPxReturn *newReturn, unsigned int index=0);
    void NewAuxData(unsigned char *data, int numBytes, 
                    unsigned int index=0);
    int IsAuxSet(unsigned int index=0);
    void SetAuxFromNextReturn(unsigned int index=0);
    void ClearAux(unsigned int index=0);
    void SetAuxColour(UINT32 rgb, unsigned int index);
    void SetAuxColour(UINT8 red, UINT8 green, UINT8 blue, 
                      unsigned int index=0);
    UINT32 GetAuxColour(unsigned int index=0);
    void SetAuxMode(int mode);

    /* Configure X axis. */
    void SetXAxis(unsigned int numGridLines,
		  unsigned int numDecimalPlaces=1,
		  int relativeLabelling=FALSE);
							  
    /* Configure Y axis. */
    void SetYAxis(double minVal, 
                  double maxVal,
                  unsigned int numGridLines,
                  unsigned int numDecimalPlaces=0,
                  const char *units=NULL, 
                  int adjustLeftMargin=TRUE);
    const char *GetYAxisUnits(void) { return m_yAxisUnits; }
    double GetYAxisMinVal(void) { return m_yAxisMinVal; }
    double GetYAxisMaxVal(void) { return m_yAxisMaxVal; }

    /* Slant range correction. */
    void SetSlantRangeHeightMetres(double heightMetres);
    double GetSlantRangeHeightMetres(void) { return m_radarHeightMetres; }

    /* Provide access to cursor position. */
    int IsCursorInWindow(void) { return ((m_movingCursor.x != 0) || (m_movingCursor.y != 0)); }
    double GetCursorRangeMetres(void) { return m_movingCursorMetres; }
    double GetIntensityAtRangeCursor(void) { return m_intensityAtRangeCursor; }
    double GetCursorIntensity(void) { return m_movingCursorIntensity; }
      
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAscanWindow)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
public:

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(SPxAScanWindow)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    afx_msg void OnDpiChangedAfterParent(void);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif /* _SPX_WIN_ASCAN_WINDOW_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
