/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Site monitoring object, used to update the site database.
 *
 * Revision Control:
 *   22/06/22 v1.65  SP 	Support remote control of application auto start.
 *
 * Previous Changes:
 *   19/05/22 1.64  SP 	Add parameter for max time sync difference.
 *   22/04/22 1.63  SP 	Add alert reporting scope.
 *   21/04/22 1.62  SP 	Add status sender ID.
 *   20/04/22 1.61  SP 	Add writeStatusHeader().
 *   13/04/22 1.60  SP 	Remove reporting of duplicate alerts.
 *   09/04/22 1.59  SP 	Include SPxPacketJsonData in status messages.
 *   14/03/22 1.58  SP 	Add more remote control commands.
 *   02/02/22 1.57  SP 	Use separate ZLIB object for each status stream.
 *   01/02/22 1.56  SP 	Support multiple status receivers.
 *   27/05/21 1.55  AGC	Make handleHeartbeat functions protected for testing.
 *   24/05/21 1.54  SP 	Add combineItem().
 *   21/12/20 1.53  SP 	Add web port clash warning.
 *   09/12/20 1.52  SP 	Fix segfault when alert is generated 
 *                     	that references a purged item.
 *   09/10/20 1.51  SP 	Add system time not synced warning.
 *   07/09/20 1.50  SP 	Support reset of application health bits.
 *   19/03/20 1.49  SP 	Add server priority clash alert.
 *   13/03/20 1.48  SP 	Support status alert list.
 *   11/03/20 1.47  SP 	Add public functions for status TCP/UDP control.
 *   11/03/20 1.46  SP 	Support system restart.
 *   11/03/20 1.45  SP 	Various bug fixes and performance improvements.
 *   04/03/20 1.44  SP 	Use SPxNetInterfaces.
 *   24/02/20 1.43  SP 	Improve reporting for TCP connections.
 *   20/02/20 1.42  SP 	Support status over a TCP connection.
 *   19/02/20 1.41  SP 	Use process ID to aid stream assignment.
 *   17/02/20 1.40  SP 	Further improvements to stream assignment.
 *   14/02/20 1.39  SP 	Use port list to improve stream assignment.
 *   13/02/20 1.38  SP 	Rework application start/stop.
 *   12/02/20 1.37  SP 	Remove deprecated remote control features.
 *   12/02/20 1.36  SP 	Improve operation on Linux.
 *   06/02/20 1.35  SP 	Various changes and bug fixes.
 *   05/02/20 1.34  SP 	Various bug fixes.
 *   05/02/20 1.33  SP 	Rename alert.
 *   04/02/20 1.32  SP 	Link to extended heartbeat sender.
 *   04/02/20 1.31  SP 	Support remote sitemap save.
 *   03/02/20 1.30  SP 	Support auto start.
 *   30/01/20 1.29  SP 	Support status event messages.
 *   30/01/20 1.28  SP 	Support control of remote applications.
 *   29/01/20 1.27  SP 	Support control of remote monitors.
 *   29/01/20 1.26  SP 	Keep UUIDs consistent across all monitors.
 *   28/01/20 1.25  SP 	Support discovery, aggregation and reporting scope.
 *   16/01/20 1.24  SP 	Add more local update functions.
 *   09/12/19 1.23  SP 	Add memory load alert.
 *   08/01/20 1.22  SP 	Add application source status alerts.
 *   08/01/20 1.21  SP 	Add application health status alerts.
 *   07/01/20 1.20  SP 	Support compressed JSON messages.
 *   10/12/19 1.19  SP 	Support site lat/long.
 *   09/12/19 1.18  SP 	Add stream address clash alert.
 *   05/12/19 1.17  SP 	Add stream detection alerts.
 *   05/12/19 1.16  SP 	Add high CPU and net load alerts.
 *   04/12/19 1.15  SP 	Add command port clash alert.
 *   21/11/19 1.14  SP 	Support alerts.
 *   19/11/19 1.13  SP 	Remove support for static fields.
 *   18/11/19 1.12  SP 	Various improvements to stream detection.
 *   18/11/19 1.11  SP 	Add heartbeats as detected streams.
 *   15/11/19 1.10  SP 	Improve stream listening.
 *   15/11/19 1.9   SP 	Remove stand-alone mode.
 *   14/11/19 1.8   SP 	Support stream listening.
 *   08/11/19 1.7   SP 	Move field keys to SPxSdbFields.
 *   07/11/19 1.6   SP 	Support extended heartbeats.
 *   06/11/19 1.5   SP 	Add heartbeat handling.
 *   04/11/19 1.4   SP 	Add more system information.
 *   31/10/19 1.3   SP 	Further development.
 *   30/10/19 1.2   SP 	Further development.
 *   29/10/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SITE_MONITOR_H
#define _SPX_SITE_MONITOR_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxSdbItem.h"
#include "SPxLibUtils/SPxSdbApplication.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxViewControl.h" /* For SPxLatLong_t */
#include "SPxLibNet/SPxPacketSendJSON.h"

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

/* Forward declare classes we use. */
class SPxSiteDatabase;
class SPxSiteNotifier;
class SPxRemoteServer;
class SPxExtHeartbeatSender;
class SPxNetInterfaces;
class SPxPacketDecoderNet;
class SPxCompress;

/*
 *  SPxSiteMonitor class.
 */
class SPxSiteMonitor : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /* Maximum number of status receivers. */
    static const unsigned int MAX_NUM_STATUS_RECEIVERS = 20;


    /* Maximum number of stream listeners. */
    static const unsigned int MAX_NUM_STREAM_LISTENERS = 20;

    /*
     * Public types.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSiteMonitor(SPxSiteDatabase *database, 
                   SPxSiteNotifier *notifier);
    virtual ~SPxSiteMonitor(void);

    /* Setup. */
    SPxErrorCode SetExtHeartbeatSender(SPxExtHeartbeatSender *extHbSender);
    const SPxSdbItem::MonitorScope_t *GetScope(void) { return &m_scope; }
    SPxErrorCode SetDiscoveryScope(SPxSdbItem::Scope_t scope);
    SPxSdbItem::Scope_t GetDiscoveryScope(void) { return m_scope.discovery; }
    SPxErrorCode SetAggregationScope(SPxSdbItem::Scope_t scope);
    SPxSdbItem::Scope_t GetAggregationScope(void) { return m_scope.aggregation; }
    SPxErrorCode SetItemReportingScope(SPxSdbItem::Scope_t scope);
    SPxSdbItem::Scope_t GetItemReportingScope(void) { return m_scope.itemReporting; }
    SPxErrorCode SetAlertReportingScope(SPxSdbItem::Scope_t scope);
    SPxSdbItem::Scope_t GetAlertReportingScope(void) { return m_scope.alertReporting; }
    SPxErrorCode StartMonitoring(void);
    SPxErrorCode StopMonitoring(void);
    int IsMonitoring(void) { return m_isMonitoring; }
    SPxErrorCode LoadSitemap(const char *filename);
    SPxErrorCode SaveSitemap(const char *filename);

    /* Network port utility functions. */
    UINT32 GetPortOwnerProcessID(UINT16 port);

    /* Local site. */
    SPxErrorCode SetLocalSiteName(const char *name);
    SPxErrorCode GetLocalSiteName(char *buf, unsigned int bufLen);
    SPxErrorCode SetLocalSiteLatLong(const SPxLatLong_t *ll);
    SPxErrorCode GetLocalSiteLatLong(SPxLatLong_t *ll);

    /* Local system. */
    SPxErrorCode SetLocalSystemName(const char *name);
    SPxErrorCode GetLocalSystemName(char *buf, unsigned int bufLen);
    SPxErrorCode SetLocalSystemLatLong(const SPxLatLong_t *ll);
    SPxErrorCode GetLocalSystemLatLong(SPxLatLong_t *ll);

    /* Status sender. */
    SPxErrorCode SetStatusSendTCP(int isTCP);
    int IsStatusSendTCP(void);
    SPxErrorCode SetStatusSendAddress(const char *address, int port, const char *ifAddr);
    const char *GetStatusSendAddress(void);
    SPxErrorCode SetStatusSendPort(int port);
    const char *GetStatusSendIfAddress(void);
    int GetStatusSendPort(void);
    SPxErrorCode SetStatusSendID(unsigned int senderID);
    unsigned int GetStatusSendID(void) { return m_statusSendID; }

    /* Status receivers.*/
    SPxErrorCode SetStatusReceiver(unsigned int index, const char *group);
    const char *GetStatusReceiver(unsigned int index);

    /* Heartbeat messages. */
    SPxErrorCode SetHeartbeatRecvAddress(const char *address, int port, const char *ifAddr);
    const char *GetHeartbeatRecvAddress(void);
    SPxErrorCode SetHeartbeatRecvPort(int port);
    const char *GetHeartbeatRecvIfAddress(void);
    int GetHeartbeatRecvPort(void);

    /* Extended heartbeat messages. */
    SPxErrorCode SetExtHeartbeatRecvAddress(const char *address, int port, const char *ifAddr);
    const char *GetExtHeartbeatRecvAddress(void);
    SPxErrorCode SetExtHeartbeatRecvPort(int port);
    const char *GetExtHeartbeatRecvIfAddress(void);
    int GetExtHeartbeatRecvPort(void);

    /* Stream listeners. */
    SPxErrorCode SetStreamListener(unsigned int index, const char *group);
    const char *GetStreamListener(unsigned int index);

    /* Alert thresholds. */
    SPxErrorCode SetMaxCPULoadThreshold(double percent);
    double GetMaxCPULoadThreshold(void) { return m_maxCPULoadThreshold; }
    SPxErrorCode SetMaxNetLoadThreshold(double percent);
    double GetMaxNetLoadThreshold(void) { return m_maxNetLoadThreshold; }
    SPxErrorCode SetMaxMemLoadThreshold(double percent);
    double GetMaxMemLoadThreshold(void) { return m_maxMemLoadThreshold; }
    SPxErrorCode SetMaxTimeDiffSecsThreshold(UINT32 secs);
    UINT32 GetMaxTimeDiffSecsThreshold(void) { return m_maxTimeDiffSecsThreshold; }

    /* System and application control. */
    SPxErrorCode SetPassword(SPxSdbSystem *system, const char *password);
    SPxErrorCode CheckControl(SPxSdbSystem *system);
    int IsPasswordSet(SPxSdbSystem *system);
    SPxErrorCode StartApplication(SPxSdbApplication *app);
    int CanStartApplication(SPxSdbApplication *app);
    SPxErrorCode StopApplication(SPxSdbApplication *app);
    int CanStopApplication(SPxSdbApplication *app);
    SPxErrorCode SetAutoStartApplication(SPxSdbApplication *app, int isEnabled);
    int CanSetAutoStartApplication(SPxSdbApplication *app, int isEnabled);
    SPxErrorCode ResetApplicationHealth(SPxSdbApplication *app);
    int CanResetApplicationHealth(SPxSdbApplication *app);
    SPxErrorCode StartProcess(SPxSdbSystem *system, 
                              const char *imageAndArgs,
                              int checkOnly=FALSE);
    int CanStartProcess(SPxSdbSystem *system, 
                        const char *imageAndArgs);
    SPxErrorCode RestartSystem(SPxSdbSystem *system);

    /*
     * Public static functions.
     */

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */
    /* Heartbeat receive. */
    void handleHeartbeat(const struct sockaddr_in *sender,
                         UINT32 distAddr,
                         UINT16 distPort,
                         const UINT8 *payload,
                         unsigned int numBytes);

    void handleHeartbeat(UINT32 senderAddr,
                         UINT16 senderPort,
                         UINT32 distAddr,
                         INT16 distPort,
                         SPxScNetHeartbeatStruct *hb);

    void handleExtHeartbeat(const struct sockaddr_in *sender,
                            UINT32 distAddr,
                            UINT16 distPort,
                            const UINT8 *payload,
                            unsigned int numBytes);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /* General. */

private:

    /*
     * Private types.
     */

    /* Status receiver context. */
    struct StatusReceiver_t
    {
        SPxSiteMonitor *obj;                /* Pointer back to object. */
        char group[128];                    /* Address, port, etc. */
        SPxPacketDecoderNet *receiver;      /* Packet receiver. */
        SPxCompress *zlib;                  /* ZLIB deccompressor. */
        UINT8 *zlibBuf;                     /* Decompression buffer. */
        unsigned int zlibBufSize;           /* Decompression buffer size. */
        int isConnected;                    /* Is there a TCP connection? */
    };

    /* Stream listener context. */
    struct StreamListener_t
    {
        char group[128];                    /* Address, port, etc. */
        SPxPacketDecoderNet *receiver;      /* Packet receiver. */
    };

    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;                   /* Private data. */
    SPxSiteDatabase *m_database;            /* Site database. */
    SPxSiteNotifier *m_notifier;            /* Site notifier. */
    SPxTimer m_timers;                      /* Thread timer. */
    int m_isMonitoring;                     /* Threads running? */
    SPxSdbItem::MonitorScope_t m_scope;     /* Item scope. */
    int m_processDiscoveryEnabled;          /* Local process discovery enabled? */
    SPxCriticalSection m_mutex;             /* Mutex used to protect internal lists. */
    SPxLatLong_t m_localSiteLL;             /* Local site lat/long. */
    SPxLatLong_t m_localSystemLL;           /* Local system lat/long. */
    int m_isSaveSitemapInProgress;          /* Is sitemap save in progress? */
    SPxExtHeartbeatSender *m_extHeartbeatSender;   /* Application's heartbeat sender. */

    /* Network interfaces. */
    SPxNetInterfaces *m_netIfs;             /* Used to get info in network interface. */

    /* Status sender. */
    SPxPacketSenderJSON m_statusSender; /* JSON status sender. */
    UINT32 m_statusSendClientAddr;      /* TCP connected client address. */
    unsigned int m_statusSendID;        /* Sender ID to include in messages. */

    /* Status receivers. */
    StatusReceiver_t m_statusReceivers[MAX_NUM_STATUS_RECEIVERS];

    /* Stream listeners. */
    StreamListener_t m_streamListeners[MAX_NUM_STREAM_LISTENERS];

    /* Heartbeat receiver. */
    SPxPacketDecoderNet m_heartbeatReceiver;
    SPxPacketDecoderNet m_extHeartbeatReceiver;

    /* Alert thresholds. */
    double m_maxCPULoadThreshold;
    double m_maxNetLoadThreshold;
    double m_maxMemLoadThreshold;
    UINT32 m_maxTimeDiffSecsThreshold;

    /* Remote monitor control. */
    SPxRemoteServer *m_remote;

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode getLocalAndLock(SPxSdbSite **siteRtn, SPxSdbSystem **systemRtn);
    void updateNetPortList(void);
    void appendNetPort(UINT16 port, UINT32 processID);
    SPxErrorCode getNetLoad(int isInput, double *loadRtn);
    SPxErrorCode getMemLoad(double *loadRtn);
    SPxErrorCode parseGroup(const char *group,
                             int *isTCP,
                             char *addr,
                             unsigned int addrLen,
                             int *port,
                             char *ifAddr,
                             unsigned int ifAddrLen);

    /* Database checking. */
    void checkDatabase(void);
    void checkItem(SPxSdbItem *item);
    void checkSite(SPxSdbSite *site);
    void checkSystem(SPxSdbSystem *system);
    void checkApplication(SPxSdbApplication *app);
    void checkStream(SPxSdbStream *stream);
    void purgeItem(SPxSdbItem *item);
    void combineItem(SPxSdbItem *item);
    void combineApplication(SPxSdbApplication *app);
    void combineStream(SPxSdbStream *stream);

    /* Updating local items in database. */
    void updateLocal(void);
    void updateLocalSite(SPxSdbSite *site);
    void updateLocalSystem(SPxSdbSystem *system);
    void updateLocalApplication(SPxSdbApplication *application);
    void updateLocalStream(SPxSdbStream *stream);

    /* Application process discovery. */
    void discoverLocalApplications(SPxSdbSystem *system);
    void assignProcessToApp(SPxSdbSystem *system, 
                            const char *processName, 
                            UINT32 processID);

    /* Database event handling. */
    void handleItemUUIDChange(SPxSdbItem *item, const char *oldUUID);
    void handleItemRemove(SPxSdbItem *item);

    /* Status send and receive. */
    void writeStatusHeader(void *writerVoid, const char *eventName);
    void sendStatusItemList(void);
    void sendStatusItemUUIDChange(SPxSdbItem *item, const char *oldUUID);
    void sendStatusItemRemove(SPxSdbItem *item);
    void sendStatusAlertList(void);
    void sendStatus(const char *mesg);
    void handleStatus(StatusReceiver_t *sr,
                      const struct sockaddr_in *sender,
                      const UINT8 *payload,
                      unsigned int numBytes);
    void handleStatus(const char *jsonText);
    void handleStatusItemList(const void *jsonMsgVoid);
    void handleStatusItemUUIDChanged(const void *jsonMsgVoid);
    void handleStatusItemRemoved(const void *jsonMsgVoid);

    /* Stream receive. */
    void handleStream(const struct sockaddr_in *sender,
                      UINT32 distAddr,
                      UINT16 distPort,
                      const UINT8 *payload,
                      unsigned int numBytes);

    int assignStreamToSystem(SPxSdbSystem *system,
                             SPxSdbStream::Type_t streamtType,
                             SPxSdbStream::Subtype_t streamSubtype,
                             UINT16 senderPort,
                             UINT32 distAddr,
                             UINT16 distPort);

    int assignStreamToApp(SPxSdbApplication *app,
                          SPxSdbStream::Type_t streamType,
                          SPxSdbStream::Subtype_t streamSubtype,
                          UINT16 senderPort,
                          UINT32 distAddr,
                          UINT16 distPort,
                          int allowCreate);

    int assignStreamToPlaceholderApp(SPxSdbSystem *system,
                                     SPxSdbApplication::Type_t appType,
                                     SPxSdbStream::Type_t streamType,
                                     SPxSdbStream::Subtype_t streamSubtype,
                                     UINT16 senderPort,
                                     UINT32 distAddr,
                                     UINT16 distPort);

    /* Alert checking and reporting. */
    void checkWarnUnexpectedItem(SPxSdbItem *item);
    void checkWarnAppHealthStatus(SPxSdbApplication *app);
    void checkWarnSystemHighCPULoad(SPxSdbSystem *system);
    void checkWarnSystemHighMemLoad(SPxSdbSystem *system);
    void checkWarnSystemHighNetLoad(SPxSdbSystem *system);
    void checkWarnSystemTimeNotSynced(SPxSdbSystem *system);
    void checkWarnStreamDisabled(SPxSdbStream *stream);
    void checkErrExpectedItemMissing(SPxSdbItem *item);
    void checkErrAppCmdPortClash(SPxSdbSystem *system);
    void checkErrAppWebPortClash(SPxSdbSystem *system);
    void checkErrAppHealthStatus(SPxSdbApplication *app);
    void checkErrAppSourceStatus(SPxSdbApplication *app);
    void checkErrAppPriorityClash(SPxSdbApplication *app);
    void checkErrStreamPacketsNotDetected(SPxSdbStream *stream);
    void checkErrStreamAddrClash(SPxSdbStream *stream);
    int isAlertEnabled(SPxSiteAlert::Code_t code, 
                       SPxSdbItem *item1, 
                       SPxSdbItem *item2=NULL);
    void reportAlert(SPxSiteAlert::Code_t code, 
                     SPxSdbItem *item1, 
                     SPxSdbItem *item2=NULL);

    /* Control via remote monitor. */
    SPxErrorCode handleRemoteSetCommand(char *name, char *value);
    SPxErrorCode handleRemoteGetCommand(char *name, char *valueBuf, int bufLen);
    SPxSdbApplication *getRemoteMonitor(SPxSdbItem *item);
    SPxRemoteServer *openRemoteMonitor(SPxSdbApplication *monitor);
    SPxRemoteServer *openRemoteApp(SPxSdbApplication *app, const char *password);
    SPxErrorCode closeRemote(SPxRemoteServer *remote);
    SPxErrorCode runRemoteCommand(SPxSdbApplication *monitor,
                                  SPxSdbItem *item,
                                  const char *param,
                                  const char *arg);
    SPxErrorCode runRemoteCommand(SPxSdbItem *item,
                                  const char *param,
                                  const char *arg=NULL);

    /* Misc. */
    UINT32 getEpochFromISO8601(const char *timeStr);

    /*
     * Private static functions.
     */

    /* Timer callbacks. */
    static void checkDatabaseTimer(void *userArg);
    static void updateLocalTimer(void *userArg);
    static void sendStatusTimer(void *userArg);
    static void netPortListUpdateTimer(void *userArg);

    /* Database iterators. */
    static int checkItemItr(void *parentVoid,
                            void *objVoid,
                            void *itemVoid);

    static int purgeItemItr(void *parentVoid,
                            void *objVoid,
                            void *itemVoid);

    static int combineItemItr(void *parentVoid,
                              void *objVoid,
                              void *itemVoid);

    /* Packet receive handlers. */
    static void statusHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *sender,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    static void heartbeatHandler(SPxPacketDecoder *decoder,
                                 void *userArg,
                                 UINT32 packetType,
                                 struct sockaddr_in *sender,
                                 SPxTime_t *timestamp,
                                 const unsigned char *payload,
                                 unsigned int numBytes);

    static void extHeartbeatHandler(SPxPacketDecoder *decoder,
                                    void *userArg,
                                    UINT32 packetType,
                                    struct sockaddr_in *sender,
                                    SPxTime_t *timestamp,
                                    const unsigned char *payload,
                                    unsigned int numBytes);

    static void streamHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *sender,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    /* Event callbacks. */
    static int databaseEventHandler(void *invokingObjPtr,
                                    void *userObjPtr,
                                    void *eventPtr);

}; /* SPxSiteMonitor */

#endif /* _SPX_SITE_MONITOR_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
