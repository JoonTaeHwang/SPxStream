/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxOutputManager class.
 *
 * Revision Control:
 *   26/05/22 v1.19  SP 	Support slave streams.
 *
 * Previous Changes:
 *   08/06/20 1.18  SP 	Add IsPrimary().
 *   04/03/20 1.17  SP 	Major rework to use new extended heartbeat message.
 *   14/10/19 1.16  AGC	Avoid cppcheck warning.
 *   20/09/19 1.15  SP 	Remove some functions.
 *   19/09/19 1.14  SP 	Use error code instead of isAvailable flag.
 *   18/09/19 1.13  SP 	Fix calculation of local server timeout.
 *   17/09/19 1.12  SP 	Further changes to support intra-server redundancy.
 *   12/09/19 1.11  SP 	Support user-defined output conditions.
 *   11/09/19 1.10  SP 	Further support for intra-server redundancy.
 *   11/09/19 1.9   SP 	Move heartbeat sending here from SPxExtHeartbeatSender.
 *   10/09/19 1.8   SP 	Extend support for intra-server redundancy.
 *   09/09/19 1.7   SP 	Add support for output conditions.
 *   05/09/19 1.6   SP 	Support auto group ID.
 *   03/09/19 1.5   SP 	Improve reporting of output state.
 *   22/08/19 1.4   SP 	Further changes to extended heartbeat.
 *   20/08/19 1.3   SP 	Derive all classes from monitor base classes.
 *   20/08/19 1.2   SP 	Rename to SPxOutputManager.
 *   19/08/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_OUTPUT_MANAGER_H
#define _SPX_OUTPUT_MANAGER_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxThreads.h"
#include "SPxLibUtils/SPxSdbApplication.h"
#include "SPxLibNet/SPxNetAddr.h"
#include "SPxLibNet/SPxPacketSend.h"
#include "SPxLibNet/SPxPacketDecoderNet.h"

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
class SPxCriticalSection;
class SPxExtHeartbeatSender;
class SPxSdbSite;
class SPxPIM;
class SPxNetInterfaces;

/*
 * SPxOutputManager class.
 */
class SPxOutputManager : public SPxObj
{
public:
    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxOutputManager(SPxExtHeartbeatSender *extHBSender);
    virtual ~SPxOutputManager(void);
    SPxErrorCode Create(void);

    /* Local application. */
    SPxErrorCode SetGroupID(unsigned int groupID);
    unsigned int GetGroupID(void) { return m_groupID; }
    SPxErrorCode SetPriority(unsigned int priority);
    unsigned int GetPriority(void) { return m_priority; }
    double GetTimeoutSecs(void) { return m_timeoutSecs; }
    int IsPrimary(void);

    /* Primary application in the group. */
    unsigned int GetPrimaryAppID(void);
    SPxErrorCode GetPrimaryAppName(char *buf, unsigned int bufLen);
    SPxErrorCode GetPrimaryHostAddress(char *buf, unsigned int bufLen);

    /* Stream functions. */
    SPxObj *GetStreamObj(SPxNetAddr *netAddr);
    SPxErrorCode GetStreamStatusText(SPxObj *senderObj, char *buf, unsigned int bufLen);

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

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;                   /* Private data. */
    SPxCriticalSection m_mutex;             /* Mutex. */
    SPxExtHeartbeatSender *m_extHBSender;   /* Extended heartbeat sender. */
    SPxPacketDecoderNet m_extHBReceiver;    /* Extended heartbeat receiver. */
    SPxTimer m_timers;                      /* Thread timers. */
    unsigned int m_groupID;                 /* This application's group ID. */
    unsigned int m_priority;                /* This application's priority. */
    double m_timeoutSecs;                   /* This application's timeout. */
    SPxSdbSite *m_site;                     /* Parent site for detected systems. */
    SPxNetInterfaces *m_netIfs;             /* Used to monitor network interfaces. */

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode setStreamConditions(unsigned int index, const char *conditions);
    SPxErrorCode setStreamMaster(unsigned int index, int masterIndex);
    void update(void);
    void purgeDiscoveredItems(void);
    void syncHeartbeats(void);
    void updateGroup(void);
    void updateStreams(void);
    void updateMasterStream(SPxSdbStream *stream);
    void updateSlaveStream(SPxSdbStream *stream);
    SPxErrorCode getStreamError(SPxSdbStream *stream, char *reason, unsigned int bufLen);
    SPxSdbApplication *getPrimaryApp(SPxSdbStream *stream, SPxNetAddr *senderNetAddr);
    SPxSdbApplication *getSenderApp(SPxSdbStream::Sender_t *sender);
    SPxSdbStream *getSenderStream(SPxSdbStream::Sender_t *sender);
    int getAppPriority(SPxSdbApplication *app);
    void handleExtHeartbeat(const struct sockaddr_in *sender,
                            UINT32 distAddr,
                            UINT16 distPort,
                            const UINT8 *payload,
                            unsigned int numBytes);

    /*
     * Private static functions.
     */
    static void updateTimer(void *userArg);

    static void extHeartbeatHandler(SPxPacketDecoder *decoder,
                                    void *userArg,
                                    UINT32 packetType,
                                    struct sockaddr_in *sender,
                                    SPxTime_t *timestamp,
                                    const unsigned char *payload,
                                    unsigned int numBytes);

    static int checkPurgeItemItr(void *parentVoid,
                                 void *objVoid,
                                 void *itemVoid);

    static bool compareApps(SPxSdbApplication *app1, 
                            SPxSdbApplication *app2);

    static int senderObjDeletionHandler(void *invokingObj,
                                        void *userArg,
                                        void *notUsed);

}; /* SPxOutputManager */

#endif /* SPX_OUTPUT_MANAGER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
