/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSiteNotifier class.
 *
 * Revision Control:
 *   21/04/22 v1.9   SP 	Fix typo.
 *
 * Previous Changes:
 *   21/04/22 1.8   SP 	Support alert UUID.
 *   12/11/20 1.7   SP 	Share mutex with site database.
 *   23/04/20 1.6   SP 	Improvements to AddAlertsToMIB().
 *   23/04/20 1.5   SP 	Add AddAlertsToMIB().
 *   22/04/20 1.4   SP 	Add GetStatus().
 *   13/03/20 1.3   SP 	Support JSON alert list.
 *   19/02/20 1.2   SP 	Add GetAlertsHTML().
 *   21/11/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SITE_NOTIFIER_H
#define _SPX_SITE_NOTIFIER_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxSiteAlert.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxSdbItem.h"
#include "SPxLibData/SPxMIB.h"

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
class SPxCallbackList;
class SPxSiteDatabase;

/*
 *  SPxSiteNotifier class.
 */
class SPxSiteNotifier : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public types.
     */

    /* Selection state. */
    typedef enum
    {
        STATE_SELECT,
        STATE_DESELECT,
        STATE_TOGGLE
    
    } State_t;

    /* Notification event type. */
    typedef enum
    {
        EVENT_ALERT_ADD,            /* Alert added. */
        EVENT_ALERT_REMOVE,         /* Alert removed. */
        EVENT_ALERT_SELECT,         /* Alert selected. */
        EVENT_ALERT_DESELECT,       /* Alert deselected. */
        EVENT_ALERT_STATE_CHANGE    /* Alert state has changed. */

    } EventType_t;

    /* Event structure. Note that the alert pointer is only valid 
     * inside the callback while the site notifier is locked. If 
     * the event is queued and handled outside of the callback, 
     * then the ID must be used.
     */
    typedef struct
    {
        SPxTime_t time;         /* Event time (auto filled). */
        EventType_t type;       /* Event type. */
        SPxSiteAlert *alert;    /* Alert (see note above) */
        UINT32 localID;         /* Local ID for alert (auto filled). */

    } Event_t;

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSiteNotifier(SPxSiteDatabase *database);
    virtual ~SPxSiteNotifier(void);
    SPxErrorCode Create(void);

    /* Access. */
    SPxSiteDatabase *GetDatabase(void) { return m_database; }

    /* Debugging. */
    SPxErrorCode SetLogFile(FILE *fPtr);
    FILE *GetLogFile(void) { return m_logFile; }
    SPxErrorCode SetVerbosity(unsigned int level);
    unsigned int GetVerbosity(void) { return m_verbosity; }
    void LogMesg(unsigned int verbosity, const char *format, ...);

    /* Event reporting. */
    SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode ReportEvent(Event_t *evt);
    SPxErrorCode ReportEventNoLock(Event_t *evt);

    /* Alerts. */
    SPxSiteAlert *GetAlertAndLock(UINT32 localID);
    SPxSiteAlert *GetAlertNoLock(UINT32 localID);
    SPxSiteAlert *AddOrGetAlertAndLock(const char *uuid,
                                       const SPxSiteAlert::Desc_t *desc, 
                                       int allowCreate);
    SPxSiteAlert *AddOrGetAlertNoLock(const char *uuid,
                                      const SPxSiteAlert::Desc_t *desc, 
                                      int allowCreate);
    SPxErrorCode UnlockAlert(SPxSiteAlert **alert);
    unsigned int GetAlerts(const SPxSiteAlert::Desc_t *desc,
                           SPxSiteAlert **alertsRtn[]);
    SPxErrorCode IterateAlerts(SPxCallbackListFn_t fn, 
                               void *userArg,
                               const SPxSiteAlert::Desc_t *desc);
    SPxErrorCode DeleteAllAlerts(void);
    SPxErrorCode SetSelectedAlert(UINT32 localID, State_t state);
    SPxErrorCode SetSelectedAlertNoLock(UINT32 localID, State_t state);
    SPxSiteAlert *GetSelectedAlertAndLock(void);
    SPxSiteAlert *GetSelectedAlertNoLock(void);

    /* General. */
    SPxErrorCode GetJSON(char *buf, unsigned int bufLen, 
                         SPxSdbItem::Scope_t scope,
                         SPxSiteAlert::State_t state);
    SPxErrorCode AppendJSON(void *writerVoid, 
                            SPxSdbItem::Scope_t scope,
                            SPxSiteAlert::State_t state);
    SPxErrorCode GetAlertsHTML(char *buf, unsigned int bufLen, 
                               SPxSiteAlert::State_t state);
    SPxErrorCode AddAlertsToMIB(SPxMIB *mib, const SPxMIB::OID_t *rootOID);
    SPxStatus_t GetStatus(void);

    /* Only used by SPxSiteAlert. */
    SPxErrorCode AddAlert(SPxSiteAlert *alert);
    SPxErrorCode RemoveAlert(SPxSiteAlert *alert);
    UINT32 GetNextAlertLocalID(void);

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

    /* General. */

private:

    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;               /* Private data. */
    SPxSiteDatabase *m_database;        /* Site database. */
    SPxCriticalSection m_logMutex;      /* Mutex for message logging. */
    unsigned int m_verbosity;           /* Debug verbosity. */
    FILE *m_logFile;                    /* Debug log file. */
    SPxCallbackList *m_eventCBList;     /* List of event callbacks. */
    SPxSiteAlert *m_selectedAlert;      /* Currently selected alert or NULL. */
    UINT32 m_nextAlertLocalID;          /* Next alert local ID to assign. */
    SPxTimer m_timers;                  /* Thread timer. */

    /*
     * Private functions.
     */

    SPxSiteAlert *getAlertByLocalID(UINT32 localID);
    void checkAlerts(void);

    /*
     * Private static functions.
     */

    /* Timer callbacks. */
    static void checkAlertsTimer(void *userArg);

}; /* SPxSiteNotifier */

#endif /* SPX_SITE_NOTIFIER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
