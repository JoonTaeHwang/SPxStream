/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for main class that actually does the scan conversion.
*
*
* Revision Control:
*   27/04/22 v1.101  REW	Support AnalysePoint().
*
* Previous Changes:
*   19/02/21 1.100  AGC	Support configurable clear value.
*   20/12/19 1.99   AGC	Improve performance of PIM accesses.
*   28/03/19 1.98   AGC	Rename SetRadarLUT lutId parameter to rid.
*   20/03/19 1.97   AGC	Support single scan converter for live/trails.
*   20/03/19 1.96   AGC	Support sharing destination.
*   22/02/19 1.95   AGC	Support PIM rescaling.
*   21/02/19 1.94   AGC	Use limited updates for pixels outside radar area.
*   19/02/19 1.93   AGC	Do not update tables when fully out of view.
*   28/09/18 1.92   AJH	Add m_useAtan2Lut.
*   29/08/18 1.91   AGC	Switch internal functions to use doubles.
*   29/08/18 1.90   REW	Add public TrueTrailsClearBitmaps().
*   16/08/18 1.89   REW	Clear uncovered edges of true trails bitmaps.
*			Rework residues for true trail scrolling.
*   16/08/18 1.88   AGC	Separate scroll values for trails bitmaps.
*   15/08/18 1.87   REW	First merge for true trail preservation.
*   27/07/18 1.86   REW	Initial work ready to start support for
*			preserving true trails across pan/zoom.
*   21/06/18 1.85   AGC	Auto-update scale factor.
*   17/05/18 1.84   AGC	Support scan conversion scaling.
*   03/04/18 1.83   SP 	Add m_visState.
*   12/01/18 1.82   REW	Support callback for MP trails shift.
*   03/11/17 1.81   REW	Support SSC standby.
*   02/11/17 1.80   REW	Add API for SSC support.
*   01/11/17 1.79   REW	Initial pixel-based SSC support.
*   31/05/17 1.78   REW	Don't use SPxAtomic for m_pimNumAzimuths
*			(restore performance of pan/zoom in last change)
*   16/05/17 1.77   AGC	Support ground/sea stabilisation.
*   28/02/17 1.76   AGC	Support longer sweep fades.
*   22/12/16 1.75   REW	Support SetDeferCalcs().
*   20/12/16 1.74   AGC	Simplify moving platform trails updates.
*   19/12/16 1.73   AGC	Undo last change.
*   16/12/16 1.72   REW	Add mutexAlreadyLocked to NotifyChange()
*			and getPim() to fix build of .cpp file.
*   11/05/16 1.71   AGC	Add moving platform reference mode support.
*   16/03/16 1.70   REW	Add SetViewTrails().
*   15/03/16 1.69   REW	Add SetMovingPlatformTrailsMode() support.
*   26/02/16 1.68   REW	Add scrollBuffer().
*   04/12/15 1.67   AGC	Add Clear() function.
*   03/11/15 1.66   AGC	PatchSet() patch data now const.
*   29/04/15 1.65   AGC	Share atan2 tables across multiple scan converters.
*   19/03/15 1.64   AGC	Fix unused argument warning.
*   18/03/15 1.63   REW	Add GetRadarBright().
*   07/11/14 1.62   AGC	Make GetState() const.
*   24/09/14 1.61   SP 	Add GetViewControl().
*   19/08/14 1.60   REW	Add slant range height support.
*   18/11/13 1.59   AGC	Fix clang warning.
*   10/10/13 1.58   REW	Support high-res azimuth offset.
*   04/10/13 1.57   AGC	Simplify headers.
*   17/09/13 1.56   SP 	Support multiple platforms in SPxViewControl.
*   25/07/13 1.55   AGC	Add option to enable anti-clockwise correction.
*   06/06/13 1.54   AGC	Support upside down radar data.
*   22/11/12 1.53   REW	PanView() is now in same group as base funcs.
*   12/06/12 1.52   SP 	Support SPxViewControl.
*   14/04/12 1.51   REW	Add PanView().
*   04/04/12 1.50   SP 	Add GetFastRedrawsEnabled().
*   08/02/12 1.49   SP 	Add GetRadarColour().
*                       Change m_radarColour from int to UINT32.
*   04/02/11 1.48   AGC	Add Set/Get SweepLine Type/ColMode/Colour functions.
*   28/10/10 1.47   AGC	Add variables for storing radar colour and brightness.
*   12/10/10 1.46   REW	Support SetSendUpdatesAlways().
*   04/10/10 1.45   AGC	Add GetRadarEndRangeInMetres function.
*   15/09/10 1.44   REW	Make destructor virtual.
*   02/09/10 1.43   REW	Add Get/SetReportAziUpdates().
*   27/05/10 1.42   SP 	Add GetScType().
*   09/12/09 1.41   REW	Support GetParameter().
*   04/12/09 1.40   REW	Use platform heading for BScan parallax.
*   27/11/09 1.39   REW	Fix #066, sweep line leaves remnant behind.
*   06/10/09 1.38   REW	Add m_lastSweepLineType.
*   18/09/09 1.37   REW	Support bscan parallax calculations.
*   21/08/09 1.36   REW	Add SetSweepLine().
*   14/08/09 1.35   REW	Start support for sweep lines.
*   24/04/09 1.34   DGJ	Add GetRadarPosition() function.
*   13/10/08 1.33   REW	Add m_outputIntervalMsecs.
*   13/08/08 1.32   REW	Support Bscan windows as well as PPI.
*   29/07/08 1.31   REW	Add GetWinGeom().
*   11/07/08 1.30   REW	Add GetState().
*   08/02/08 1.29   REW	Keep handle of history process, if given.
*   07/02/08 1.28   REW	Keep second PIM for redraws.
*   31/01/08 1.27   REW	Remember activity state.
*   20/11/07 1.26   REW	Allow redraws to be disabled.
*   15/11/07 1.25   REW	Support scanMode settings.
*   25/10/07 1.24   REW	Change SetSource() to use UINT32 args.
*   13/09/07 1.23   REW	Add changeFlags to addZoneToOutputList().
*   10/09/07 1.22   REW	Args to recalcScTopLefts().
*   07/09/07 1.21   REW	Keep info on last calculations.
*   06/09/07 1.20   REW	Add NotifyChanges() and updateInternals() etc.
*   23/08/07 1.19   REW	Reduce per-pixel memory requirements.
*   23/08/07 1.18   REW	Use fixed-point representation of indices.
*   17/08/07 1.17   REW	Add GetView().
*   06/08/07 1.16   DGJ	Add GetPixelsPerSample() access function
*   09/07/07 1.15   REW	Add GetLogFile().
*   03/07/07 1.14   REW	Add m_logFile support.
*   27/06/07 1.13   REW	Atan2Table is per object.  suggestRedraw flag
*			removed from addZone function. Add function
*			GetRadarEndRangeInPixels().
*   22/06/07 1.12   REW	Remove unused algorithms and structure entries.
*			Try optimising atan2() calculations.
*   20/06/07 1.11   REW	Add getValueForPixel(), and m_saved fields.
*   19/06/07 1.10   DGJ	Added access functions for fade and radar XY.
*   18/06/07 1.9    REW	Destination needs to be set at startup.
*   15/06/07 1.8    REW	New algorithms working. Needs tidying etc.
*   14/06/07 1.7    REW	Continue rewrite. Partial checkin for DGJ sync.
*   13/06/07 1.6    REW	Start total rewrite.  Now new API but naive
*			brute force forwards scan conversion.
*   08/06/07 1.5    DGJ	Reworked/renamed SPxSc class hierarchy.
*   07/06/07 1.4    REW	Get it to build on linux (not functional)
*   04/06/07 1.3    DGJ	Tweaked name capitalisation.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*   14/05/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_SC_SOURCE_LOCAL_H
#define _SPX_SC_SOURCE_LOCAL_H

/*
 * Other headers required.
 */

/* For SPxScanMode. */
#include "SPxLibData/SPxRib.h"

/* Our object is derived from the standard source class. */
#include "SPxLibSc/SPxScSource.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Maximum number of true-trails bitmaps. */
#define	SPX_SC_SOURCE_LOCAL_MAX_TT_BITMAPS	4

/* Maximum number of destinations. */
#define SPX_SC_SOURCE_LOCAL_MAX_DESTS		2


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

/* Forward declarations. */
class SPxRunProcess;
struct SPxReturn_tag;
class SPxPacketDecoder;
class SPxViewControl;

/* Define the structure we use internally for maintaining zones. */
typedef struct SPxScLocalZoneInfo_tag {
    /* Top left of patch, in screen pixels. */
    UINT16 x;
    UINT16 y;

    /* Dimensions of patch, in screen pixels. */
    UINT16 w;
    UINT16 h;

    /* Flag to say patch has changed since it was last output. */
    int dirty;
} SPxScLocalZoneInfo_t;

/*
 * Define the structure we use internally for maintaining pixels.
 */
typedef struct SPxScLocalPerPixelInfo_tag {
    /*
     * For "double-LUT azi-to-pixel-list-to-source-box" algorithm.
     */
    /* Exact location of top left corner of pixel.  The "Fp" denotes
     * we are using our fixed-point representation.
     */
    int rangeIdxFp;
    int azimuthIdxFp;

    /* Bounding box of radar samples hitting this pixel. */
    UINT16 minRngIdx;
    UINT16 maxRngIdx;
    UINT16 minAziIdx;
    UINT16 maxAziIdx;
} SPxScLocalPerPixelInfo_t;

/* Define the structure we use internally for lists of pixels. */
typedef struct SPxScLocalPixelCoord_tag {
    UINT16 x;		/* X coord (pixels from left) */
    UINT16 y;		/* Y coord (pixels from top) */
} SPxScLocalPixelCoord_t;

/* Define the structure we use internally for maintaining azis. */
typedef struct SPxScLocalPerAziInfo_tag {
    unsigned int numPixelsHit;		/* Num pixels to update for azi */
    unsigned int numPixelsSoFar;	/* Counter whilst filling table */
    SPxScLocalPixelCoord_t *firstPixel;	/* First pixel in list */
} SPxScLocalPerAziInfo_t;

/* Define structure for per-pixel info in the true trails bitmaps. */
typedef struct SPxScLocalTrueTrailsPerPixelInfo_tag {
    struct SPxScLocalTrueTrailsPerPixelInfo_tag *next;
    INT16 x;		/* Pixel offset from centre */
    INT16 y;		/* Pixel offset from centre */
    REAL32 rngPixels;	/* Range from radar */
    REAL32 aziDegs;	/* Azimuth clockwise from north */
    UINT16 aziPimIdx;	/* Azimuth index in PIM */
} SPxScLocalTrueTrailsPerPixelInfo_t;

/* Define structure for true trails bitmaps. */
typedef struct SPxScLocalTrueTrailsBitmapInfo_tag {
    unsigned char *buf;		/* Bitmap */
    unsigned int bufSizeBytes;	/* Allocated size of buf */
    REAL32 radarXPixels;	/* Radar position (pixels from left) */
    REAL32 radarYPixels;	/* Radar position (pixels from top) */
    REAL32 widthMetres;		/* Width represented by bitmap */
} SPxScLocalTrueTrailsBitmapInfo_t;

/* Moving platform trails modes (needs second scan converter). */
enum SPxScMpTrailsMode_t
{
    /* Normal mode (true from true, relative from relative, none). */
    SPX_SC_MP_TRAILS_NORMAL = 0,

    /* Relative trails in true motion mode. */
    SPX_SC_MP_TRAILS_RELATIVE_FROM_TRUE = 1,

    /* True trails in relative motion mode. */
    SPX_SC_MP_TRAILS_TRUE_FROM_RELATIVE = 2
};

/* Stabilisation modes. */
enum SPxScMpStabiliseMode_t
{
    /* Trails are stabilised with reference to the ground. */
    SPX_SC_MP_STABILISE_MODE_GROUND = 0,

    /* Trails are stabilised with reference to the sea. */
    SPX_SC_MP_STABILISE_MODE_SEA = 1

};

/* Forward declare any classes we need in our own class in case
 * headers are included in the wrong order.
 */
class SPxPIM;
class SPxScDest;

/*
 * Define the class that actually does scan conversion and is therefore
 * a source of scan converted data!
 *
 * This is derived the SPxScSource class which is the base of all sources
 * of scan converted data (including network receipt), which in turn is
 * derived from the base SPxSc class.
 */
class SPxScSourceLocal :public SPxScSource
{
public:
    /*
     * Public variables.
     */
    
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxScSourceLocal(SPxScDest *dest, int scType = SPX_SC_TYPE_PPI,
		     SPxScDest *destTrails = NULL);
    virtual ~SPxScSourceLocal();

    /* Information retrieval. */
    SPxScDest *GetDest(UCHAR rid=0)	{ return(m_dest[rid]); }
    int GetState(UCHAR rid=0) const	{ return(m_scState[rid]); }
    int GetFadeType(UCHAR rid=0)	{ return m_fadeType[rid]; }
    int GetFadeRate(UCHAR rid=0)	{ return m_fadeRate[rid]; }
    int GetRadarScreenX(void)	{ return m_radarPixelsX; }
    int GetRadarScreenY(void)	{ return m_radarPixelsY; }
    int GetScType(void)         { return m_scType; } 
    double GetPixelsPerSample(void) 
    {
	/* This may return zero if current scaling not known/calculated. */
	m_criticalSection.Enter(); 
	double pixelsPerSample = m_pixelsPerSample; 
	m_criticalSection.Leave(); 
	return(pixelsPerSample);
    }
    double GetRadarEndRangeInPixels(void)
    {
	return(m_savedEndRange * m_samplesPerWorld * m_pixelsPerSample);
    }
    double GetRadarEndRangeInMetres(void)
    {
	return(m_savedEndRange);
    }
    int GetView(REAL32 *xPtr, REAL32 *yPtr, REAL32 *wPtr, REAL32 *hPtr)
    {
	if(xPtr) { *xPtr = (REAL32)m_viewCentreX; }
	if(yPtr) { *yPtr = (REAL32)m_viewCentreY; }
	if(wPtr) { *wPtr = (REAL32)m_viewWidth; }
	if(hPtr) { *hPtr = (REAL32)m_viewHeight; }
	return(SPX_NO_ERROR);
    }
    int GetWinGeom(INT16 *screenXPtr, INT16 *screenYPtr,
			UINT16 *screenWPtr, UINT16 *screenHPtr)
    {
	if(screenXPtr)	{ *screenXPtr = (INT16)m_winX; }
	if(screenYPtr)	{ *screenYPtr = (INT16)m_winY; }
	if(screenWPtr)	{ *screenWPtr = (UINT16)m_winWidthPixels; }
	if(screenHPtr)	{ *screenHPtr = (UINT16)m_winHeightPixels; }
	return(SPX_NO_ERROR);
    }

    /* Input of data and information from the source. */
    int NotifyChange(SPxRunProcess *runProcess, UINT32 flags);
    int NewData(SPxRunProcess *runProcess, SPxReturn_tag *firstReturn,
			unsigned int firstAziIdx, unsigned int numAzis);

    /* Overrides for the pure virtual functions from SPxSc base.
     * Most of these need to (also) invoke the same function on our
     * destination object.
     */
    int SetWinPos(INT16 screenX, INT16 screenY);
    int SetWinGeom(INT16 screenX, INT16 screenY,
				UINT16 screenW, UINT16 screenH);
    int SetWinParam(UINT16 param, UINT32 arg1,
				UINT32 arg2=0, UINT32 arg3=0);
    int SetFade(UCHAR fadeType, UINT16 fadeRate) { return SetFade(0, fadeType, fadeRate); }
    int SetFade(UCHAR rid, UCHAR fadeType, UINT16 fadeRate);
    int SetView(REAL32 viewX, REAL32 viewY, REAL32 viewW, REAL32 viewH);
    int SetViewTrails(REAL32 viewX, REAL32 viewY, REAL32 viewW, REAL32 viewH,
				UCHAR clearTrails);
    int PanView(REAL32 viewX, REAL32 viewY, UCHAR clearTrails=0);
    int SetRadarColour(UCHAR rid, UCHAR red, UCHAR green, UCHAR blue);
    int GetRadarColour(UCHAR rid, UCHAR *red, UCHAR *green, UCHAR *blue);
    UINT32 GetRadarColour(UCHAR rid);
    int SetRadarBright(UCHAR rid, UCHAR bright);
    UCHAR GetRadarBright(UCHAR rid);
    int SetRadarLUT(UCHAR rid, UCHAR * lutData);
    int ShowRadar(UCHAR rid, UCHAR state);
    int PatchSet(UCHAR format, UINT16 numPatches,
				const UINT16 *patchData, UINT16 patchLen);
    int SetSource(UINT32 sourceID, UINT32 arg1=0, UINT32 arg2=0);
    int SetRadarPosition(REAL32 radarX, REAL32 radarY, UCHAR clearTrails=0);
    void GetRadarPosition(double *radarX, double *radarY)
    {
	if( radarX ) { *radarX = m_radarWorldX; }
	if( radarY ) { *radarY = m_radarWorldY; }
    }
    int SetRotationDegs(double rotationDegs);
    int SetSweepLine(SPxScSweepLineType_t type,
				SPxScSweepLineColMode_t colourMode,
				UINT32 rgb,
				UINT32 reserved1, UINT32 reserved2);
    int SetSweepLineType(SPxScSweepLineType_t type);
    int SetSweepLineColMode(SPxScSweepLineColMode_t colMode);
    int SetSweepLineColour(UINT32 rgb);
    int GetSweepLineType(SPxScSweepLineType_t *type);
    int GetSweepLineColMode(SPxScSweepLineColMode_t *colMode);
    int GetSweepLineColour(UINT32 *rgb);
    int EnableAntiClockwiseCorrection(int correctAntiClockwise);
    int GetAntiClockwiseCorrectionEnabled(void) { return m_correctAntiClockwise; }

    /* Configuration options. */
    void EnableFastRedraws(int enable) { m_flagRedrawBufferEnabled = enable;}
    int GetFastRedrawsEnabled(void) { return m_flagRedrawBufferEnabled; }
    int SetRedrawHistoryProcess(SPxRunProcess *historyProcess);
    void SetOutputIntervalMsecs(UINT32 ms) { m_outputIntervalMsecs = ms; }
    UINT32 GetOutputIntervalMsecs(void) { return(m_outputIntervalMsecs); }
    void SetReportAziUpdates(int enable) { m_cfgReportAziUpdates = enable; }
    int GetReportAziUpdates(void)	{ return(m_cfgReportAziUpdates); }
    void SetSendUpdatesAlways(int enable) { m_cfgSendUpdatesAlways = enable; }
    int GetSendUpdatesAlways(void)	{ return(m_cfgSendUpdatesAlways); }
    void SetDeferCalcs(int defer)	{ m_cfgDeferCalcs = defer; }
    int GetDeferCalcs(void) const	{ return(m_cfgDeferCalcs); }
    void SetUseAtan2Lut(int use) { m_useAtan2Lut = use; }
    int GetUseAtan2Lut(void) const { return(m_useAtan2Lut); }
    void SetFullPixelUpdates(int full);
    int GetFullPixelUpdates(void) const { return m_fullPixelUpdates; }
    void SetRescalePim(int rescale) { m_rescalePim = rescale; }
    int GetRescalePim(void) const { return(m_rescalePim); }
    void SetClearValue(UINT8 clearVal) { m_clearVal = clearVal; }
    UINT8 GetClearValue(void) const { return(m_clearVal); }

    /* Height for slant range adjustment. */
    int SetSlantRangeHeightMetres(double metres);
    double GetSlantRangeHeightMetres(void) {return(m_slantRangeHeightMetres);}

    /* Moving platform trails mode. */
    int SetMovingPlatformTrailsMode(SPxScMpTrailsMode_t mode);
    SPxScMpTrailsMode_t GetMovingPlatformTrailsMode(void) const
    {
	return(m_mpTrailsMode);
    }

    int SetMovingPlatformRefMode(SPxScMpRefMode_t mode);
    SPxScMpRefMode_t GetMovingPlatformRefMode(void) const
    {
	return(m_mpRefMode);
    }

    int SetMovingPlatformStabiliseMode(SPxScMpStabiliseMode_t mode);
    SPxScMpStabiliseMode_t GetMovingPlatformStabiliseMode(void) const
    {
	return(m_mpStabiliseMode);
    }

    /* Callback for moving platform trails shifts. */
    int SetMpTrailsCallback(void (*fn)(SPxScSourceLocal *, void *, int, int),
				void *userArg);

    /* External view control. */
    SPxErrorCode SetViewControl(SPxViewControl *obj,
				unsigned int radarIndex = 0,
				unsigned int platformIndex = 0);
    SPxViewControl *GetViewControl(void) const;

    /* Scan-to-scan correlation control. */
    SPxErrorCode SetSSCEnabled(int enabled);
    int GetSSCEnabled(void) const
    {
	return(m_sscEnabled);
    }
    SPxErrorCode SetSSCStandbyEnabled(int enabled);
    int GetSSCStandbyEnabled(void) const
    {
	return(m_sscStandbyEnabled);
    }
    SPxErrorCode SetSSCWindowLength(unsigned int numScans);
    unsigned int GetSSCWindowLength(void) const
    {
	return(m_sscWindowLength);
    }
    SPxErrorCode SetSSCWindowThreshold(unsigned int numHits);
    unsigned int GetSSCWindowThreshold(void) const
    {
	return(m_sscWindowThreshold);
    }
    SPxErrorCode SetSSCInstantThreshold(unsigned char sampleLevel);
    unsigned char GetSSCInstantThreshold(void) const
    {
	return(m_sscInstantThreshold);
    }
    SPxErrorCode SetSSCQuantisationGain(double gain);
    double GetSSCQuantisationGain(void) const
    {
	return(m_sscQuantisationGain);
    }
    SPxErrorCode SetSSCCorrelationGain(double gain);
    double GetSSCCorrelationGain(void) const
    {
	return(m_sscCorrelationGain);
    }

    /* True trails support. */
    SPxErrorCode SetTrueTrailsDimension(unsigned int pixels);
    unsigned int GetTrueTrailsDimension(void) const
    {
	return(m_trueTrailsDimension);
    }
    SPxErrorCode SetTrueTrailsRangeList(const char *rangeMetresList);
    const char *GetTrueTrailsRangeList(void) const
    {
	return(m_trueTrailsRangeList);
    }
    SPxErrorCode TrueTrailsClearBitmaps(void);

    /* Scan-conversion scale factor. */
    SPxErrorCode SetScaleFactor(int scaleFactor);
    int GetScaleFactor(void) const
    {
	return(m_scaleFactor);
    }

    /* Clear internal buffer. */
    int Clear(void);

    /* Statistical analysis. */
    SPxErrorCode AnalysePoint(int x, int y,
				unsigned char *valPtr, double *snsPtr=NULL,
				double *meanPtr=NULL, double *sdPtr=NULL);

    /* Overrides for virtual functions in SPxObj base class. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);
    
    /*
     * Static public functions.
     */
    static void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    static FILE *GetLogFile(void)		{ return(m_logFile); }

protected:
    /*
     * Protected functions for testing.
     */
    virtual UINT32 GetMpTrailsUpdateInterval(void);

private:
    /*
     * Private variables.
     */
    /* Saved scan converter state (in addition to GetActive()) */
    SPxAtomic<int> m_scState[2];	/* SPX_SC_STATE_STOP/RUN etc. */

    /* Saved visibility state. */
    SPxAtomic<int> m_visState[2];	/* SPX_SC_STATE_HIDDEN/VISIBLE etc. */

    SPxAtomic<int> m_scStateInUse[2];		/* SPX_SC_STATE_STOP/RUN etc. */

    /* Type of scan conversion to do. */
    int m_scType;			/* SPX_SC_TYPE_PPI/BSCAN etc. */

    /* Destination for our scan converted video. */
    SPxScDest *m_dest[SPX_SC_SOURCE_LOCAL_MAX_DESTS];

    /* Source of the radar video etc.. */
    SPxAtomic<SPxPIM*> m_pim;		   /* PIM for live updates */
    SPxPIM *m_pimRedraw;		   /* PIM for fast redraws */
    SPxRunProcess *m_redrawHistoryProcess; /* Process feeding m_pimRedraw */
    int m_pimNumAzimuths;
    int m_pimNumSamples;
    SPxAtomic<int> m_prevAziIdx;
    int m_rescalePim;			   /* Rescale PIM based on current view size? */

    /* Saved information from the radar return headers etc. */
    double m_savedStartRange;
    double m_savedEndRange;
    unsigned int m_savedNominalLength;

    /* Ship's heading (used for bscan parallax compensation). */
    SPxAtomic<UINT32> m_shipsHeading16Bit;

    /* Modes. */
    SPxScanMode m_scanMode;		/* What scan mode are we in? */
    int m_modeIsRotating;		/* Is the scanMode a rotating one? */
    int m_isAntiClockwise;		/* Is the radar rotating anti-clockwise? */
    int m_correctAntiClockwise;		/* Correct display of anti-clockwise radar? */
    int m_highResAziRemainder;		/* Extra azi offset to apply */

    /* Radar colour. */
    UINT32 m_radarColour[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    unsigned int m_radarBright[SPX_SC_SOURCE_LOCAL_MAX_DESTS];

    /* Fading. */
    unsigned int m_fadeType[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    unsigned int m_fadeRate[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    unsigned int m_fadeAmount[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    double m_fadeRemainder[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    UINT16 m_lastAzimuth;

    /* Sweep line. */
    SPxScSweepLineType_t m_sweepLineType;
    SPxScSweepLineColMode_t m_sweepLineColMode;
    UINT32 m_sweepLineRGB;

    /* Dimensions of window (and position, to pass on only). */
    int m_setWinWidthPixels;
    int m_setWinHeightPixels;
    int m_winWidthPixels;
    int m_winHeightPixels;
    int m_winX;
    int m_winY;

    /* Location of radar, world and relative to window. */
    double m_radarWorldX;
    double m_radarWorldY;
    int m_radarPixelsX;
    int m_radarPixelsY;
    double m_bscanRadarDegsFromNorth;

    /* Location of own ship's reference point, relative to window. */
    int m_osrpPixelsX;
    int m_osrpPixelsY;

    /* View information. */
    double m_viewCentreX;
    double m_viewCentreY;
    double m_setViewWidth;
    double m_setViewHeight;
    double m_viewWidth;
    double m_viewHeight;

    /* Convenient mapping values. */
    double m_samplesPerWorld;
    double m_worldsPerSample;
    double m_pixelsPerSample;
    double m_samplesPerPixel;
    double m_bscanPixelsPerDegree;
    double m_bscanDegreesPerPixel;

    /* Buffer for scan converting into. */
    unsigned char *m_buffer[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    unsigned int m_bufferSizeBytes[SPX_SC_SOURCE_LOCAL_MAX_DESTS];	/* Not all used */

    /* Scaling. */
    int m_scaleFactor;				/* Requested scale factor. */
    int m_usedScaleFactor;			/* In-use scale factor. */

    /* Scan-to-scan correlation support. */
    int m_sscEnabled;				/* Is SSC enabled? */
    int m_sscStandbyEnabled;			/* Is SSC standby enabled? */
    UINT16 *m_sscBuffer[SPX_SC_SOURCE_LOCAL_MAX_DESTS];	/* Buffer for history data */
    unsigned int m_sscBufferSizeBytes[SPX_SC_SOURCE_LOCAL_MAX_DESTS];	/* Size of buffer in bytes */
    unsigned int m_sscAge[SPX_SC_SOURCE_LOCAL_MAX_DESTS];	/* How old is SSC buffer? */
    unsigned int m_sscWindowLength;		/* Number of history scans */
    unsigned int m_sscWindowThreshold;		/* Required num hits */
    unsigned char m_sscInstantThreshold;	/* Or minimum live value */
    unsigned int m_sscPositionIdx;		/* 0-based index in window */
    unsigned int m_sscPositionBitShift;		/* Shift up/down */
    unsigned int m_sscPositionMask;		/* Bitmask (shifted up) */
    unsigned int m_sscResolutionNumBits;	/* Bits per history sample */
    unsigned int m_sscResolutionMask;		/* Mask for one sample */
    double m_sscQuantisationGain;		/* Pre-gain for quantisation */
    double m_sscCorrelationGain;		/* Gain for correlated data */
    unsigned char *m_sscQuantisationLUT;	/* Quantising live data */
    unsigned char *m_sscCorrelationLUT;		/* Correlating history scans */
    unsigned char *m_sscCombinationLUT;		/* Combining with live data */

    /* Table of zones and other information for them. */
    SPxScLocalZoneInfo_t *m_patchTable;
    unsigned int m_patchTableSizeEntries;	/* Not all used */
    unsigned int m_patchTableUsedEntries;
    unsigned int m_numZonesAcross;
    UINT32 m_lastZoneOutputMsecs;		/* Time of last output */
    UINT32 m_outputIntervalMsecs;		/* Output rate */

    /* Buffer of pending zones ready to be output. */
    unsigned char *m_pendingZoneBuffer[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    unsigned int m_pendingZoneBufferSizeBytes[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    unsigned int m_pendingZoneBufferUsedBytes[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    unsigned int m_numPendingZones[SPX_SC_SOURCE_LOCAL_MAX_DESTS];
    int m_aziAtLastZoneOutput[SPX_SC_SOURCE_LOCAL_MAX_DESTS];

    /* Flags controlling recalculations. */
    int m_scTablesValid;
    UINT32 m_scTablesValidAtMsecs;	/* Timestamp for update */
    int m_flagRangeChanged;	/* Range/numSamples of radar source changed */
    int m_flagSizeChanged;	/* Size of window changed */
    int m_flagZoomChanged;	/* Scale in window changed */
    int m_flagViewChanged;	/* View in window changed */
    int m_flagPositionChanged;	/* Radar position changed */
    int m_flagClearBuffer;	/* Clear the contents of the buffer */
    int m_flagRedrawBuffer;	/* Quickly redraw the buffer. */
    int m_flagRedrawBufferEnabled;	/* Are quick redraws enabled? */

    /* Copies of the last values used in the recalculations. */
    double m_lastSamplesPerWorld;
    double m_lastPixelsPerSample;
    int m_lastWinWidthPixels;
    int m_lastWinHeightPixels;
    double m_lastViewCentreX;
    double m_lastViewCentreY;
    double m_lastRadarWorldX;
    double m_lastRadarWorldY;
    int m_lastRadarPixelsX;
    int m_lastRadarPixelsY;
    SPxScanMode m_lastScanMode;
    int m_lastIsAntiClockwise;
    SPxScSweepLineType_t m_lastSweepLineType;
    SPxAtomic<UINT32> m_lastShipsHeading16Bit;
    int m_lastHighResAziRemainder;
    double m_lastSlantRangeHeightMetres;
    int m_lastFullPixelUpdates;
    int m_lastPimNumAzimuths;

    /* Tables for scan conversion mappings. */
    SPxScLocalPerPixelInfo_t *m_pixelMappings;
    unsigned int m_pixelMappingsSizeEntries;	/* Not all used */
    SPxScLocalPerAziInfo_t *m_aziTable;
    unsigned int m_aziTableSizeEntries;		/* Not all used */
    SPxScLocalPixelCoord_t *m_aziMappings;
    unsigned int m_aziMappingsSizeEntries;	/* Not all used */
    unsigned int m_totalPixelsInRangeHit;

    /* Dirty area. */
    unsigned int m_dirtyMinX1;
    unsigned int m_dirtyMinX2;
    unsigned int m_dirtyMinY1;
    unsigned int m_dirtyMinY2;
    unsigned int m_dirtyMaxX1;
    unsigned int m_dirtyMaxX2;
    unsigned int m_dirtyMaxY1;
    unsigned int m_dirtyMaxY2;
    int m_fullPixelUpdates;

    /* Cleared 8-bit value. */
    UINT8 m_clearVal;

    /* Table for atan2() lookups, and flag to say whether we use it. */
    const int *m_atan2Lut;
    int m_useAtan2Lut;

    /* Critical section for protecting ourselves. */
    SPxCriticalSection m_criticalSection;

    /* Other configuration options. */
    int m_cfgReportAziUpdates;		/* Non-zero => report azi to clients */
    int m_cfgSendUpdatesAlways;		/* Non-zero => send updates all 360 */
    int m_cfgDeferCalcs;		/* Non-zero => do calcs in thread */

    /* Slant range height. */
    double m_slantRangeHeightMetres;

    /* Moving platform trails (second scan converter). */
    SPxScMpTrailsMode_t m_mpTrailsMode;	/* Mode */
    SPxScMpRefMode_t m_mpRefMode;	/* Reference mode */
    SPxScMpStabiliseMode_t m_mpStabiliseMode; /* Stabilisation mode. */
    double m_mpTrailsPlatLatDegs;	/* Last known view centre */
    double m_mpTrailsPlatLongDegs;	/* Last known view centre */
    double m_mpTrailsCentreLatDegs;	/* Last known view centre */
    double m_mpTrailsCentreLongDegs;	/* Last known view centre */
    double m_mpTrailsRotationDegs;
    double m_mpTrailsXPixelsResidue;	/* Balancing amount for next update */
    double m_mpTrailsYPixelsResidue;	/* Balancing amount for next update */
    UINT32 m_mpTrailsUpdateTime;	/* Time of last trails update */

    /* Callback for shifts in moving platform trails. */
    void (*m_mpTrailsCallbackFn)(SPxScSourceLocal *, void *, int, int);
    void *m_mpTrailsCallbackArg;	/* User arg for callback function. */

    /* External view control. */
    SPxViewControl *m_viewCtrl;          /* Installed view control object. */
    unsigned int m_viewCtrlRadarIndex;   /* Index of radar in view control. */
    unsigned int m_viewCtrlPlatformIndex;/* Index of platform in view control. */
 
    /* True trails information. */
    unsigned int m_trueTrailsDimension;		/* Pixels (width and height) */
    char *m_trueTrailsRangeList;		/* List of ranges */
    SPxScLocalTrueTrailsPerPixelInfo_t *m_trueTrailsPixelArray;
    SPxScLocalTrueTrailsPerPixelInfo_t **m_trueTrailsAziList;
    SPxScLocalTrueTrailsBitmapInfo_t
		m_trueTrailsBitmapArray[SPX_SC_SOURCE_LOCAL_MAX_TT_BITMAPS];

    /*
     * Private functions.
     */
    /* Private versions of public functions. */
    int setWinPos(INT16 screenX, INT16 screenY);
    int setWinGeom(INT16 screenX, INT16 screenY,
		   UINT16 screenW, UINT16 screenH);
    int panView(double viewX, double viewY, UCHAR clearTrails);
    int setViewTrails(double viewX, double viewY, double viewW, double viewH,
		      UCHAR clearTrails);
    int setRadarPosition(double radarX, double radarY, UCHAR clearTrails);
    SPxErrorCode setScaleFactor(int scaleFactor);

    int updateInternals(void);

    /* Buffer allocation. */
    int allocateBuffers(void);

    /* Actual scan conversion. */
    int scanConvertAzi(int aziIdx, SPxReturn *rtn, SPxPIM *pim,
			int drawSweepLine=FALSE, int clearSweepLine=FALSE);
    unsigned char getValueForPixel(SPxScLocalPerPixelInfo_t *pix,
					int aziIdx,
					SPxPIM *pim,
					int drawSweepLine=FALSE,
					int clearSweepLine=FALSE);
    int updatePixel(int bufferIdx, int x, int y, unsigned char val,
			int drawSweepLine=FALSE, int clearSweepLine=FALSE);

    /* Zone output. */
    int outputDirtyZones(void);
    int addZoneToOutputList(int bufferIdx,
			    UINT16 x, UINT16 y, UINT16 w, UINT16 h,
			    UINT32 changeFlags);
    int flushPendingZones(int bufferIdx);

    /* Calculations etc. */
    void recalcFade(void);
    int recalcScTables(void);
    void recalcScTopLefts(int startX, int startY, int w, int h);
    int recalcScBoxes(void);
    void recalcScAziLists(void);
    int getAziIdxForXY(int x, int y);
    int initAtan2Table(void);
    SPxErrorCode scrollBuffer(int bufferIdx, int xPixels, int yPixels);
    int updateView(UINT32 changeFlags,
		   unsigned int radarIndex,
		   unsigned int platformIndex);
    void updateMpTrails(void);
    int rescalePim(void);
    void clear(void);
    void clearBuffer(int rid);
    void clearSSC(int rid);
    int canControlDest(void);

    /* Scan-to-scan correlation. */
    SPxErrorCode sscUpdatePosition(unsigned int newPosition);
    SPxErrorCode sscRebuildQuantisationLUT(void);
    SPxErrorCode sscRebuildCorrelationLUT(void);
    SPxErrorCode sscRebuildCombinationLUT(void);

    /* True trails support. */
    SPxErrorCode trueTrailsFreeInfo(void);
    SPxErrorCode trueTrailsMapToAzis(void);
    SPxErrorCode trueTrailsSetupBufs(void);
    SPxErrorCode trueTrailsRedraw(int bufferIdx);
    SPxErrorCode trueTrailsClearBitmapsLocked(void);
    SPxErrorCode trueTrailsClearColumns(SPxScLocalTrueTrailsBitmapInfo_t *info,
					int firstCol, int numCols);
    SPxErrorCode trueTrailsClearRows(SPxScLocalTrueTrailsBitmapInfo_t *info,
					int firstRow, int numRows);
    SPxErrorCode trueTrailsScrollBitmaps(REAL32 xPixels, REAL32 yPixels);

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;		/* Destination for debug messages */

    /* View control callback. */
    static int viewControlHandler(void *invokingObj,
				  void *contextPtr,
				  void *changeFlagsPtr);
    static int viewControlDeleted(void *invokingObject,
				  void *userObject,
				  void *arg);
    
}; /* SPxScSourceLocal */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_SC_SOURCE_LOCAL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
