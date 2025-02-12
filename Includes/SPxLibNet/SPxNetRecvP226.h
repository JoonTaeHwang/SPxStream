/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for project-226 network receiving module.
*
*
* Revision Control:
*   28/07/22 v1.20   AJH	Support actual serial number.
*
* Previous Changes:
*   21/07/22 1.19   AJH	Query support for interference settings.
*   19/07/22 1.18   AJH	Add m_sectorBlankSupported.
*   02/09/21 1.17   AJH	Add m_useModesSupported.
*   21/05/21 1.16   AJH	Only set SPX_SUPPORT_P226 if not already defined.
*   19/11/20 1.15   BTB	Make constructor explicit.
*   04/09/20 1.14   AJH	Add support for reset device IDs.
*   16/03/20 1.13   AJH	Fix Linux build.
*   13/03/20 1.12   AJH	Support SDK V3.5.00.
*   23/05/18 1.11   BTB	Support getting radar type as a string.
*   17/01/18 1.10   BTB	Add functions and parameters for extra radar settings.
*   23/09/16 1.9    AGC	Improve mutex behaviour.
*   28/10/15 1.8    REW	Add license helper class.
*   24/09/14 1.7    AGC	Improve multiple source connection behaviour.
*   04/03/14 1.6    REW	Add support for watchdog.
*   17/02/14 1.5    SP 	Add support for STC curve.
*   04/10/13 1.4    AGC	Simplify headers.
*   13/03/13 1.3    REW	Start linux support.
*   13/12/12 1.2    REW	Add options for radar height and azi offset.
*   12/12/12 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECVP226_H
#define _SPX_NETRECVP226_H

/* The SDK does not support VS versions earlier than 2015. */
#define SPX_NET_RECV_P226_MIN_MSC_VER	    1900

#ifndef SPX_SUPPORT_P226
#ifdef _WIN32
#if _MSC_VER >= SPX_NET_RECV_P226_MIN_MSC_VER
#define SPX_SUPPORT_P226 1
#endif
#else
#define SPX_SUPPORT_P226 1
#endif
#endif

#if SPX_SUPPORT_P226
/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need the radar source base class header. */
#include "SPxLibData/SPxRadarSource.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Debug flags.
 */
#define	SPX_NET_RECV_P226_DEBUG_STATE	0x00000001	/* State */
#define	SPX_NET_RECV_P226_DEBUG_PARAMS	0x00000002	/* Parameters */
#define	SPX_NET_RECV_P226_DEBUG_RADARS	0x00000010	/* Radars */
#define	SPX_NET_RECV_P226_DEBUG_SPOKE	0x00000100	/* Spoke messages */
#define	SPX_NET_RECV_P226_DEBUG_STATUS	0x00000200	/* Status messages */
#define	SPX_NET_RECV_P226_DEBUG_VERBOSE	0x80000000	/* Verbose */


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

/* Forward declare the classes we use. */
class SPxNetRecvP226Helper;
class SPxNetRecvP226LicenseHelper;
class SPxRIB;
class SPxThread;

/*
 * Define our receiver class, derived from generic radar source, not from
 * common network receiver base class.
 */
class SPxNetworkReceiveP226 :public SPxRadarSource
{
/* The helper class can access our private members. */
friend class SPxNetRecvP226Helper;

public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    explicit SPxNetworkReceiveP226(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP226();
    virtual SPxErrorCode Create(void);

    /* Object control. */
    SPxErrorCode SetSerialNumber(const char *serialNumber);
    const char *GetSerialNumber(void)	{ return(m_serialNumber); }
    const char *GetActualSerialNumber(void) { return(m_actualSerialNumber ? m_actualSerialNumber : m_serialNumber); }
    SPxErrorCode SetLicenseKey(const char *licenseKey);
    const char *GetLicenseKey(void)	{ return(m_licenseKey); }
    SPxErrorCode SetImageStream(unsigned int imageStream);
    unsigned int GetImageStream(void)	{ return(m_imageStream); }

    /* Status. */
    int GetNumRadars(void)	{ return(m_numRadars); }
    int GetLicenseResult(void)	{ return(m_licenseResult); }

    /* Radar properties. */
    unsigned int GetRadarType(void)	{ return(m_radarType); }
    const char *GetRadarTypeString(unsigned int type);
    unsigned int GetRadarPowerCycles(void) { return(m_radarPowerCycles); }
    unsigned int GetRadarUpTimeHours(void) { return(m_radarUpTime); }
    double GetRadarMaxRangeMetres(void)	{ return(m_radarMaxRangeMetres); }
    SPxErrorCode SetRadarHeightMetres(double metres);
    double GetRadarHeightMetres(void)	{ return(m_radarHeightMetres); }
    double GetActualRadarHeightMetres(void)
				{ return(m_actualRadarHeightMetres); }
    SPxErrorCode SetRadarZeroBearingDegs(double degrees);
    double GetRadarZeroBearingDegs(void) { return(m_radarZeroBearingDegs); }
    double GetActualRadarZeroBearingDegs(void)
				{ return(m_actualRadarZeroBearingDegs); }
    SPxErrorCode SetParkPositionDegs(double degrees);
    double GetParkPositionDegs(void) { return(m_parkPositionDegs); }
    double GetActualParkPositionDegs(void) { return(m_actualParkPositionDegs); }

    /* Radar state. */
    unsigned int GetRadarState(void) { return(m_radarModeState); }
    const char *GetRadarStateString(unsigned int state);
    unsigned int GetRadarWarmupSecs(void) { return(m_radarModeWarmupSecs); }

    /* Watchdog timer. */
    SPxErrorCode SetWatchdogMsecs(unsigned int msecs);
    unsigned int GetWatchdogMsecs(void)	{ return(m_watchdogMsecs); }

    /*
     * Radar control.
     */
    SPxErrorCode QueryRadar(void);
    SPxErrorCode ResetDeviceIDs(void);

    /* Range. */
    SPxErrorCode SetRangeMetres(double rangeMetres);
    double GetRangeMetres(void)		{ return(m_rangeMetres); }
    double GetActualRangeMetres(void)	{ return(m_actualRangeMetres); }

    /* Gain. */
    SPxErrorCode SetGain(unsigned int level, int automatic);
    unsigned int GetGainLevel(void)	{ return(m_gainLevel); }
    int GetGainAuto(void)		{ return(m_gainAuto); }
    unsigned int GetActualGainLevel(void) { return(m_actualGainLevel); }
    int GetActualGainAuto(void)		{ return(m_actualGainAuto); }

    /* Sea-clutter. */
    SPxErrorCode SetSeaClutter(unsigned int level, int autoMode);
    unsigned int GetSeaClutterLevel(void) { return(m_seaClutterLevel); }
    int GetSeaClutterAuto(void)		{ return(m_seaClutterAuto); }
    unsigned int GetActualSeaClutterLevel(void)
					{ return(m_actualSeaClutterLevel); }
    int GetActualSeaClutterAuto(void)	{ return(m_actualSeaClutterAuto); }
    
    /* Sea-clutter STC curve type. */
    SPxErrorCode SetSTCCurveType(int type);
    unsigned int GetSTCCurveType(void)       { return(m_stcCurveType); }
    unsigned int GetActualSTCCurveType(void) { return(m_actualSTCCurveType); }

    /* Rain. */
    SPxErrorCode SetRain(unsigned int level);
    unsigned int GetRainLevel(void)	{ return(m_rainLevel); }
    unsigned int GetActualRainLevel(void) { return(m_actualRainLevel); }

    /* FTC (may also be affected by rain settings). */
    SPxErrorCode SetFTC(unsigned int level);
    unsigned int GetFTCLevel(void)	{ return(m_ftcLevel); }
    unsigned int GetActualFTCLevel(void) { return(m_actualFTCLevel); }

    /* Interference rejection. */
    int IsInterferenceRejectSupported(void) { return m_interferenceRejectSupported; }
    SPxErrorCode SetInterferenceReject(unsigned int level);
    unsigned int GetInterferenceReject(void) { return(m_interferenceReject);}
    unsigned int GetActualInterferenceReject(void)
					{ return(m_actualInterferenceReject);}

    /* Rotation rate. */
    SPxErrorCode SetFastScanMode(unsigned int level);
    unsigned int GetFastScanMode(void)		{ return(m_fastScanMode); }
    unsigned int GetActualFastScanMode(void) { return(m_actualFastScanMode); }
    double GetActualRPM(void)			{ return(m_actualRPM); }

    /* Target stretch. */
    SPxErrorCode SetTargetStretch(unsigned int enable);
    unsigned int GetTargetStretch(void)		{ return(m_targetStretch); }
    unsigned int GetActualTargetStretch(void) { return(m_actualTargetStretch);}

    /* Target boost. */
    SPxErrorCode SetTargetBoost(unsigned int level);
    unsigned int GetTargetBoost(void)		{ return(m_targetBoost); }
    unsigned int GetActualTargetBoost(void)	{ return(m_actualTargetBoost);}

    /* Local interference rejection. */
    int IsLocalIRSupported(void) { return m_localIRSupported; }
    SPxErrorCode SetLocalIR(unsigned int level);
    unsigned int GetLocalIR(void)		{ return(m_localIR); }
    unsigned int GetActualLocalIR(void)		{ return(m_actualLocalIR);}

    /* Noise rejection. */
    SPxErrorCode SetNoiseReject(unsigned int level);
    unsigned int GetNoiseReject(void)		{ return(m_noiseReject); }
    unsigned int GetActualNoiseReject(void)	{ return(m_actualNoiseReject);}

    /* Beam sharpening. */
    SPxErrorCode SetBeamSharpening(unsigned int level);
    unsigned int GetBeamSharpening(void)	{ return(m_beamSharpening); }
    unsigned int GetActualBeamSharpening(void)
					{ return(m_actualBeamSharpening);}

    /* Use mode. */
    SPxErrorCode SetUseMode(unsigned int mode);
    unsigned int GetUseMode(void)		{ return(m_useMode); }
    unsigned int GetActualUseMode(void)		{ return(m_actualUseMode); }

    /* Side lobe. */
    SPxErrorCode SetSideLobe(unsigned int level, int autoMode);
    unsigned int GetSideLobeLevel(void)		{ return(m_sideLobeLevel); }
    int GetSideLobeAuto(void)			{ return(m_sideLobeAuto); }
    unsigned int GetActualSideLobeLevel(void)
    {
	return(m_actualSideLobeLevel);
    }
    int GetActualSideLobeAuto(void)		{ return(m_actualSideLobeAuto); }

    /* LEDs level. */
    SPxErrorCode SetLEDsLevel(unsigned int level);
    unsigned int GetLEDsLevel(void)		{ return(m_ledsLevel); }
    unsigned int GetActualLEDsLevel(void)	{ return(m_actualLEDsLevel); }

    /* User Min SNR. */
    SPxErrorCode SetUserMinSNR(double level_dB);
    double GetUserMinSNR(void) { return(m_userMinSNR); }
    double GetActualUserMinSNR(void) { return(m_actualUserMinSNR); }

    /* Video Aperture. */
    SPxErrorCode SetVideoAperture(double level_dB);
    double GetVideoAperture(void) { return(m_videoAperture); }
    double GetActualVideoAperture(void) { return(m_actualVideoAperture); }

    /* Transmit. */
    SPxErrorCode SetTransmitting(unsigned int enable);
    unsigned int GetTransmitting(void) { return(m_transmitting); }

    /* Sector Blanking. */
    int IsSectorBlankingSupported(void) { return m_sectorBlankSupported; }
    SPxErrorCode SetSectorBlanking(unsigned int sectorID, unsigned int enable);
    unsigned int GetSectorBlanking(unsigned int sectorID)
				{ return(m_sectorBlanking[sectorID]); }
    unsigned int GetActualSectorBlanking(unsigned int sectorID)
				{ return(m_actualSectorBlanking[sectorID]); }
    SPxErrorCode SetSectorBlankingStartDegs(unsigned int sectorID,
					 double start_degrees);
    double GetSectorBlankingStartDegs(unsigned int sectorID)
				{ return(m_sectorBlankingStartDegs[sectorID]); }
    double GetActualSectorBlankingStartDegs(unsigned int sectorID)
				{ return(m_actualSectorBlankingStartDegs[sectorID]); }
    SPxErrorCode SetSectorBlankingEndDegs(unsigned int sectorID,
					 double end_degrees);
    double GetSectorBlankingEndDegs(unsigned int sectorID)
				{ return(m_sectorBlankingEndDegs[sectorID]); }
    double GetActualSectorBlankingEndDegs(unsigned int sectorID)
				{ return(m_actualSectorBlankingEndDegs[sectorID]); }

    /*
     * Parameter assignment.
     */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /* Static public functions for log file support. */
    static void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    static FILE *GetLogFile(void)		{ return(m_logFile); }
    static void SetDebug(UINT32 debug)
    {
	m_debug = debug;
	if( (debug != 0) && (GetLogFile() == NULL) )
	{
	    SetLogFile(stdout);
	}
    }
    static UINT32 GetDebug(void) { return m_debug; }

private:
    /*
     * Private fields.
     */
    /* Worker thread. */
    SPxThread *m_thread;	/* Handle for thread object. */

    /* Radar input buffer where data is written. */
    SPxRIB *m_rib;
    int m_ribFull;		/* Do we think RIB filled up? */

    /* Critical section protection. */
    SPxCriticalSection m_mutex;

    /* Object control options. */
    char *m_serialNumber;	/* Serial number to connect to */
    char *m_actualSerialNumber;	/* Serial number actually used */
    char *m_licenseKey;		/* License key to use */
    unsigned int m_imageStream;	/* Number of image stream to use */

    /* Watchdog timeout. */
    unsigned int m_watchdogMsecs;	/* Milliseconds, or 0 to disable */

    /* Feature retrieval timer. */
    unsigned int m_featureMsecs;

    /* Radar properties. */
    unsigned int m_radarType;		/* Type of radar */
    unsigned int m_radarPowerCycles;	/* Number of power cycles */
    unsigned int m_radarUpTime;		/* Transmit time in hours */
    double m_radarMaxRangeMetres;	/* Reported max range */
    double m_radarHeightMetres;		/* Configured height */
    double m_actualRadarHeightMetres;	/* Reported configured height */
    double m_radarZeroBearingDegs;	/* Configured zero bearing offset */
    double m_actualRadarZeroBearingDegs; /* Reported zero bearing offset */
    double m_parkPositionDegs;		/* Configured parking position */
    double m_actualParkPositionDegs;	/* Reported parking position */

    /* Radar control options. */
    double m_rangeMetres;		/* Requested range. */
    double m_actualRangeMetres;		/* Value reported by radar */
    unsigned int m_gainLevel;		/* Level for manual gain (0 to 255) */
    unsigned int m_actualGainLevel;	/* Value reported by radar */
    int m_gainAuto;			/* 0 for manual, non-zero auto */
    int m_actualGainAuto;		/* Value reported by radar */
    unsigned int m_seaClutterLevel;	/* Manual level (0 to 255) */
    unsigned int m_actualSeaClutterLevel; /* Value reported by radar */
    int m_seaClutterAuto;		/* 0 for manual, non-zero auto */
    int m_actualSeaClutterAuto;		/* Value reported by radar */
    unsigned int m_stcCurveType;        /* 0 for calm, 1 for moderate and 2 for rough. */
    unsigned int m_actualSTCCurveType;  /* Value reported by radar */
    unsigned int m_rainLevel;		/* Manual level (0 to 255) */
    unsigned int m_actualRainLevel;	/* Value reported by radar */
    unsigned int m_ftcLevel;		/* Manual level (0 to 255) */
    unsigned int m_actualFTCLevel;	/* Value reported by radar */
    unsigned int m_interferenceReject;	/* Manual level (0 to 3) */
    unsigned int m_actualInterferenceReject; /* Value reported by radar */
    unsigned int m_fastScanMode;	/* Requested setting */
    unsigned int m_actualFastScanMode;	/* Value reported by radar */
    double m_actualRPM;			/* Value reported by radar */
    unsigned int m_targetStretch;	/* Requested setting */
    unsigned int m_actualTargetStretch;	/* Value reported by radar */
    unsigned int m_targetBoost;		/* Requested setting */
    unsigned int m_actualTargetBoost;	/* Value reported by radar */
    unsigned int m_localIR;		/* Requested setting */
    unsigned int m_actualLocalIR;	/* Value reported by radar */
    unsigned int m_noiseReject;		/* Requested setting */
    unsigned int m_actualNoiseReject;	/* Value reported by radar */
    unsigned int m_beamSharpening;	/* Requested setting */
    unsigned int m_actualBeamSharpening; /* Value reported by radar */
    unsigned int m_useMode;		/* Requested setting */
    unsigned int m_actualUseMode;	/* Value reported by radar */
    unsigned int m_sideLobeLevel;	/* Manual level (0 to 255) */
    unsigned int m_actualSideLobeLevel; /* Value reported by radar */
    int m_sideLobeAuto;			/* 0 for manual, non-zero auto */
    int m_actualSideLobeAuto;		/* Value reported by radar */
    unsigned int m_ledsLevel;		/* Requested setting */
    unsigned int m_actualLEDsLevel;	/* Value reported by radar */
    double m_userMinSNR;		/* Requested setting */
    double m_actualUserMinSNR;		/* Value reported by radar */
    double m_videoAperture;		/* Requested setting */
    double m_actualVideoAperture;	/* Value reported by radar */
    unsigned int m_transmitting;	/* Requested state */
    unsigned int m_sectorBlanking[6];	/* Requested state */
    unsigned int m_actualSectorBlanking[6]; /* State reported by radar */
    double m_sectorBlankingStartDegs[6]; /* Requested setting */
    double m_actualSectorBlankingStartDegs[6]; /* Value reported by radar */
    double m_sectorBlankingEndDegs[6];	/* Requested setting */
    double m_actualSectorBlankingEndDegs[6]; /* Value reported by radar */

    /* Radar control objects. */
    SPxNetRecvP226Helper *m_helper;
    SPxNetRecvP226LicenseHelper *m_licenseHelper;

    /* Radar information. */
    unsigned int m_radarModeState;	/* State from mode message */
    unsigned int m_radarModeWarmupSecs;	/* Time from mode message */

    /* PRF estimation. */
    UINT32 m_prfRefTimeMsecs;			/* Reference time */
    unsigned int m_prfSpokesSinceLastRef;	/* Counter */
    unsigned int m_prfUsecsPerSpoke;		/* Inter-spoke timing */

    /* Buffer for decoding. */
    unsigned char *m_decodeBuffer;
    unsigned int m_decodeBufferSize;

    /* Status flags etc. */
    unsigned int m_dllAvailable;	/* Was DLL found? */
    SPxAtomic<int> m_numRadars;		/* How many radars found? */
    int m_licenseResult;		/* License level */
    int m_multiRadarConnected;		/* Multi-radar object connected? */
    int m_useModesSupported;		/* Radar supports use modes? */
    int m_sectorBlankSupported;		/* Radar supports sector blank? */
    int m_interferenceRejectSupported;	/* Radar supports interference rejection? */
    int m_localIRSupported;		/* Radar supports local IR? */

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;	/* Destination for debug messages */
    static UINT32 m_debug;	/* Debug flags */

    /*
     * Private functions.
     */
    static void *threadWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);

}; /* SPxNetworkReceiveP226 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_SUPPORT_P226 */
#endif /* _SPX_NETRECVP226_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
