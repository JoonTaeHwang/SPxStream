/*********************************************************************
*
* (c) Copyright 2019 - 2020, 2022, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for Project 430 network receiving module.
*
*
* Revision Control:
*   01/09/22 v1.8    AJH	Fix last change.
*
* Previous Changes:
*   01/09/22 1.7    AJH	Fix last change.
*   01/09/22 1.6    AJH	Add NoInit feature.
*   05/04/22 1.5    AJH	Add default radar number for some operations.
*   12/01/22 1.4    AJH	Support range number for dual-range radars.
*   20/11/20 1.3    AJH	Implement range scale query for remote control.
*   19/11/20 1.2    BTB	Make constructor explicit.
*   04/06/19 1.1    AJH	Initial version.
**********************************************************************/

#ifndef _SPX_NETRECVP430_H
#define _SPX_NETRECVP430_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need the radar source base class header. */
#include "SPxLibData/SPxRadarSource.h"

/* We currently only have Windows support for Furuno library for 64-bit. */
#ifdef _WIN64
#define SPX_SUPPORT_P430
#endif

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Debug flags.
 */
#define	SPX_NET_RECV_P430_DEBUG_STATE	0x00000001	/* State */
#define	SPX_NET_RECV_P430_DEBUG_PARAMS	0x00000002	/* Parameters */
#define	SPX_NET_RECV_P430_DEBUG_RADARS	0x00000010	/* Radars */
#define	SPX_NET_RECV_P430_DEBUG_SPOKE	0x00000100	/* Spoke messages */
#define	SPX_NET_RECV_P430_DEBUG_STATUS	0x00000200	/* Status messages */
#define	SPX_NET_RECV_P430_DEBUG_VERBOSE	0x80000000	/* Verbose */

/* Maximum number of blind sectors. */
#define SPX_NET_RECV_P430_NUM_BLIND_SECTOR  2

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
class SPxRIB;
class SPxThread;

struct SPxNetworkReceiveP430BlindSector
{
    unsigned int startDegs;
    unsigned int widthDegs;
};

/*
 * Define our receiver class, derived from generic radar source, not from
 * common network receiver base class.
 */
class SPxNetworkReceiveP430 : public SPxRadarSource
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    explicit SPxNetworkReceiveP430(SPxRIB *buffer, unsigned int instance=0);
    virtual ~SPxNetworkReceiveP430();
    virtual SPxErrorCode Create(void);

    /* Watchdog timer. */
    SPxErrorCode SetWatchdogMsecs(unsigned int msecs);
    unsigned int GetWatchdogMsecs(void)	{ return(m_watchdogMsecs); }

    static SPxNetworkReceiveP430 *m_instanceTable[2];

    SPxErrorCode SetInterface(const char *interfaceString);
    const char *GetInterface(void) { return (m_interface) ? m_interface : ""; }
    SPxErrorCode SetRadarName(const char *radarName);
    const char *GetRadarName(void) { return ( m_radarName ) ? m_radarName : ""; }
    const char *GetRadarNameActual(void) { return m_radarNameActual; }
    SPxErrorCode SetRangeNumber(unsigned int rangeNumber);
    unsigned int GetRangeNumber(void) { return m_rangeNumber; }

    /* Radar configuration. */
    unsigned int GetNumRadars(void) { return( m_numRadars ); }

    char *GetRadarType(void);
    unsigned int GetRadarOnTime(void) { return m_radarOnTimeSecs; }
    unsigned int GetRadarTxTime(void) { return m_radarTxTimeSecs; }

    SPxErrorCode SetRangeScale(unsigned int rangeScale);
    unsigned int GetRangeScale(void) { return( m_rangeScale ); }
    unsigned int GetRangeScaleActual(void) { return( m_rangeScaleActual ); }
    double GetRangeMetresActual(void);

    SPxErrorCode SetGain(unsigned int gainLevel, unsigned int gainAuto);
    unsigned int GetGainLevel(void) { return( m_gainLevel ); }
    unsigned int GetGainLevelActual(void) { return( m_gainLevelActual ); }
    unsigned int GetGainAuto(void) { return( m_gainAuto ); }
    unsigned int GetGainAutoActual(void) { return( m_gainAutoActual ); }

    SPxErrorCode SetSeaClutter( unsigned int seaClutterLevel, unsigned int seaClutterAuto );
    unsigned int GetSeaClutterLevel(void) { return( m_seaClutterLevel ); }
    unsigned int GetSeaClutterLevelActual(void) { return( m_seaClutterLevelActual ); }
    unsigned int GetSeaClutterAuto(void) { return( m_seaClutterAuto ); }
    unsigned int GetSeaClutterAutoActual(void) { return( m_seaClutterAutoActual ); }

    SPxErrorCode SetRain(unsigned int rainClutterLevel, unsigned int rainClutterAuto, unsigned int ftcMode);
    unsigned int GetRainLevel(void) { return( m_rainLevel ); }
    unsigned int GetRainLevelActual(void) { return( m_rainLevelActual ); }
    unsigned int GetRainAuto(void) { return( m_rainAuto ); }
    unsigned int GetRainAutoActual(void) { return( m_rainAutoActual ); }
    unsigned int GetRainFTCMode(void) { return( m_rainFTCMode ); }
    unsigned int GetRainFTCModeActual(void) { return( m_rainFTCModeActual ); }

    SPxErrorCode SetSTC(unsigned int nearSTC, unsigned int middleSTC, unsigned int farSTC);
    unsigned int GetNearSTC(void) { return( m_nearSTC ); }
    unsigned int GetNearSTCActual(void) { return( m_nearSTCActual ); }
    unsigned int GetMiddleSTC(void) { return( m_middleSTC ); }
    unsigned int GetMiddleSTCActual(void) { return( m_middleSTCActual ); }
    unsigned int GetFarSTC(void) { return( m_farSTC ); }
    unsigned int GetFarSTCActual(void) { return( m_farSTCActual ); }

    SPxErrorCode SetTransmitEnable(unsigned int transmitEnable);
    unsigned int GetTransmitEnable(void) { return( m_transmitEnable ); }
    unsigned int GetTransmitEnableActual(void) { return( m_transmitEnableActual ); }

    SPxErrorCode SetNoInit(unsigned int noInit) { m_noInit = noInit; return SPX_NO_ERROR; }
    unsigned int GetNoInit(void) { return(m_noInit); }

    SPxErrorCode SetBlindSector(unsigned int sectorNum, unsigned int startDegs, unsigned int widthDegs);
    unsigned int GetBlindSectorStart(unsigned int sectorNum)
	{ return ( m_blindSector[MIN(sectorNum, SPX_NET_RECV_P430_NUM_BLIND_SECTOR - 1)].startDegs ); }
    unsigned int GetBlindSectorStartActual(unsigned int sectorNum)
	{ return ( m_blindSectorActual[MIN(sectorNum, SPX_NET_RECV_P430_NUM_BLIND_SECTOR - 1)].startDegs ); }
    unsigned int GetBlindSectorWidth(unsigned int sectorNum)
	{ return ( m_blindSector[MIN(sectorNum, SPX_NET_RECV_P430_NUM_BLIND_SECTOR - 1)].widthDegs ); }
    unsigned int GetBlindSectorWidthActual(unsigned int sectorNum)
	{ return ( m_blindSectorActual[MIN(sectorNum, SPX_NET_RECV_P430_NUM_BLIND_SECTOR - 1)].widthDegs ); }

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

    void HandleEcho(int no, short status, char *echo, short sweep_len, short scale,
	short rangeScale, short angle, short heading, short h_flg);

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

    /* Watchdog timeout. */
    unsigned int m_watchdogMsecs;	/* Milliseconds, or 0 to disable */

    /* Last time of status check. */
    unsigned int m_lastStatusCheckMsecs;

    /* Our interface to use. */
    char *m_interface;

    /* Radar configuration. */
    char *m_radarName;
    char m_radarNameActual[9];
    unsigned int m_radarNum;
    unsigned int m_radarNumDefault;
    unsigned int m_rangeNumber;
    unsigned int m_rangeScale;
    unsigned int m_rangeScaleActual;
    unsigned int m_transmitEnable;
    unsigned int m_transmitEnableActual;
    SPxNetworkReceiveP430BlindSector m_blindSector[SPX_NET_RECV_P430_NUM_BLIND_SECTOR];
    SPxNetworkReceiveP430BlindSector m_blindSectorActual[SPX_NET_RECV_P430_NUM_BLIND_SECTOR];
    unsigned int m_gainLevel;
    unsigned int m_gainLevelActual;
    unsigned int m_gainAuto;
    unsigned int m_gainAutoActual;
    unsigned int m_seaClutterLevel;
    unsigned int m_seaClutterLevelActual;
    unsigned int m_seaClutterAuto;
    unsigned int m_seaClutterAutoActual;
    unsigned int m_rainLevel;
    unsigned int m_rainLevelActual;
    unsigned int m_rainAuto;
    unsigned int m_rainAutoActual;
    unsigned int m_rainFTCMode;
    unsigned int m_rainFTCModeActual;
    unsigned int m_nearSTC;
    unsigned int m_nearSTCActual;
    unsigned int m_middleSTC;
    unsigned int m_middleSTCActual;
    unsigned int m_farSTC;
    unsigned int m_farSTCActual;
    unsigned int m_radarModel;
    unsigned int m_radarLength;
    unsigned int m_radarOnTimeSecs;
    unsigned int m_radarTxTimeSecs;

    /* PRF estimation. */
    UINT32 m_prfRefTimeMsecs;			/* Reference time */
    unsigned int m_prfSpokesSinceLastRef;	/* Counter */
    unsigned int m_prfUsecsPerSpoke;		/* Inter-spoke timing */

    /* Buffer for decoding. */
    unsigned char *m_decodeBuffer;
    unsigned int m_decodeBufferSize;

    /* Status flags etc. */
    SPxAtomic<int> m_numRadars;		/* How many radars found? */
    unsigned int m_instance;		/* Number of this instance of the class. */

    /* Watchdog support. */
    UINT32 m_lastMessageMsecs;

    /* Radar spoke sequence number. */
    unsigned int m_sequenceNumber;

    /* Connected to radar? */
    unsigned int m_connected;

    /* Suppress initialisation? */
    unsigned int m_noInit;

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;	/* Destination for debug messages */
    static unsigned int m_debug;	/* Debug flags */
    static unsigned int m_isInitialised; /* Is Furuno library initialised? */

    /*
     * Private functions.
     */
    static void *threadWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);
    void checkRadarStatus(void);
#ifdef _WIN32
    int vendorLicense(const char *vendorCode, const char *vendorPassword,
	const char *vendorId, const char *deviceInfo);
    int targetLicense(const char *filePath);
#endif

#ifdef _WIN32
    static void hexStringToBytes(const char *strIn, unsigned char* &array, int& size);
#endif
    static double getRadarRange(unsigned int rangeScale);

}; /* SPxNetworkReceiveP430 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVP430_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
