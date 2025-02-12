/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for the remote SPx Monitor interface.
 *
 * Revision Control:
 *   18/05/22 v1.5   SP 	Throttle alarm rate.
 *
 * Previous Changes:
 *   28/04/22 1.4   SP 	Add GetMonitorAddress().
 *   28/04/22 1.3   SP 	Support alert alarm sound.
 *   27/04/22 1.2   SP 	Add AreAlertsReceived().
 *   26/04/22 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_REMOTE_MONITOR_H
#define _SPX_REMOTE_MONITOR_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibNet/SPxPacketDecoderNet.h"

/* Forward declare any classes required. */
class SPxCompress;

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
class SPxRemoteMonitor : public SPxPacketDecoderNet
{
public:

    /*
     * Public types.
     */

    /* Alert. */
    struct Alert_t
    {
        char uuid[SPX_UUID_MAX_LEN];        /* Globally unique ID. */
        UINT32 localID;                     /* ID that is local to the reporting monitor. */
        SPxSiteAlert::Code_t code;          /* Code that classifies alert. */
        SPxSiteAlert::Type_t type;          /* Type, error, warning, etc. */
        SPxSiteAlert::State_t state;        /* State, active, resolved, etc. */
        char item1UUID[SPX_UUID_MAX_LEN];   /* UUID of first item. */
        char item2UUID[SPX_UUID_MAX_LEN];   /* Optional UUID of second item. */
        char text[128];                     /* Description. */
        SPxTime_t lastUpdateTime;           /* Time of last update. */
        int isAcknowledged;                 /* Is acknowledged by operator? */
    };

    /* Active alert iterator. */
    typedef void (*AlertFn_t)(void *userArg, const Alert_t *alert);

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxRemoteMonitor(void);
    virtual ~SPxRemoteMonitor(void);

    /* Status receipt. */
    SPxErrorCode SetSenderID(unsigned int senderID);
    unsigned int GetSenderID(void) { return m_senderID; }
    int AreAlertsReceived(void);
    SPxErrorCode GetMonitorAddress(char *buf, unsigned int bufLen);

    /* Alerts. */
    SPxErrorCode AcknowledgeAlert(const char *uuid, int isAck);
    SPxErrorCode AcknowledgeAllAlerts(int isAck);
    SPxSiteAlert::Type_t GetHighestAlertType(void);
    SPxErrorCode IterateActiveAlerts(AlertFn_t fn, void *userArg);
    const Alert_t *GetAlertAndLock(const char *uuid);
    const Alert_t *GetAlertNoLock(const char *uuid);
    SPxErrorCode UnlockAlert(const Alert_t **alert);
    SPxErrorCode SetNewAlertAlarmEnabled(int isEnabled);
    int IsNewAlertAlarmEnabled(void) { return m_newAlertAlarmEnabled; }

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

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxCriticalSection m_mutex;     /* Object mutex. */
    SPxCompress *m_zlib;            /* Decompressor. */
    unsigned int m_zlibBufSize;     /* Decompress buffer size. */
    UINT8 *m_zlibBuf;               /* Decompress buffer. */
    unsigned int m_senderID;        /* Sender ID filter. */
    SPxTimer m_timers;              /* Thread timer. */
    SPxTime_t m_lastAlertMesgTime;  /* Time of last alerts message. */
    SPxTime_t m_lastAlertAlarmTime; /* Time of last alert alarm sound. */
    int m_newAlertAlarmEnabled;     /* Play sound when new alert received? */

    /*
     * Private functions.
     */

    SPxRemoteMonitor::Alert_t *getAlert(const char *uuid);
    void handleStatus(const struct sockaddr_in *sender,
                      const UINT8 *payload,
                      unsigned int numBytes);
    void handleStatus(const struct sockaddr_in *sender, 
                      const char *jsonText);
    void handleItemList(const void *jsonMsgVoid);
    void handleItemUUIDChanged(const void *jsonMsgVoid);
    void handleItemRemoved(const void *jsonMsgVoid);
    void handleAlertList(const void *jsonMsgVoid);
    void handleAlert(const void *jsonAlertVoid);
    void addNewAlert(Alert_t *alert);
    void purgeAlerts(void);

    /*
     * Private static functions.
     */

    static void statusHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *sender,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    static void purgeAlertsTimer(void *userArg);

}; /* SPxRemoteMonitor */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* _SPX_REMOTE_MONITOR_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
