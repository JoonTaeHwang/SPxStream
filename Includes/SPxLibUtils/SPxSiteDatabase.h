/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSiteDatabase class.
 *
 * Revision Control:
 *   24/05/22 v1.29  SP 	Add all item configuration functions.
 *
 * Previous Changes:
 *   24/05/22 1.28  SP 	Rename function.
 *   07/04/22 1.27  SP 	Implement IsLocal().
 *   21/03/22 1.26  SP 	Write directly to file in GetJSON().
 *   12/11/20 1.25  SP 	Add GetMutex().
 *   16/03/20 1.24  SP 	Add GetSelectedItem().
 *   04/03/20 1.23  SP 	Remove GetNextLocalID().
 *   19/02/20 1.22  SP 	Add GetHTML().
 *   04/02/20 1.21  SP 	Support remote sitemap save.
 *   04/02/20 1.20  SP 	Support item local ID.
 *   03/02/20 1.19  SP 	Support message reporting.
 *   03/02/20 1.18  SP 	Add application start/stop events.
 *   30/01/20 1.17  SP 	Support restructured status messages.
 *   30/01/20 1.16  SP 	Support UUID change event.
 *   29/01/20 1.15  SP 	Keep UUIDs consistent across all monitors.
 *   28/01/20 1.14  SP 	Support discovery, aggregation and reporting scope.
 *   07/01/20 1.13  SP 	Support compressed JSON messages.
 *   04/12/19 1.12  SP 	Support JSON message handling in base class.
 *   19/11/19 1.11  SP 	Support saving sitemap to config.
 *   18/11/19 1.10  SP 	Mark items as expected when set from config.
 *   15/11/19 1.9   SP 	Don't make public functions virtual.
 *   08/11/19 1.8   SP 	Improve debug reporting.
 *   06/11/19 1.7   SP 	Improve operation of Compare().
 *   04/11/19 1.6   SP 	Remove background timer.
 *   31/10/19 1.5   SP 	Fix build warning.
 *   30/10/19 1.4   SP 	Further development.
 *   29/10/19 1.3   SP 	Support generation of JSON messages.
 *   25/10/19 1.2   SP 	Rename item classes and further development.
 *   24/10/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SITE_DATABASE_H
#define _SPX_SITE_DATABASE_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxSdbItem.h"
#include "SPxLibUtils/SPxSdbSite.h"
#include "SPxLibUtils/SPxTimer.h"

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
class SPxSdbSite;
class SPxSdbSystem;
class SPxSdbApplication;

/*
 *  SPxSiteDatabase class.
 */
class SPxSiteDatabase : public SPxSdbItem
{
public:
    /*
     * Public variables.
     */

    /*
     * Public types.
     */

    /* Default sitemap filename. */
    static const char *DEFAULT_SITEMAP_FILENAME;

    /* Message handler function. */
    typedef void (*MesgFn_t)(void *userArg, const char *mesg);

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
        EVENT_ITEM_ADD,                     /* Item added. */
        EVENT_ITEM_REMOVE,                  /* Item removed. */
        EVENT_ITEM_SELECT,                  /* Item selected. */
        EVENT_ITEM_DESELECT,                /* Item deselected. */
        EVENT_ITEM_UUID_CHANGE,             /* Item UUID changed. */
        EVENT_ITEM_DISPLAY_NAME_CHANGE,     /* Item display name changed. */
        EVENT_ITEM_EXPECTED_STATE_CHANGE,   /* Item set as expected or unexpected. */
        EVENT_APPLICATION_START,            /* Application started. */
        EVENT_APPLICATION_STOP              /* Application stopped. */

    } EventType_t;

    /* Event structure. Note that the item pointer is only valid 
     * inside the callback while the site database is locked. If 
     * the event is queued and handled outside of the callback, 
     * then the UUID must be used.
     */
    typedef struct
    {
        SPxTime_t time;                         /* Event time (auto filled). */
        EventType_t type;                       /* Event type. */
        SPxSdbItem *item;                       /* Item (see note above) */
        UINT32 localID;                         /* Local ID for item (auto filled). */
        char uuid[SPxSdbItem::UUID_MAX_LEN];    /* UUID for item (auto filled). */
        char oldUUID[SPxSdbItem::UUID_MAX_LEN]; /* Old UUID when changed. */

    } Event_t;

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSiteDatabase(void);
    virtual ~SPxSiteDatabase(void);
    SPxErrorCode Create(void);

    /* Required by base class but not used. */
    int IsLocal(void) { return FALSE; }

    /* Debugging. */
    SPxErrorCode SetLogFile(FILE *fPtr);
    FILE *GetLogFile(void) { return m_logFile; }
    SPxErrorCode SetVerbosity(unsigned int level);
    unsigned int GetVerbosity(void) { return m_verbosity; }
    void LogMesg(unsigned int verbosity, const char *format, ...);
    SPxErrorCode SetMesgHandler(MesgFn_t fn, void *userArg);

    /* Event reporting. */
    SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode ReportEvent(Event_t *evt);
    SPxErrorCode ReportEventNoLock(Event_t *evt);

    /* Site access. */
    SPxSdbSite *AddOrGetSiteAndLock(const char *uuid,
                                    const SPxSdbSite::Desc_t *desc, 
                                    int allowCreate);
    SPxSdbSite *AddOrGetSiteNoLock(const char *uuid,
                                   const SPxSdbSite::Desc_t *desc, 
                                   int allowCreate);
    SPxErrorCode UnlockSite(SPxSdbSite **site);
    SPxErrorCode IterateSites(SPxCallbackListFn_t fn, void *userArg);
    SPxErrorCode DeleteAllSites(void);

    /* Generic item access. */
    SPxSdbItem *GetItemAndLock(void);
    SPxSdbItem *GetItemAndLock(UINT32 localID);
    SPxSdbItem *GetItemNoLock(UINT32 localID);
    SPxSdbItem *GetItemAndLock(const char *uuid);
    SPxSdbItem *GetItemNoLock(const char *uuid);
    SPxErrorCode UnlockItem(SPxSdbItem **item);
    int IsItemInDatabase(const char *uuid);
    SPxErrorCode SetSelectedItem(UINT32 localID, State_t state);
    SPxErrorCode SetSelectedItemNoLock(UINT32 localID, State_t state);
    UINT32 GetSelectedItem(void);
    SPxSdbItem *GetSelectedItemAndLock(void);
    SPxSdbItem *GetSelectedItemNoLock(void);

    /* All item operations. */
    SPxErrorCode SetAllItemsExpected(int isExpected);
    SPxErrorCode SetAllApplicationsAutoStart(int isEnabled);

    /* General. */
    SPxErrorCode LoadSitemap(const char *filename,
                             const MonitorScope_t *scope);
    SPxErrorCode SaveSitemap(const char *filename, 
                             const MonitorScope_t *scope);
    SPxErrorCode ParseJSON(const char *jsonText,
                           const MonitorScope_t *scope,
                           EndPoint_t source);
    SPxErrorCode HandleJSON(const void *jsonDbVoid,
                            const MonitorScope_t *scope,
                            EndPoint_t source);
    SPxErrorCode WriteFormattedJSON(FILE *filePtr, 
                                    const MonitorScope_t *scope);
    SPxErrorCode AppendJSON(void *writerVoid, 
                            const MonitorScope_t *scope,
                            EndPoint_t dest);
    SPxErrorCode GetHTML(char *buf, unsigned int bufLen, int isExpected);
    SPxErrorCode ValidateUUID(const char *uuid);
    SPxErrorCode ValidateUUIDNoLock(const char *uuid);

    /* For use by companion classes. */
    SPxCriticalSection *GetMutex(void) { return &m_mutex; }

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

    /* Hide public access to these base class functions. */
    using SPxSdbItem::GetChildByIndex;
    using SPxSdbItem::GetChildByLocalID;
    using SPxSdbItem::GetChildByUUID;
    using SPxSdbItem::IterateChildren;

    /* General. */
    // cppcheck-suppress virtualCallInConstructor
    virtual void updateDisplayName(void) { /* Not used. */ }
    virtual SPxErrorCode purgeDesc(double /* timeoutSecs */) { return SPX_NO_ERROR; }

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;               /* Private data. */
    SPxCriticalSection m_mutex;         /* Main mutex for this object. */
    SPxCriticalSection m_logMutex;      /* Mutex for message logging. */
    unsigned int m_verbosity;           /* Debug verbosity. */
    FILE *m_logFile;                    /* Debug log file. */
    SPxCallbackList *m_eventCBList;     /* List of event callbacks. */
    SPxSdbItem *m_selectedItem;         /* Currently selected item or NULL. */
    MesgFn_t m_mesgFn;                  /* Message handler. */
    void *m_mesgFnArg;                  /* Message handler argument. */
    UINT32 m_nextLocalID;               /* Next local ID to assign. */
    int m_isAppAutoStartEnabled;        /* Used by SetAllApplicationsAutoStart(). */

    /*
     * Private functions.
     */

    /* General. */
    SPxSdbItem *getItemByLocalID(SPxSdbItem *item, UINT32 localID);
    SPxSdbItem *getItemByUUID(SPxSdbItem *item, const char *uuid);
    SPxErrorCode addOrUpdateSite(const void *jsonSiteVoid,
                                 const MonitorScope_t *scope,
                                 EndPoint_t source);

    /*
     * Private static functions.
     */

    /* Database iterators. */
    static int autoStartAppItr(void *parentVoid,
                               void *objVoid,
                               void *itemVoid);

}; /* SPxSiteDatabase */

#endif /* SPX_SITE_DATABASE_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
