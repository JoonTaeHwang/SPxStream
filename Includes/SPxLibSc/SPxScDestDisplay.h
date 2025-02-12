/*********************************************************************
*
* (c) Copyright 2007 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for the scan conversion display base class.
*
* Revision Control:
*   14/10/19 v1.34   AGC	Suppress cppcheck warnings.
*
* Previous Changes:
*   28/03/19 1.33   AGC	Rename SetRadarLUT lutId parameter to rid.
*   18/02/19 1.32   AGC	Add ClearWholeBitmap().
*   17/05/18 1.31   AGC	Support scan conversion scaling.
*   11/05/16 1.30   AGC	Add rotation support.
*   07/01/16 1.29   AGC	Add pure virtual Set/GetUseIntermediateBitmap().
*   11/12/14 1.28   AGC	Add ClearBitmap() function.
*   04/10/13 1.27   AGC	Simplify headers.
*   19/04/12 1.26   AGC	Make GetUpdateInterval() const.
*   29/03/12 1.25   AGC	Make SetUpdateInterval() virtual.
*   07/07/11 1.24   AGC	Prevent warning in AV library.
*   28/01/11 1.23   AGC	Add SetFadingEnabled/GetFadingEnabled(),
*			GetRadarColour() and GetSweepLine().
*   13/09/10 1.22   REW	Restore GetFadeInterval() to return int
*			and add GetFadeIntervalDouble().
*   08/09/10 1.21   REW	Fade interval kept as double, not integer.
*   27/08/10 1.20   REW	Make destructor virtual.
*   27/05/10 1.19   SP 	Add SetParameter() & GetParameter().
*   21/08/09 1.18   REW	Add SetSweepLine().
*   18/02/09 1.17   REW	Add m_fadeLUT8 and associated support.
*   13/10/08 1.16   DGJ	Add SetRadarEmphasis() etc.
*   20/08/08 1.15   REW	Add SPX_DISPLAY_WIN_PARAM_MCAST_INFO.
*   11/07/08 1.14   REW	Add GetRadarBright().
*   17/01/08 1.13   REW	Avoid icc compiler warnings.
*   11/10/07 1.12   DGJ	Parameter to set window parameter
*   17/09/07 1.11   DGJ	Move Active/Inactive to SPxObj
*   31/08/07 1.10   DGJ	Support Active/Inactive
*   13/07/07 1.9    DGJ	Introduce minFadeInterval
*   04/07/07 1.8    DGJ	Add GetUpdateInterval()
*   02/07/07 1.7    DGJ	Force recalculation of fade table when
*			update interval changes
*   29/06/07 1.6    DGJ	Add m_radarUpdate.
*   27/06/07 1.5    DGJ	Add m_maxW, m_maxH and GetMaxW/H().
*   21/06/07 1.4    DGJ	Brought common code from subclasses to here.
*   18/06/07 1.3    REW	Change OVERRIDE_REDIRECT to be 1, not 2.
*   15/06/07 1.2    DGJ	Add SPX_DISPLAY_OVERRIDE_REDIRECT.
*   13/06/07 1.1    DGJ	Initial Version.
**********************************************************************/

#ifndef _SPxScDestDisplay_h
#define _SPxScDestDisplay_h

/*
 * Other headers required.
 */
#include "SPxScDest.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

// Debug Flags
#define DBUG(x) ((m_debug & x))
#define D_COMMANDS 0x1
#define D_PATCH_UPDATES 0x2
#define D_FADE 0x4
#define D_SETLUT 0x8

/* Flags to control display processing */
#define SPX_DISPLAY_OVERRIDE_REDIRECT (1L << 0)
   
// Define the default update interval in ms. (100 ms = 10 Hz).
#define DEFAULT_FADING_INTERVAL_MS 100

// Default fade time in seconds.
#define DEFAULT_FADE_TIME_SECS 10
     
/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/
// Window parameters
enum
{
    SPX_DISPLAY_WIN_PARAM_TEST_PATTERN = 10,	// Test pattern
    SPX_DISPLAY_WIN_PARAM_UNDERLAY = 11,	// Set underlay window to arg1
    SPX_DISPLAY_WIN_PARAM_OVERLAY =12,		// Set Overlay window to arg1
    SPX_DISPLAY_WIN_XEVENT_MASK = 13,		// Set X Event mask to arg1
    SPX_DISPLAY_WIN_UPDATE_INTERVAL = 14,	// Set update interval in ms
    SPX_DISPLAY_WIN_PARAM_PRIORITY = 15,	// Set window priority
    SPX_DISPLAY_WIN_PARAM_MCAST_INFO = 16	// Set multicast ID/addr/port
};

typedef struct DirtyBox
{
    UINT16 x;
    UINT16 y;
    UINT16 w;
    UINT16 h;
    UINT32 weight;
    UINT16 density;
} DirtyBox;

/* Base class for all scan conversion display destinations (windows/x11). */
class SPxScDestDisplay :public SPxScDest
{
protected:
    static FILE *m_logFile;		// Log file.
    static int m_debug;			// Flags for debugging.
    UINT32 m_colourLUT[256];		// The principal colour LUT
    int m_fadeType;			// Type of fade.
    int m_fadeRate;			// Fade rate (10's of ms)
    int m_updateIntervalMS;		// Interval between updates (eg 100 ms
					// for 10 Hz fading).
    UINT16 m_fadeLUTar[256*256];	// The fade LUT for alpha-red
    UINT16 m_fadeLUTgb[256*256];	// The fade LUT for green-blue		
    UINT8 m_fadeLUT8[256];		// The fade LUT for 8-bit values
    unsigned char m_brightness;
    unsigned char m_radarColourRed,m_radarColourGreen, m_radarColourBlue;
					// Radar Colour.
    int m_winX, m_winY;			// Screen location of top-left of window
    int m_winW, m_winH;			// Window size.
    int m_maxW, m_maxH;			// Maximum screen dimensions of window.
    int m_scaleFactor;			// Scale factor.
    int m_radarUpdate;
    int m_minFadeInterval;		// Minimum fade interval in millisecs.
    double m_fadeInterval;		// The fade interval in milliseconds.
    int	m_useFading;			// TRUE to enable fading.
    double m_radarEmphasis;		// Controls alpha calculation

    double m_rotationDegs;		// Display rotation in degrees.
    SPxScMpRefMode_t m_mpRefMode;	// Moving platform reference mode.

    /* Sweep line. */
    SPxScSweepLineType_t m_sweepLineType;
    SPxScSweepLineColMode_t m_sweepLineColMode;
    UINT32 m_sweepLineRGB;

protected:
    void SetFadeInterval(double fadeInterval) {m_fadeInterval = fadeInterval;}
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName,
                     char *valueBuf, int bufLen);

public:
    static void SetDebugFlags(UINT32 debug) {m_debug = debug;}
    static void SetDebug(FILE *log) {m_logFile = log;}
    SPxScDestDisplay(void);		
    virtual ~SPxScDestDisplay();
    virtual void ClearBitmap(UINT32 /*val*/ =0) {}
    virtual void ClearWholeBitmap(UINT32 val=0) { ClearBitmap(val); }
    void SetFadingEnabled(int enable) { m_useFading = enable; }
    int GetFadingEnabled(void) { return m_useFading; }
    void SetMinFadeInterval(int minFadeIntervalMS);
			    /* If > 0 this is a value in ms. 
			     * If == 0 it means same as update interval.
			     * If < 0 it means a multiple of update interval.
			     */
    int GetMinFadeInterval(void);
    int GetMinFadeIntervalValue(void) {return m_minFadeInterval;}
    int GetFadeInterval(void) { return((int)m_fadeInterval);}
    double GetFadeIntervalDouble(void) {return m_fadeInterval;}
    virtual void SetUpdateInterval(int updateIntervalMS);
    int GetUpdateInterval(void) const {return m_updateIntervalMS;}
    double GetRadarEmphasis (void) {return m_radarEmphasis;}
    void SetRadarEmphasis(double emphasis);
    UINT16* GetFadeLUTar(void) {return m_fadeLUTar;}
    UINT16* GetFadeLUTgb(void) {return m_fadeLUTgb;}
    UINT8* GetFadeLUT8(void) {return m_fadeLUT8;}
    int GetFadeType(void) {return m_fadeType;}
    int GetFadeRate(void) {return m_fadeRate;}
    int GetW(void) {return m_winW;}
    int GetH(void) {return m_winH;}
    int GetMaxW(void) {return m_maxW;}
    int GetMaxH(void) {return m_maxH;}
    virtual void SetUseIntermediateBitmap(int enable)=0;
    virtual int GetUseIntermediateBitmap(void)=0;

    int GetDisplayW(void);
    int GetDisplayH(void);
    virtual void GetDisplayMatrix(float (&matrix)[9]);

    /* Functions from base class. */
    int SetRadarLUT(UCHAR rid, UCHAR *lutData);
    // cppcheck-suppress virtualCallInConstructor
    int SetFade(UCHAR fadeType, UINT16 fadeRate);
    int SetRadarBright(UCHAR rid, UCHAR bright);
    UCHAR GetRadarBright(UCHAR /*rid*/)	{ return(m_brightness); }
    // cppcheck-suppress virtualCallInConstructor
    int SetRadarColour(UCHAR rid, UCHAR arg1, UCHAR arg2, UCHAR arg3);
    void GetRadarColour(UCHAR rid, UCHAR *arg1, UCHAR *arg2, UCHAR *arg3);
    int SetRotationDegs(double rotationDegs);
    double GetRotationDegs(void);
    SPxErrorCode SetScaleFactor(int scaleFactor);
    int GetScaleFactor(void) const { return(m_scaleFactor); }
    int SetMovingPlatformRefMode(SPxScMpRefMode_t mode) { m_mpRefMode = mode; return SPX_NO_ERROR; }
    SPxScMpRefMode_t GetMovingPlatformRefMode(void) const { return m_mpRefMode; }
    int SetSweepLine(SPxScSweepLineType_t type,
				SPxScSweepLineColMode_t colourMode,
				UINT32 rgb,
				UINT32 reserved1, UINT32 reserved2);
    void GetSweepLine(SPxScSweepLineType_t *type,
		      SPxScSweepLineColMode_t *colourMode,
		      UINT32 *rgb,
		      UINT32 * /*reserved1*/, UINT32 * /*reserved2*/)
    {
	if(type) *type = m_sweepLineType;
	if(colourMode) *colourMode = m_sweepLineColMode;
	if(rgb) *rgb = m_sweepLineRGB;
    }
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPxScDestDisplay_h */
/*********************************************************************
*
* End of file
*
**********************************************************************/
