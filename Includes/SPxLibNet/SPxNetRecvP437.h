/*********************************************************************
*
* (c) Copyright 2019 - 2020, 2022, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for Project 437 network receiving module.
*
*
* Revision Control:
*   08/06/22 v1.6    AJH	Add support for Doppler mode.
*
* Previous Changes:
*   19/11/20 1.5    BTB	Make constructor explicit.
*   12/11/19 1.4    AJH	Support channel selection.
*   07/11/19 1.3    AJH	Add radar description.
*   04/11/19 1.2    AJH	Fix compiler warning.
*   28/10/19 1.1    AJH	Initial version.
**********************************************************************/

#ifndef _SPX_NETRECVP437_H
#define _SPX_NETRECVP437_H

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
#define	SPX_NET_RECV_P437_DEBUG_STATE	0x00000001	/* State */
#define	SPX_NET_RECV_P437_DEBUG_PARAMS	0x00000002	/* Parameters */
#define	SPX_NET_RECV_P437_DEBUG_RADARS	0x00000010	/* Radars */
#define	SPX_NET_RECV_P437_DEBUG_SPOKE	0x00000100	/* Spoke messages */
#define	SPX_NET_RECV_P437_DEBUG_STATUS	0x00000200	/* Status messages */
#define	SPX_NET_RECV_P437_DEBUG_VERBOSE	0x80000000	/* Verbose */

/* Maximum number of instrumented ranges. */
#define NUM_RADAR_RANGES		    20

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
class SPxNetworkReceiveP437Notifier;
class SPxDopplerStore;

/*
 * Define our receiver class, derived from generic radar source, not from
 * common network receiver base class.
 */
class SPxNetworkReceiveP437 : public SPxRadarSource
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    explicit SPxNetworkReceiveP437(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP437();
    virtual SPxErrorCode Create(void);

    /* Watchdog timer. */
    SPxErrorCode SetWatchdogMsecs(unsigned int msecs);
    unsigned int GetWatchdogMsecs(void)	{ return(m_watchdogMsecs); }

    SPxErrorCode SetRadarSerialNumber(UINT32 serialNumber);
    UINT32 GetRadarSerialNumber(void) { return m_radarSerialNumber;  }
    UINT32 GetRadarSerialNumberActual(void) { return m_radarSerialNumberActual; }

    SPxErrorCode SetChannel(unsigned int channel);
    unsigned int GetChannel(void) { return( m_channel ); }

    /* Radar configuration. */
    SPxErrorCode SetNumRadars(unsigned int numRadars);
    unsigned int GetNumRadars(void) { return( m_numRadars ); }
    char *GetRadarDescription(void) { return m_radarDescription; }
    SPxErrorCode SetRadarRangeTable(unsigned int i, double radarRangeNM);

    SPxErrorCode SetRangeScale(unsigned int rangeScale);
    unsigned int GetRangeScale(void) { return( m_rangeScale ); }
    void SetRangeScaleActual(unsigned int rangeScale) { m_rangeScaleActual = rangeScale; }
    unsigned int GetRangeScaleActual(void) { return( m_rangeScaleActual ); }
    double GetRangeMetresActual(void);

    SPxErrorCode SetGain(unsigned int gainLevel, unsigned int gainAuto);
    unsigned int GetGainLevel(void) { return( m_gainLevel ); }
    unsigned int GetGainAuto(void) { return( m_gainAuto ); }
    void SetGainLevelActual(unsigned int gainLevel) { m_gainLevelActual = gainLevel; }
    unsigned int GetGainLevelActual(void) { return( m_gainLevelActual ); }
    void SetGainAutoActual(unsigned int gainAuto) { m_gainAutoActual = gainAuto; }
    unsigned int GetGainAutoActual(void) { return( m_gainAutoActual ); }

    SPxErrorCode SetSea( unsigned int seaLevel, unsigned int seaAuto );
    unsigned int GetSeaLevel(void) { return( m_seaLevel ); }
    unsigned int GetSeaAuto(void) { return( m_seaAuto ); }
    void SetSeaLevelActual(unsigned int seaLevel) { m_seaLevelActual = seaLevel; }
    unsigned int GetSeaLevelActual(void) { return( m_seaLevelActual ); }
    void SetSeaAutoActual(unsigned int seaAuto) { m_seaAutoActual = seaAuto; }
    unsigned int GetSeaAutoActual(void) { return( m_seaAutoActual ); }

    SPxErrorCode SetRain(unsigned int rainLevel, unsigned int rainOn);
    unsigned int GetRainLevel(void) { return( m_rainLevel ); }
    unsigned int GetRainOn(void) { return( m_rainOn ); }
    void SetRainLevelActual(unsigned int rainLevel) { m_rainLevelActual = rainLevel; }
    unsigned int GetRainLevelActual(void) { return( m_rainLevelActual ); }
    void SetRainOnActual(unsigned int rainOn) { m_rainOnActual = rainOn; }
    unsigned int GetRainOnActual(void) { return( m_rainOnActual ); }

    SPxErrorCode SetTransmitEnable(unsigned int transmitEnable);
    unsigned int GetTransmitEnable(void) { return( m_transmitEnable ); }
    void SetTransmitEnableActual(unsigned int transmitEnable) { m_transmitEnableActual = transmitEnable; }
    unsigned int GetTransmitEnableActual(void) { return( m_transmitEnableActual ); }

    SPxErrorCode SetInterferenceMode(unsigned int interfMode);
    unsigned int GetInterferenceMode(void) { return( m_interfMode ); }
    void SetInterferenceModeActual(unsigned int interfMode) { m_interfModeActual = interfMode; }
    unsigned int GetInterferenceModeActual(void) { return( m_interfModeActual ); }

    SPxErrorCode SetDopplerMode(unsigned int dopplerMode);
    unsigned int GetDopplerMode(void) { return(m_dopplerMode); }
    void SetDopplerModeActual(unsigned int dopplerMode) { m_dopplerModeActual = dopplerMode; }
    unsigned int GetDopplerModeActual(void) { return(m_dopplerModeActual); }
    void SetDopplerActive(unsigned int dopplerActive) { m_dopplerActive = dopplerActive; }
    unsigned int GetDoppleActive(void) { return(m_dopplerActive); }

    /* Doppler support. */
    SPxErrorCode SetDopplerStore(SPxDopplerStore *ds);
    SPxDopplerStore *GetDopplerStore(void) const { return(m_dopplerStore); }
    SPxErrorCode SetDopplerFormat(unsigned int format);
    unsigned int GetDopplerFormat(void) const { return(m_dopplerFormat); }

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

    void handleEcho(unsigned int samples,
	unsigned int spokesPerScan, unsigned int range,
	unsigned int bearing, unsigned int dataLength, unsigned char *spokeData);

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

    /* QuantumLib notifier. */
    SPxNetworkReceiveP437Notifier *m_notifier;

    /* Radar configuration. */
    UINT32 m_radarSerialNumber;
    UINT32 m_radarSerialNumberActual;
    char m_radarDescription[32];
    double m_radarRangesNM[NUM_RADAR_RANGES];
    unsigned int m_channel;
    unsigned int m_rangeScale;
    unsigned int m_rangeScaleActual;
    unsigned int m_transmitEnable;
    unsigned int m_transmitEnableActual;
    unsigned int m_gainLevel;
    unsigned int m_gainLevelActual;
    unsigned int m_gainAuto;
    unsigned int m_gainAutoActual;
    unsigned int m_seaLevel;
    unsigned int m_seaLevelActual;
    unsigned int m_seaAuto;
    unsigned int m_seaAutoActual;
    unsigned int m_rainLevel;
    unsigned int m_rainLevelActual;
    unsigned int m_rainOn;
    unsigned int m_rainOnActual;
    unsigned int m_interfMode;
    unsigned int m_interfModeActual;
    unsigned int m_dopplerMode;
    unsigned int m_dopplerModeActual;
    unsigned int m_dopplerActive;

    /* PRF estimation. */
    UINT32 m_prfRefTimeMsecs;			/* Reference time */
    unsigned int m_prfSpokesSinceLastRef;	/* Counter */
    unsigned int m_prfUsecsPerSpoke;		/* Inter-spoke timing */

    /* Buffer for decoding. */
    unsigned char *m_decodeBuffer;
    unsigned int m_decodeBufferSize;

    /* Status flags etc. */
    SPxAtomic<int> m_numRadars;		/* How many radars found? */

    /* Watchdog support. */
    UINT32 m_lastMessageMsecs;

    /* Radar spoke sequence number. */
    unsigned int m_sequenceNumber;

    /* Connected to radar? */
    unsigned int m_connected;

    /* Doppler format. */
    SPxDopplerStore *m_dopplerStore;	/* Doppler store */
    unsigned int m_dopplerFormat;	/* SPX_DOPPLER_FORMAT_... */

    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;		    /* Destination for debug messages */
    static unsigned int m_debug;	    /* Debug flags */
    static unsigned int m_isInitialised;    /* Is Quantum library initialised? */

    /*
     * Private functions.
     */
    static void *threadWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);

}; /* SPxNetworkReceiveP430 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVP437_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
