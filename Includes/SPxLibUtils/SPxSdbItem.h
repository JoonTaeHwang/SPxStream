/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSdbItem class.
 *
 * Revision Control:
 *   10/06/22 v1.45  SP 	Stop sorting children by display name.
 *
 * Previous Changes:
 *   26/05/22 1.44  SP 	Add SetSortingEnabled().
 *   20/05/22 1.43  SP 	SetExpected() sets ancestors as well.
 *   16/05/22 1.42  SP 	Rework discovery functions.
 *   22/04/22 1.41  SP 	Add alert reporting scope.
 *   21/04/22 1.40  SP 	Changes to JSON keys.
 *   07/04/22 1.39  SP 	Add IsLocal().
 *   10/02/22 1.38  SP 	Add JSON_STATUS.
 *   10/12/20 1.37  SP 	Use SPX_UUID_MAX_LEN.
 *   13/03/20 1.36  SP 	Support status alert list.
 *   10/03/20 1.35  SP 	Optionally purge children in Purge().
 *   09/03/20 1.34  SP 	Simplify application transparent fields.
 *   20/02/20 1.33  SP 	Add JSON_IS_TCP.
 *   19/02/20 1.32  SP 	Add appendHTML().
 *   17/02/20 1.31  SP 	Fix reporting of score by Compare().
 *   11/02/20 1.30  SP 	Remove call to SetLastUpdateTime().
 *   10/02/20 1.29  SP 	Support local and remote discovery flags.
 *   06/02/20 1.28  SP 	Remove CanSetExpected().
 *   05/02/20 1.27  SP 	Remove support for fatal errors.
 *   05/02/20 1.26  SP 	Remove IsLocal().
 *   04/02/20 1.25  SP 	Support local ID.
 *   03/02/20 1.24  SP 	Support auto start.
 *   31/01/20 1.23  SP 	Support fatal error alert.
 *   30/01/20 1.22  SP 	Add more JSON fields.
 *   29/01/20 1.21  SP 	Add SetHowDiscovered().
 *   28/01/20 1.20  SP 	Support discovery, aggregation and reporting scope.
 *   16/01/20 1.19  SP 	Add JSON_IMAGE_NAME.
 *   16/01/20 1.18  SP 	Add JSON_COMMAND_LINE.
 *   06/12/19 1.17  SP 	Remove JSON_PROJECT_CODE.
 *   04/12/19 1.16  SP 	Support per-item alert configuration.
 *   22/11/19 1.15  SP 	Support link to alerts.
 *   21/11/19 1.14  SP 	Changes to support alerts.
 *   15/11/19 1.13  SP 	Move aggregator mode here.
 *   14/11/19 1.12  SP 	Add more JSON fields.
 *   12/11/19 1.11  SP 	Change stream code to projectCode.
 *   08/11/19 1.10  SP 	Add more JSON keys.
 *   07/11/19 1.9   SP 	Support extended heartbeats.
 *   06/11/19 1.8   SP 	Add getNumMaskBits().
 *   04/11/19 1.7   SP 	Changes to generic fields.
 *   31/10/19 1.6   SP 	Further development.
 *   30/10/19 1.5   SP 	Further development.
 *   29/10/19 1.4   SP 	Support generation of JSON messages.
 *   28/10/19 1.3   SP 	Fix build warnings.
 *   25/10/19 1.2   SP 	Rename class and further development.
 *   24/10/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SDB_ITEM_H
#define _SPX_SDB_ITEM_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxSiteAlert.h"
#include "SPxLibUtils/SPxSysUtils.h"

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
class SPxSdbSite;
class SPxSdbSystem;
class SPxSdbApplication;
class SPxSdbStream;
class SPxSdbField;

/*
 *  SPxSdbItem class.
 */
class SPxSdbItem : public SPxObj
{
public:
    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /* JSON field names. */
    static const char *JSON_ALERT_LIST;
    static const char *JSON_ALERTS;
    static const char *JSON_APPLICATION;
    static const char *JSON_APPLICATIONS;
    static const char *JSON_CODE;
    static const char *JSON_COMMAND_LINE;
    static const char *JSON_DISCOVERY_FLAGS;
    static const char *JSON_DIST_ADDR;
    static const char *JSON_DIST_IF_ADDR;
    static const char *JSON_DIST_PORT;
    static const char *JSON_EVENT;
    static const char *JSON_FIELDS;
    static const char *JSON_HOST_ADDR;
    static const char *JSON_IMAGE_NAME;
    static const char *JSON_INSTANCE_ID;
    static const char *JSON_IS_64BIT;
    static const char *JSON_IS_AUTO_START;
    static const char *JSON_IS_ENABLED;
    static const char *JSON_IS_EXPECTED;
    static const char *JSON_IS_LOCAL;
    static const char *JSON_IS_TCP;
    static const char *JSON_ITEM_LIST;
    static const char *JSON_ITEM_REMOVED;
    static const char *JSON_ITEM_UUID_CHANGED;
    static const char *JSON_ITEM1_UUID;
    static const char *JSON_ITEM2_UUID;
    static const char *JSON_KEY;
    static const char *JSON_LOCAL_ID;
    static const char *JSON_MESSAGE;
    static const char *JSON_NAME;
    static const char *JSON_NEW_UUID;
    static const char *JSON_OLD_UUID;
    static const char *JSON_PROCESS_ID;
    static const char *JSON_RTSP_MEDIA;
    static const char *JSON_SENDER_ID;
    static const char *JSON_SENDER_PORT;
    static const char *JSON_SITE_DATABASE;
    static const char *JSON_ALERT_DATABASE;
    static const char *JSON_SITES;
    static const char *JSON_STATE;
    static const char *JSON_STATUS;
    static const char *JSON_STREAMS;
    static const char *JSON_SUBTYPE;
    static const char *JSON_SYSTEMS;
    static const char *JSON_TEXT;
    static const char *JSON_TYPE;
    static const char *JSON_UUID;
    static const char *JSON_VALUE;

    /* Maximum name lengths. */
    static const unsigned int NAME_MAX_LEN = 128;
    static const unsigned int DISPLAY_NAME_MAX_LEN = 256;

    /* Maximum buffer size for UUID. */
    static const unsigned int UUID_MAX_LEN = SPX_UUID_MAX_LEN;

    /*
     * Public types.
     */

    /* Type of class that can be derived from SPxSdbItem.
     * Maintain values in descending order of hierarchy.
     */
    enum ItemType_t
    {
        ITEM_TYPE_UNKNOWN       = 0,
        ITEM_TYPE_DATABASE      = 1,
        ITEM_TYPE_SITE          = 2,
        ITEM_TYPE_SYSTEM        = 3,
        ITEM_TYPE_APPLICATION   = 4,
        ITEM_TYPE_STREAM        = 5
    };

    /* Item scope. Must be listed in ascending order. */
    enum Scope_t
    {
        SCOPE_NONE              = 0,    /* Disabled. */
        SCOPE_LOCAL_SYSTEM      = 1,    /* Within local system only. */
        SCOPE_LOCAL_SITE        = 2,    /* Within local site only. */
        SCOPE_GLOBAL            = 3     /* Everywhere. */
    };

    /* Scope for discovery, aggregation and reporting. */
    struct MonitorScope_t
    {
        Scope_t discovery;      /* What items this monitor can discover. */
        Scope_t aggregation;    /* What items other monitors can tell us about. */
        Scope_t itemReporting;  /* What items this monitor reports to other monitors. */
        Scope_t alertReporting; /* What alerts this monitor reports to other monitors. */
    };

    /* Source or destination of data. */
    enum EndPoint_t
    {
        END_POINT_CONFIG        = 0,    /* Config file. */
        END_POINT_STATUS_MSG    = 1,    /* Network status message. */
        END_POINT_EXT_HB_MSG    = 2     /* Extended heartbeat message. */
    };

    /* Discovery flags. We use a UINT16 so that both local
     * and remote flags can be combined in a UINT32 if needed.
     */
    static const UINT16 DISCO_FLAG_STICKY           = 0x0001;   /* Sticky item added by local monitor. */
    static const UINT16 DISCO_FLAG_PROCESS          = 0x0002;   /* Running process. */
    static const UINT16 DISCO_FLAG_PACKET           = 0x0004;   /* Received packet. */
    static const UINT16 DISCO_FLAG_HEARTBEAT        = 0x0008;   /* Received heartbeat. */
    static const UINT16 DISCO_FLAG_EXT_HEARTBEAT    = 0x0010;   /* Received extended heartbeat. */
    static const UINT16 DISCO_FLAG_PING             = 0x0020;   /* Network ping. */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSdbItem(ItemType_t type, SPxSdbItem *parent, const char *uuid);
    virtual ~SPxSdbItem(void);

    /* General. */
    virtual ItemType_t GetItemType(void) { return m_itemType; }
    virtual SPxSiteDatabase *GetDatabase(void) { return m_database; }
    virtual int IsLocal(void)=0;    /* Implemented in derived class. */
    virtual UINT32 GetLocalID(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetUUID(const char *uuid);
    virtual SPxErrorCode GetUUID(char *buf, unsigned int bufLen);
    virtual int CompareUUID(const char *uuid);
    virtual SPxErrorCode GetDisplayName(char *buf, unsigned int bufLen, int fullName=FALSE);
    virtual SPxErrorCode SetExpected(int isExpected, int setDescendants=TRUE, int setAncestors=TRUE);
    virtual int IsExpected(int checkDescendants=TRUE);
    virtual int IsUnexpected(void);
    virtual SPxErrorCode AddAlertRef(SPxSiteAlert::Type_t type);
    virtual SPxErrorCode RemoveAlertRef(SPxSiteAlert::Type_t type);
    virtual SPxSiteAlert::Type_t GetAlertType(void);
    virtual SPxErrorCode GetCreateTime(SPxTime_t *time);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode GetLastUpdateTime(SPxTime_t *time);

    /* Local and remote item discovery. */
    virtual SPxErrorCode SetHowDiscoveredLocally(UINT16 discoFlags);
    virtual UINT16 GetHowDiscoveredLocally(void);
    virtual SPxErrorCode SetHowDiscoveredRemotely(UINT16 discoFlags);
    virtual UINT16 GetHowDiscoveredRemotely(void);
    virtual int IsDiscoveredByLocalMonitor(void);
    virtual int IsDiscoveredByRemoteMonitor(void);
    virtual int IsDiscovered(void);

    /* Access to item's derived class. */
    virtual SPxSdbSite *GetAsSite(void);
    virtual SPxSdbSystem *GetAsSystem(void);
    virtual SPxSdbApplication *GetAsApplication(void);
    virtual SPxSdbStream *GetAsStream(void);

    /* Ancestor access. */
    virtual SPxSdbItem *GetParent(void) { return m_parent; }
    virtual SPxSdbSite *GetAncestorSite(void);
    virtual SPxSdbSystem *GetAncestorSystem(void);
    virtual SPxSdbApplication *GetAncestorApplication(void);
    virtual int IsAncestorOf(UINT32 localID);

    /* Child access. */
    virtual unsigned int GetNumChildren(void);
    virtual SPxSdbItem *GetChildByIndex(unsigned int index);
    virtual SPxSdbItem *GetChildByLocalID(UINT32 localID);
    virtual SPxSdbItem *GetChildByUUID(const char *uuid);
    virtual SPxErrorCode IterateChildren(SPxCallbackListFn_t fn, void *userArg);

    /* Field access. */
    virtual SPxSdbField *AddOrGetField(const char *key, int allowCreate);
    virtual SPxErrorCode RemoveField(const char *key);
    virtual unsigned int GetNumFields(void);
    virtual SPxSdbField *GetField(unsigned int index);
    virtual SPxErrorCode Purge(double timeoutSecs, int purgeChildren=TRUE);

    /* Alerts. */
    virtual SPxErrorCode SetAlertEnabled(SPxSiteAlert::Code_t code, int isEnabled);
    virtual int IsAlertEnabled(SPxSiteAlert::Code_t code);
    virtual int IsAlertOverride(SPxSiteAlert::Code_t code, int checkChildren);
    virtual SPxErrorCode ClearAlertOverride(SPxSiteAlert::Code_t code, int clearChildren);
    virtual SPxSdbItem *GetAlertOverrideItem(SPxSiteAlert::Code_t code);

    /* Checking scope. */
    virtual int IsInScope(Scope_t scope, int checkDescendants=FALSE);
    virtual int IsInScopeForDestination(const MonitorScope_t *scope, 
                                        EndPoint_t dest);

    /* Config load/save. */
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);
    virtual void SetSaveUpdateOnly(int updateOnly);

    /*
     * Public static functions.
     */

    static const char *GetItemTypeName(ItemType_t type);

protected:

    /*
     * Protected types.
     */

    /* Alert info. */
    struct AlertInfo_t
    {
        SPxSiteAlert::Code_t code;  /* Alert code. */
        int isEnabled;              /* TRUE or FALSE. */
    };

    /* How an item was discovered by monitor. */
    struct HowDiscovered_t
    {
        /* Sticky item added by a local monitor. */
        int sticky;
        SPxTime_t stickyLastUpdateTime;

        /* Detected from a running process. */
        int process;
        SPxTime_t processLastUpdateTime;

        /* Detected from a received network packet. */
        int packet;
        SPxTime_t packetLastUpdateTime;

        /* Discovered from a receivedheartbeat. */
        int heartbeat;
        SPxTime_t heartbeatLastUpdateTime;

        /* Discovered a received extended heartbeat. */
        int extHeartbeat;
        SPxTime_t extHeartbeatLastUpdateTime;

        /* Discovered by ping detection. */
        int ping;
        SPxTime_t pingLastUpdateTime;
    };

    /*
     * Protected variables.
     */

    /* Max comparison score. */
    static const unsigned int MAX_SCORE = 33;

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* General. */
    virtual SPxErrorCode initialise(void);
    virtual SPxErrorCode shutdown(void);
    virtual SPxErrorCode addChild(SPxSdbItem *child);
    virtual SPxErrorCode removeChild(SPxSdbItem *child);
    virtual SPxErrorCode setDisplayName(const char *format, ...);
    virtual void updateDisplayName(void)=0; /* Implement in a derived class. */
    virtual SPxErrorCode purgeDesc(double timeoutSecs)=0; /* Implement in a derived class. */

    /* JSON support. */
    virtual SPxErrorCode handleJSON(const void *jsonVoid, 
                                    const MonitorScope_t *scope, 
                                    EndPoint_t source);
    virtual SPxErrorCode appendJSON(void *writerVoid, 
                                    const MonitorScope_t *scope, 
                                    EndPoint_t dest);

    /* HTML support. */
    virtual SPxErrorCode appendHTML(char **buf, 
                                    unsigned int *bufLen, 
                                    unsigned int level,
                                    int isExpected,
                                    const char *suffix=NULL);
    virtual SPxErrorCode appendLineHTML(char **buf, 
                                        unsigned int *bufLen,
                                        const char *line);

    /* Debugging. */
    virtual void logMesg(unsigned int verbosity, const char *format, ...);

    /*
     * Protected static functions.
     */

    static unsigned int getNumMaskBits(UINT32 mask);

    static bool compareItems(SPxSdbItem *i1, SPxSdbItem *i2);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;                       /* Private data. */
    SPxSiteDatabase *m_database;                /* Parent database. */
    SPxSdbItem *m_parent;                       /* Parent item. */
    ItemType_t m_itemType;                      /* This item type. */
    UINT32 m_localID;                           /* Local ID. */
    int m_isExpected;                           /* Expected or discovered? */
    SPxTime_t m_createTime;                     /* Time created. */
    SPxTime_t m_lastUpdateTime;                 /* Time of last update. */
    unsigned int m_errorAlertRefCount;          /* Count of error alerts. */
    unsigned int m_warningAlertRefCount;        /* Count of warning alerts. */
    HowDiscovered_t m_localDiscovery;           /* How item was discovered by this monitor. */
    HowDiscovered_t m_remoteDiscovery;          /* How item was discovered by a remote monitor. */

    /*
     * Private functions.
     */

    /* Fields. */
    SPxErrorCode handleJSONFields(const void *jsonFieldsVoid);
    SPxErrorCode handleJSONFieldsObject(const void *jsonFieldsVoid);
    SPxErrorCode handleJSONFieldsArray(const void *jsonFieldsVoid);
    SPxErrorCode appendJSONFields(void *writerVoid);
    SPxErrorCode appendJSONField(void *writerVoid, SPxSdbField *field);

    /* Alert info. */
    SPxErrorCode handleJSONAlertInfos(const void *jsonAIsVoid);
    SPxErrorCode handleJSONAlertInfo(const void *jsonAIVoid);
    SPxErrorCode appendJSONAlertInfos(void *writerVoid);
    SPxErrorCode appendJSONAlertInfo(void *writerVoid, AlertInfo_t *ai);

    /* Discovery. */
    SPxErrorCode purgeDiscovery(int isLocal, HowDiscovered_t *disco, double timeoutSecs);

    /*
     * Private static functions.
     */

    static int isDescendantItr(void *parentVoid,
                               void *uuidVoid,
                               void *itemVoid);

    static bool compareAlerts(const AlertInfo_t &ai1, const AlertInfo_t &ai2);

}; /* SPxSdbItem */

#endif /* _SPX_SDB_ITEM_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
