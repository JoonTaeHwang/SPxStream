/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for the P395 radar control interface.
 *
 * Revision Control:
 *   14/10/19 v1.14   AGC	Suppress cppcheck warning.
 *
 * Previous Changes:
 *   12/09/19 1.13  SP 	Support track notifications.
 *   11/09/19 1.12  SP 	Changes for 11/09/19 ICD.
 *                     	Remove burn through range.
 *   28/06/19 1.11  SP 	Support multiple burn-through positions.
 *   26/06/19 1.10  SP 	Report unacknowledged commands.
 *   25/06/19 1.9   SP 	Remove logError().
 *   20/06/19 1.8   SP 	Fixes after testing with emulator.
 *   13/06/19 1.7   SP 	Add test mode.
 *   11/06/19 1.6   SP 	Rename class.
 *   07/06/19 1.5   SP 	Add new functions.
 *   05/06/19 1.4   SP 	Improvements to connection handling.
 *   03/06/19 1.3   SP 	Changes for 03/06/19 ICD.
 *   30/05/19 1.2   SP 	Further development.
 *   23/05/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_P395_CTRL_H
#define _SPX_P395_CTRL_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/* Forward declare any classes required. */
class SPxThread;
class SPxNetCat253UDP;
class SPxNetCat253;

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*
 * Define our class.
 */
class SPxP395Ctrl : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Connection state. */
    typedef enum
    {
        CONN_STATE_DISCONNECTED = 0,    /* Not connected. */
        CONN_STATE_CONNECTED    = 1,    /* Connected. */
        CONN_STATE_MONITOR      = 2,    /* Connected as status monitor. */
        CONN_STATE_CONTROLLER   = 3     /* Connected as controller. */

    } ConnectionState_t;

    /* General status values. */
    typedef enum
    {
        STATUS_UNKNOWN   = 0,
        STATUS_NORMAL    = 1,
        STATUS_WARNING   = 2,
        STATUS_DEGRADED  = 3,
        STATUS_ERROR     = 4

    } Status_t;

    /* Comms link status values. */
    typedef enum
    {
        COMMS_LINK_STATUS_UP        = 0,
        COMMS_LINK_STATUS_DOWN      = 1,
        COMMS_LINK_STATUS_FAULTY    = 2,
        COMMS_LINK_UNKNOWN          = 255   /* Not reported by radar. */

    } CommsLinkStatus_t;

    /* Scan mode. */
    typedef enum
    {
        SCAN_MODE_SURVEILLANCE      = 0,
        SCAN_MODE_STOP_AND_STARE    = 1
    
    } ScanMode_t;

    /* Operating state. */
    typedef enum
    {
        OP_STATE_STOPPED        = 0,
        OP_STATE_RUNNING        = 1,
        OP_STATE_MAINTENANCE    = 2,
        OP_STATE_TEST           = 3
    
    } OperatingState_t;

    /* Profile type. */
    typedef enum
    {
        PROFILE_TYPE_OPERATIONAL    = 0,
        PROFILE_TYPE_USER           = 1
    
    } ProfileType_t;

    /* Transmit power (percentage). */
    typedef enum
    {
        TX_POWER_25     = 0,
        TX_POWER_50     = 1,
        TX_POWER_75     = 2,
        TX_POWER_100    = 3

    } TxPower_t;

    /* Comms link info. */
    typedef struct
    {
        UINT32 time;                /* Time last updated. */
        UINT16 linkID;              /* Unique ID. */
        CommsLinkStatus_t status;   /* Status. */

    } CommsLink_t;

    /* Comms link iterator. */
    typedef void (*CommsLinkFn_t)(SPxP395Ctrl *obj,
                                  void *userArg,
                                  const CommsLink_t *commsLink);

    /* Module alarm info. */
    typedef struct
    {
        UINT32 time;            /* Time last updated. */
        UINT16 moduleID;        /* Unique module ID. */
        UINT16 alarmID;         /* Unique alarm ID for module. */
        Status_t status;        /* Status. */
        char text[256];         /* Alarm description */
    
    } ModuleAlarm_t;

    /* Module alarm iterator callback. */
    typedef void (*ModuleAlarmFn_t)(SPxP395Ctrl *obj,
                                    void *userArg,
                                    const ModuleAlarm_t *moduleAlarm);

    /* LRU info. */
    typedef struct
    {
        UINT32 time;        /* Time last updated. */
        UINT16 lruID;       /* Unique ID. */
        Status_t status;    /* Status. */

    } LRU_t;

    /* LRU iterator callback. */
    typedef void (*LRUFn_t)(SPxP395Ctrl *obj,
                            void *userArg,
                            const LRU_t *lru);

    /* Track notification info. */
    typedef struct
    {
        UINT32 time;            /* Time last updated. */
        UINT32 trackID;         /* Unique track ID. */
        UINT16 notificationID;  /* Unique notification ID. */
        UINT8 active;           /* Activity status. */
        char text[256];         /* Notification description */
    
    } TrackNotification_t;

    /* Track notification iterator callback. */
    typedef void (*TrackNotificationFn_t)(SPxP395Ctrl *obj,
                                          void *userArg,
                                          const TrackNotification_t *trackNotification);

    /* Sector info. */
    typedef struct
    {
        UINT32 time;        /* Time last updated. */
        UINT8 sectorID;     /* Unique ID. */
        int isEnabled;      /* TRUE or FALSE. */
        REAL32 startDegs;   /* Start angle in degrees. */
        REAL32 stopDegs;    /* Stop angle in degrees. */
    
    } Sector_t;

    /* Sector iterator callback. */
    typedef void (*SectorFn_t)(SPxP395Ctrl *obj,
                               void *userArg,
                               const Sector_t *sector);

    /* Zone info. */
    typedef struct
    {
        UINT32 time;        /* Time last updated. */
        UINT8 zoneID;       /* Unique ID. */
        int isEnabled;      /* TRUE or FALSE. */
        REAL32 startDegs;   /* Start angle in degrees. */
        REAL32 stopDegs;    /* Stop angle in degrees. */
        REAL32 startMetres; /* Start range in metres. */
        REAL32 stopMetres;  /* Stop range in metres. */
    
    } Zone_t;

    /* Zone iterator callback. */
    typedef void (*ZoneFn_t)(SPxP395Ctrl *obj,
                             void *userArg,
                             const Zone_t *zone);

    /* Position info. */
    typedef struct
    {
        UINT32 time;            /* Time last updated. */
        UINT8 positionID;       /* Unique ID. */
        int isEnabled;          /* TRUE or FALSE. */
        REAL32 azimuthDegs;     /* Azimuth in degrees. */
        REAL32 elevationDegs;   /* Elevation in degrees. */
        REAL32 rangeMetres;     /* Range in metres. */
    
    } Position_t;

    /* Position iterator callback. */
    typedef void (*PositionFn_t)(SPxP395Ctrl *obj,
                                 void *userArg,
                                 const Position_t *position);

    /* Text message handler function. */
    typedef void (*MesgFn_t)(void *userArg, const char *text);

    /*
     * Public variables.
     */

    /* Max number of items. */
    static const UINT8 MAX_NUM_TX_BLANK_SECTORS       = 10;
    static const UINT8 MAX_NUM_NON_ACQ_ZONES          = 10;
    static const UINT8 MAX_NUM_GUARD_ZONES            = 10;
    static const UINT8 MAX_NUM_BURN_THRO_POSITIONS    = 10;

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP395Ctrl(void);
    virtual ~SPxP395Ctrl(void);
    virtual SPxErrorCode Create(void);

    /* Configuration. */
    virtual SPxErrorCode SetLogFilename(const char *filename);
    virtual const char *GetLogFilename(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void LogMesg(unsigned int verbosity, const char *format, ...);
    virtual SPxErrorCode SetMesgHandler(MesgFn_t fn, void *userArg);
    virtual SPxErrorCode SetPassword(const char *password);
    virtual const char *GetPassword(void);
    virtual SPxErrorCode SetTestMode(int isEnabled);
    virtual int IsTestMode(void) { return m_isTestMode; }

    /* Control state. */
    virtual SPxErrorCode RequestConnection(void);
    virtual SPxErrorCode ReleaseConnection(void);
    virtual int IsConnectionPending(void);
    virtual int IsConnected(void);
    virtual int IsMonitorPending(void);
    virtual int IsMonitor(void);
    virtual SPxErrorCode RequestControl(void);
    virtual SPxErrorCode ReleaseControl(void);
    virtual int IsControlPending(void);
    virtual int IsController(void);

    /* Commands. */
    virtual SPxErrorCode SetOperatingState(OperatingState_t opState);

    virtual SPxErrorCode SetSurveillanceScanMode(void);

    virtual SPxErrorCode SetStopAndStarePosition(REAL32 azimuthDegs,
                                                 REAL32 elevationDegs,
                                                 REAL32 rangeMetres);

    virtual SPxErrorCode SetTxPower(TxPower_t power);

    virtual SPxErrorCode SetAntennaSpeed(REAL32 degsPerSec);

    virtual SPxErrorCode SetBasebandFreq(REAL32 hz);

    virtual SPxErrorCode SetRange(REAL32 metres);

    virtual SPxErrorCode SetTransmitEnabled(int isEnabled);

    virtual SPxErrorCode SetFreqHoppingEnabled(int isEnabled);

    virtual SPxErrorCode SetRadarTrackEnabled(int isEnabled);

    virtual SPxErrorCode SetIFFTrackEnabled(int isEnabled);

    virtual SPxErrorCode SetTrackCorrelationEnabled(int isEnabled);

    virtual SPxErrorCode SetAcceptCuedTrackEnabled(int isEnabled);

    virtual SPxErrorCode SetRainModeEnabled(int isEnabled);

    virtual SPxErrorCode SetProfile(ProfileType_t profileType,
                                    UINT16 profileID);

    virtual SPxErrorCode SetTxBlankSector(UINT8 sectorID,
                                          int isEnabled,
                                          REAL32 startDegs,
                                          REAL32 stopDegs);

    virtual SPxErrorCode SetNonAcqZone(UINT8 zoneID,
                                       int isEnabled,
                                       REAL32 startDegs,
                                       REAL32 stopDegs,
                                       REAL32 startMetres,
                                       REAL32 stopMetres);

    virtual SPxErrorCode SetGuardZone(UINT8 zoneID,
                                      int isEnabled,
                                      REAL32 startDegs,
                                      REAL32 stopDegs,
                                      REAL32 startMetres,
                                      REAL32 stopMetres);

    virtual SPxErrorCode SetBurnThroPosition(UINT8 positionID,
                                             int isEnabled,
                                             REAL32 azimuthDegs,
                                             REAL32 elevationDegs);

    virtual SPxErrorCode LoadTerrainMap(const char *filename);

    virtual SPxErrorCode DropTrack(UINT32 trackID);

    virtual SPxErrorCode RestoreDefaults(void);

    /* Status. */
    virtual SPxErrorCode GetHeartbeat(UINT16 *heartbeatRtn,
                                      UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetOperatingState(OperatingState_t *opStateRtn,
                                           UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetScanMode(ScanMode_t *scanModeRtn,
                                     UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetStopAndStarePosition(REAL32 *azimuthDegsRtn,
                                                 REAL32 *elevationDegsRtn,
                                                 REAL32 *rangeMetresRtn,
                                                 UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetTxPower(TxPower_t *lpiRtn,
                                    REAL32 *dbmRtn,
                                    UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetAntennaAzimuth(REAL32 *degsRtn,
                                           UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetAntennaSpeed(REAL32 *degsPerSecRtn,
                                         UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetBasebandFreq(REAL32 *hzRtn,
                                         UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetRange(REAL32 *metresRtn,
                                  UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetRadarStatus(int *isTxEnabledRtn,
                                        int *isFreqHoppingEnabledRtn,
                                        int *isRadarTrackEnabledRtn,
                                        int *isIFFTrackEnabledRtn,
                                        int *isTrackCorrelationRtn,
                                        int *isAcceptCuedTrackEnabledRtn,
                                        int *isRainModeEnabledRtn,
                                        UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetProfile(ProfileType_t *profileTypeRtn,
                                    UINT16 *profileIDRtn,
                                    UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetTxBlankSector(UINT8 sectorID,
                                          int *isEnabledRtn,
                                          REAL32 *startDegsRtn,
                                          REAL32 *stopDegsRtn,
                                          UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetNonAcqZone(UINT8 zoneID,
                                       int *isEnabledRtn,
                                       REAL32 *startDegsRtn,
                                       REAL32 *stopDegsRtn,
                                       REAL32 *startMetresRtn,
                                       REAL32 *stopMetresRtn,
                                       UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetGuardZone(UINT8 zoneID,
                                      int *isEnabledRtn,
                                      REAL32 *startDegsRtn,
                                      REAL32 *stopDegsRtn,
                                      REAL32 *startMetresRtn,
                                      REAL32 *stopMetresRtn,
                                      UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetBurnThroPosition(UINT8 positionID,
                                             int *isEnabledRtn,
                                             REAL32 *azimuthDegsRtn,
                                             REAL32 *elevationDegsRtn,
                                             UINT32 expiryMsecs=0);

    /* Status. */
    virtual CommsLinkStatus_t GetCommsLinkStatus(UINT32 expiryMsecs=0);
    virtual Status_t GetLRUStatus(UINT32 expiryMsecs=0);
    virtual Status_t GetModuleAlarmStatus(UINT32 expiryMsecs=0);
    virtual SPxStatus_t GetOverallStatus(UINT32 expiryMsecs=0);

    /* Status item iterators. */
    virtual SPxErrorCode IterateCommsLinks(CommsLinkFn_t fn,
                                           void *userArg,
                                           UINT32 expiryMsecs=0);

    virtual SPxErrorCode IterateModuleAlarms(ModuleAlarmFn_t fn,
                                             void *userArg,
                                             UINT32 expiryMsecs=0);

    virtual SPxErrorCode IterateLRUs(LRUFn_t fn,
                                     void *userArg,
                                     UINT32 expiryMsecs=0);

    virtual SPxErrorCode IterateTrackNotifications(TrackNotificationFn_t fn,
                                                   void *userArg,
                                                   UINT32 expiryMsecs=0);

    virtual SPxErrorCode IterateTxBlankSectors(SectorFn_t fn,
                                               void *userArg,
                                               UINT32 expiryMsecs=0);

    virtual SPxErrorCode IterateNonAcqZones(ZoneFn_t fn,
                                            void *userArg,
                                            UINT32 expiryMsecs=0);

    virtual SPxErrorCode IterateGuardZones(ZoneFn_t fn,
                                           void *userArg,
                                           UINT32 expiryMsecs=0);

    virtual SPxErrorCode IterateBurnThroPositions(PositionFn_t fn,
                                                  void *userArg,
                                                  UINT32 expiryMsecs=0);

    /* Debugging. */
    virtual SPxErrorCode SetLogFile(FILE *file);
    virtual FILE *GetLogFile(void) { return m_logFile; }
    virtual SPxErrorCode SetVerbosity(unsigned int level);
    virtual unsigned int GetVerbosity(void) { return m_verbosity; }

    /* Parameter control. */
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

    /*
     * Public static functions.
     */

    static SPxStatus_t CommsLinkStatusToSPxStatus(CommsLinkStatus_t clStatus);
    static SPxStatus_t StatusToSPxStatus(Status_t status);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    typedef struct
    {
        UINT16 commandID;       /* Command ID (CMD_xxx) */
        UINT16 sequenceID;      /* Cat-253 sequence ID. */
        UINT32 time;            /* Time command sent. */
    
    } Command_t;

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxCriticalSection m_mutex;         /* Object mutex. */
    SPxCriticalSection m_mesgLogMutex;  /* Message logging mutex. */
    FILE *m_logFile;                    /* Log file. */
    unsigned int m_verbosity;           /* Debug verbosity. */
    SPxThread *m_thread;                /* Background thread. */
    UINT8 m_crcTable[256];              /* CRC8 lookup table. */
    UINT8 m_dataSendBuf[512];           /* Buffer to build data in for sending. */
    UINT8 m_appDataSendBuf[512];        /* Buffer to build app data in for sending. */
    SPxNetCat253UDP *m_cat253;          /* Cat-253 command message interface. */
    ConnectionState_t m_reqConnState;   /* Request connection state. */
    UINT32 m_lastReqConnStateTime;      /* Time requested connection state changed. */
    ConnectionState_t m_connState;      /* Actual connection state. */
    UINT32 m_lastConnStateTime;         /* Time connection state changed. */
    UINT32 m_lastSentMesgTime;          /* Time last message sent. */
    UINT32 m_lastSentConnMesgTime;      /* Time last connection message sent. */
    UINT32 m_lastSentAliveMesgTime;     /* Time last alive message sent. */
    UINT16 m_lastRecvSequenceID;        /* Sequence ID of last message received. */
    UINT32 m_lastRecvMesgTime;          /* Time last message received. */
    UINT32 m_lastRecvAliveMesgTime;     /* Time last alive message received. */
    MesgFn_t m_mesgFn;                  /* Message handler function. */
    void *m_mesgFnArg;                  /* Message handler function argument. */
    int m_isTestMode;                   /* Is interface configured for test mode? */
    UINT32 m_lastTestModeFastUpdateTime;/* Last test mode fast update time. */
    UINT32 m_lastTestModeSlowUpdateTime;/* Last test mode slow update time. */

    /* Status info (update resetStatus() when adding new items). */
    UINT16 m_heartbeat;                 /* Heartbeat value. */
    UINT32 m_heartbeatTime;             /* Time heartbeat updated. */
    REAL32 m_antennaAzimuthDegs;        /* Antenna position in degrees. */
    UINT32 m_antennaAzimuthTime;        /* Time antenna position updated. */
    ScanMode_t m_scanMode;              /* Scan mode. */
    UINT32 m_scanModeTime;              /* Time scan mode updated. */
    OperatingState_t m_opState;         /* Operating state. */
    UINT32 m_opStateTime;               /* Time operating state updated. */
    REAL32 m_txPowerDbm;                /* TX power in dBm. */
    TxPower_t m_txPowerLpi;             /* Tx power as an enum. */
    UINT32 m_txPowerTime;               /* Time TX power updated. */
    REAL32 m_basebandFreqHz;            /* Baseband frequency in Hz. */
    UINT32 m_basebandFreqTime;          /* Time baseband frequency was updated. */
    REAL32 m_antennaSpeedDegsPerSec;    /* Antenna speed in degrees per second. */
    UINT32 m_antennaSpeedTime;          /* Time antenna speed was updated. */
    REAL32 m_rangeMetres;               /* Radar range in metres. */
    UINT32 m_rangeTime;                 /* Time radar range was updated. */
    int m_isTxEnabled;                  /* Is TX enabled? */
    int m_isFreqHoppingEnabled;         /* Is frequency hopping enabled? */
    int m_isRadarTrackEnabled;          /* Is radar track suppression enabled? */
    int m_isIFFTrackEnabled;            /* Is IFF track suppression enabled? */
    int m_isTrackCorrelationEnabled;    /* Is correlation of radar and IFF tracks enabled? */
    int m_isAcceptCuedTrackEnabled;     /* Is acceptance of cued tracks enabled? */
    int m_isRainModeEnabled;            /* Is rain mode enabled? */
    UINT32 m_radarStatusTime;           /* Time the above Boolean states were updated. */
    ProfileType_t m_profileType;        /* Current profile type. */
    UINT16 m_profileID;                 /* Current profile ID. */
    UINT32 m_profileTime;               /* Time profile was updated. */
    Position_t m_stopAndStarePos;       /* Stop and stare position. */

    /*
     * Private functions.
     */

    void resetStatus(void);

    SPxErrorCode sendMesg(UINT8 mti, 
                          UINT8 *appData=NULL, 
                          unsigned int appDataLen=0,
                          UINT16 *sequenceIDRtn=NULL);

    SPxErrorCode sendCommand(UINT16 commandID,
                             UINT8 *data=NULL, 
                             unsigned int dataLen=0);

    UINT32 doBackgroundProcessing(void);

    void manageConnectionState(void);

    void manageTestMode(void);

    void setReqConnectionState(ConnectionState_t newState);
    void setConnectionState(ConnectionState_t newState);

    void sendAliveMessage(void);

    void checkCommandTimeouts(void);

    void handleMesg(UINT8 mti, 
                    const UINT8 *appData,
                    unsigned int appDataLen);

    void handleConnectResponseMesg(void);

    void handleConnectReleaseMesg(void);

    void handleCommandTokenAssignMesg(const UINT8 *appData,
                                      unsigned int appDataLen);

    void handleCommandTokenReleaseMesg(void);

    void handleAcknowledgeMesg(const UINT8 *appData,
                                unsigned int appDataLen);

    void handleAliveMesg(void);

    void handleStatusMesg(const UINT8 *appData,
                          unsigned int appDataLen);

    void handleStatusData(UINT16 statusID, 
                          const UINT8 *data, 
                          UINT16 dataLen);

    void handleStatusHeartbeat(const UINT8 *data, 
                               UINT16 dataLen);

    void handleStatusCommsLink(const UINT8 *data, 
                               UINT16 dataLen);

    void handleStatusCBITE(const UINT8 *data, 
                           UINT16 dataLen);

    void handleStatusLRU(const UINT8 *data, 
                          UINT16 dataLen);

    void handleStatusTrackNotification(const UINT8 *data, 
                                       UINT16 dataLen);

    void handleAntennaAzimuth(const UINT8 *data, 
                               UINT16 dataLen);

    void handleStatusINS(const UINT8 *data, 
                         UINT16 dataLen);

    void handleStatusScanMode(const UINT8 *data, 
                              UINT16 dataLen);

    void handleStatusOperatingState(const UINT8 *data, 
                                    UINT16 dataLen);

    void handleStatusTxPower(const UINT8 *data, 
                             UINT16 dataLen);

    void handleStatusTxBlankSectors(const UINT8 *data, 
                                    UINT16 dataLen);

    void handleStatusNonAcqZones(const UINT8 *data, 
                                 UINT16 dataLen);

    void handleStatusGuardZones(const UINT8 *data, 
                                UINT16 dataLen);

    void handleStatusBurnThroPosition(const UINT8 *data, 
                                      UINT16 dataLen);

    void handleStatusBasebandFreq(const UINT8 *data, 
                                  UINT16 dataLen);

    void handleStatusAntennaSpeed(const UINT8 *data, 
                                  UINT16 dataLen);

    void handleStatusRange(const UINT8 *data, 
                           UINT16 dataLen);

    void handleStatusRadar(const UINT8 *data, 
                           UINT16 dataLen);

    void handleStatusProfile(const UINT8 *data, 
                             UINT16 dataLen);

    void handleStatusStopAndStarePosition(const UINT8 *data, 
                                          UINT16 dataLen);

    SPxErrorCode checkStatus(UINT32 time, UINT32 expiryMsecs);

    int isDataLengthValid(const char *functionName,
                          unsigned int len,
                          unsigned int unitLen, 
                          unsigned int maxRepeat);

    void initCRC8(void);
    UINT8 calcCRC8(const UINT8 *data, unsigned int len);
    UINT8 reflect8(UINT8 val);

    REAL32 be32ToReal32(UINT32 val32_be);
    UINT32 real32toBE32(REAL32 val);

    /*
     * Private static functions.
     */

    static void *backgroundThreadHandler(SPxThread *thread);

    static void mesgHandler(SPxNetCat253 *cat253,
                            void *arg,
                            const UINT8 *appData,
                            unsigned int appDataLen);

    static bool compareCommsLinks(CommsLink_t item1, CommsLink_t item2);

    static bool compareModuleAlarms(ModuleAlarm_t item1, 
                                    ModuleAlarm_t item2);

    static bool compareLRUs(LRU_t item1, LRU_t item2);

    static bool compareTrackNotifications(TrackNotification_t item1, 
                                          TrackNotification_t item2);

    static bool compareSectors(Sector_t item1, Sector_t item2);

    static bool compareZones(Zone_t item1, Zone_t item2);

    static bool comparePositions(Position_t item1, Position_t item2);

}; /* SPxP395Ctrl */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* _SPX_P395_CTRL_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
