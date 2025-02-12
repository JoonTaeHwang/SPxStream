/*********************************************************************
 *
 * (c) Copyright 2017 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmDatabase class which supports various 
 *   types of track alarms.
 *
 * Revision Control:
 *   31/01/22 v1.44  AJH 	Support save/restore of message TCP/interface state.
 *
 * Previous Changes:
 *   18/05/21 1.43  AJH Support retrieval of alarm sound name.
 *   17/05/21 1.42  AJH Fix typo in constant name.
 *   11/05/21 1.41  SP 	Use renamed SPxAlarm functions.
 *   10/05/21 1.40  AJH	Add GetRefLatLong().
 *   30/04/21 1.39  SP 	Remove unnecessary virtual from functions.
 *   28/04/21 1.38  SP 	Support state filters in remote control commands.
 *   28/04/21 1.37  SP 	Support TYPE_LOG_TO_FILE alarm action.
 *   27/04/21 1.36  SP 	Support load & save of embedded track groups.
 *   26/04/21 1.35  AJH	Allow reference position to be set.
 *   16/04/21 1.34  AJH	Remember last-used config file name.
 *   08/04/21 1.33  SP 	Extend remote control to support SPxAlarmServer.
 *   10/02/21 1.32  AGC	Support option for alarming tracks being set as red.
 *                     	Mark more functions as const.
 *   28/09/20 1.31  AGC	Add IsTrackGroupUsedInConfig().
 *   19/12/19 1.30  BTB	Support making alarming tracks bold.
 *   13/05/19 1.29  SP 	Further support for remote control.
 *   10/05/19 1.28  SP 	Add support for remote control.
 *   10/05/19 1.27  SP 	Support TTM alarm message containing track descriptor.
 *   21/03/19 1.26  BTB	Support sound alarm once action.
 *			Support custom alarm sounds.
 *   04/01/19 1.25  BTB Send network messages per alarm on a timer with an alarm identifier.
 *   17/12/18 1.24  BTB Check for timeouts (for alarms with auto acknowledge/clear actions).
 *   14/11/18 1.23  BTB Handle load, save and deletion of shared alarms.
 *   12/11/18 1.22  BTB Handle load and delete all events from the area database.
 *   07/06/18 1.21  SP 	Support interleaving of alarm sounds.
 *   23/05/18 1.20  SP 	Support network message alarm action.
 *   18/05/18 1.19  SP 	Support more built-in alarm actions.
 *   17/05/18 1.18  SP 	Add SetConfigsState().
 *   17/05/18 1.17  SP 	Include CPA track in action event.
 *   03/05/18 1.16  SP 	Move units configuraion to SPxUniTrackDatabase.
 *   01/05/18 1.15  SP 	Remove area database from constructor.
 *   25/04/18 1.14  SP 	Add updateAlarm().
 *   13/12/17 1.13  SP 	Make some functions virtual.
 *   11/12/17 1.12  SP 	Changes to support CPA alarms.
 *                      Remove support for reference points.
 *   23/11/17 1.11  SP 	Support reporting of alarms from multiple configs.
 *   22/11/17 1.10  SP 	Add support for gates using SPxAreaObj.
 *   17/11/17 1.9   SP 	Add Load() and Save().
 *   13/11/17 1.8   SP 	Rename alarm action.
 *   08/11/17 1.7   SP 	Support alarm actions.
 *   03/11/17 1.6   SP 	Use SPxAreaObj for areas.
 *   01/11/17 1.5   SP 	Support renamed class.
 *   31/10/17 1.4   SP 	Rework to support alarm clauses.
 *   25/10/17 1.3   SP 	Further development.
 *   23/10/17 1.2   SP 	Further development.
 *   18/10/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_DATABASE_H
#define _SPX_ALARM_DATABASE_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxAlarm.h"
#include "SPxLibUtils/SPxAlarmConfig.h"
#include "SPxLibUtils/SPxAlarmCondition.h"
#include "SPxLibUtils/SPxAlarmAction.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxSound.h"
#include "SPxLibNet/SPxUniTrack.h"
#include "SPxLibNet/SPxPacketSend.h"

/* Forward declare any classes required. */
class SPxUniTrackDatabase;
class SPxAlarmTrack;
class SPxAreaObj;

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Parameters. */
#define SPX_ALARM_DB_PARAM_TRACK_TYPES              "TrackTypes"
#define SPX_ALARM_DB_PARAM_NET_MESG_TYPE            "NetMesgType"
#define SPX_ALARM_DB_PARAM_NET_MESG_ADDR            "NetMesgAddress"
#define SPX_ALARM_DB_PARAM_NET_MESG_PORT            "NetMesgPort"
#define SPX_ALARM_DB_PARAM_NET_MESG_INTERFACE       "NetMesgInterface"
#define SPX_ALARM_DB_PARAM_NET_MESG_TCP             "NetMesgTCP"
#define SPX_ALARM_DB_PARAM_CUSTOM_ALARM_FILE        "CustomAlarmSoundFile-"
#define SPX_ALARM_DB_PARAM_CUSTOM_ALARM_NAME        "CustomAlarmSoundName-"
#define SPX_ALARM_DB_PARAM_REF_LAT_LONG             "RefLatLong"
#define SPX_ALARM_DB_PARAM_ALARM_LOG_FILE_PREFIX    "AlarmLogFilePrefix"

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
class SPxAlarmDatabase : public SPxObj
{
    /* The objects these can access our internals directly. */
    friend class SPxAlarm;
    friend class SPxAlarmConfig;
    friend class SPxAlarmCondition;
    friend class SPxAlarmAction;

public:

    /*
     * Public types.
     */

    /* Network message type. */
    typedef enum
    {
        NET_MESG_TYPE_NONE      = 0,    /* No network message. */
        NET_MESG_TYPE_TTM_V1    = 1,    /* TTM message containing track ID (was SPX_PACKET_MISC_NMEA_0183). */
        NET_MESG_TYPE_TTM_V2    = 2,    /* TTM message containing track descriptor. */

        /* For backward compatibility. */
        NET_MESG_TYPE_TTM_V1_DEPRECATED = 183

    } NetMesgType_t;

    /* Notification event type. */
    typedef enum
    {
        EVENT_LOAD_START,           /* Load from config has started. */
        EVENT_LOAD_COMPLETE,        /* Load from config has completed. */
        EVENT_ALARM_CREATE,         /* An alarm has been created. */
        EVENT_ALARM_CHANGE,         /* An alarm has changed. */
        EVENT_ALARM_DELETE,         /* An alarm is about to be been deleted. */
        EVENT_CONFIG_CREATE,        /* An alarm config has been created. */
        EVENT_CONFIG_CHANGE,        /* An alarm config has changed. */
        EVENT_CONFIG_DELETE,        /* An alarm config is about to be been deleted. */
        EVENT_CONDITION_CREATE,     /* An alarm condition has been created. */
        EVENT_CONDITION_CHANGE,     /* An alarm condition has changed. */
        EVENT_CONDITION_DELETE,     /* An alarm condition is about to be been deleted. */
        EVENT_ACTION_CREATE,        /* An alarm action has been created. */
        EVENT_ACTION_CHANGE,        /* An alarm action has changed. */
        EVENT_ACTION_START,         /* An alarm action has been started. */
        EVENT_ACTION_STOP,          /* An alarm action has been stopped. */
        EVENT_ACTION_DELETE         /* An alarm action is about to be been deleted. */

    } EventType_t;

    /* Alarm event info. Used by:
     *  EVENT_ALARM_CREATE
     *  EVENT_ALARM_CHANGE
     *  EVENT_ALARM_DELETE
     */
    typedef struct
    {
        SPxAlarm *alarm;                    /* Alarm handle. */
        SPxAlarm::ChangeFlags_t flags;      /* Change flags. */

    } EventAlarmInfo_t;

    /* Alarm config event info. Used by:
     *  EVENT_CONFIG_CREATE
     *  EVENT_CONFIG_CHANGE
     *  EVENT_CONFIG_DELETE
     */
    typedef struct
    {
        SPxAlarmConfig *config;                 /* Config handle. */
        SPxAlarmConfig::ChangeFlags_t flags;    /* Change flags. */

    } EventConfigInfo_t;

    /* Condition event info. Used by:
     *  EVENT_CONDITION_CREATE
     *  EVENT_CONDITION_CHANGE
     *  EVENT_CONDITION_DELETE
     */
    typedef struct
    {
        SPxAlarmCondition *condition;               /* Condition handle. */
        SPxAlarmCondition::ChangeFlags_t flags;     /* Change flags. */

    } EventConditionInfo_t;

    /* Action event info. Used by:
     *  EVENT_ACTION_CREATE
     *  EVENT_ACTION_CHANGE
     *  EVENT_ACTION_DELETE
     *  EVENT_ACTION_START
     *  EVENT_ACTION_STOP
     */
    typedef struct
    {
        SPxAlarmAction *action;                 /* Action handle. */
        SPxAlarmAction::ChangeFlags_t flags;    /* Change flags. */
        SPxUniTrack *track1;                    /* Track that triggered the alarm or NULL. */
        SPxUniTrack *track2;                    /* Second track for CPA, proximity, etc or NULL. */

    } EventActionInfo_t;

    /* Union of all event info structures. */
    typedef union
    {
        EventAlarmInfo_t alarmEvt;
        EventConfigInfo_t configEvt;
        EventConditionInfo_t conditionEvt;
        EventActionInfo_t actionEvt;
    
    } EventInfo_t;

    /* Top-level event structure. Use type to determine the event type 
     * and then access one (and only one) of the event info structures 
     * that follow.
     */
    typedef struct
    {
        EventType_t type;       /* Event type. */
        EventInfo_t info;       /* Event info. */
    
    } Event_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxAlarmDatabase(SPxUniTrackDatabase *trackDB,
                              int alarmTracksAreBold=FALSE,
                              int alarmTracksAreRed=TRUE);
    virtual ~SPxAlarmDatabase(void);

    /* Access. */
    SPxUniTrackDatabase *GetTrackDB(void) { return m_trackDB; }
    SPxAreaObj *GetAreaDB(void) { return m_areaDB; }

    /* Reference position. */
    SPxErrorCode SetRefLatLong(double latDegs, double longDegs);
    SPxErrorCode GetRefLatLong(double *latDegs, double *longDegs, int *refSet);

    /* Triggered alarms. */
    SPxErrorCode SetAlarmTrackTypes(SPxUniTrack::TrackType_t filters);
    SPxUniTrack::TrackType_t GetAlarmTrackTypes(void) const { return m_alarmTrackTypes; }
    unsigned int GetNumAlarms(SPxAlarm::State_t states=SPxAlarm::STATE_ANY) const;
    SPxErrorCode IterateAlarms(SPxCallbackListFn_t fn, 
                               void *userArg, 
                               int mutexAlreadyLocked=FALSE);
    SPxErrorCode DeleteAlarm(SPxAlarm *alarm);
    SPxErrorCode DeleteAlarms(SPxAlarm::State_t states=SPxAlarm::STATE_ANY);
    SPxErrorCode SetAlarmsState(SPxAlarm::State_t newState, 
                                SPxUniTrack *track=NULL,
                                SPxAlarm::State_t states=SPxAlarm::STATE_ANY);
    SPxErrorCode SetNextAlarmsState(SPxAlarm::State_t state);

    /* Alarm configs. */
    SPxAlarmConfig *CreateConfig(SPxErrorCode *errRtn=NULL);
    unsigned int GetNumConfigs(SPxAlarmConfig::State_t states=SPxAlarmConfig::STATE_ANY) const;
    SPxErrorCode IterateConfigs(SPxCallbackListFn_t fn, 
                                void *userArg,
                                int mutexAlreadyLocked=FALSE);
    SPxErrorCode DeleteConfig(SPxAlarmConfig *config);
    SPxErrorCode DeleteConfigs(SPxAlarmConfig::State_t states=SPxAlarmConfig::STATE_ANY);
    SPxErrorCode DeleteSharedConfigs(SPxAlarmConfig::State_t states=SPxAlarmConfig::STATE_ANY);
    SPxErrorCode SetConfigsState(SPxAlarmConfig::State_t state);
    int IsTrackGroupUsedInConfig(UINT32 trackGroupID) const;

    /* Network messages.*/
    SPxErrorCode SetNetMesgType(NetMesgType_t type);
    NetMesgType_t GetNetMesgType(void)  const { return m_netMesgType; }
    SPxPacketSender *GetNetMesgSender(void) { return &m_netMesgSender; }

    /* Alarm logging. */
    SPxErrorCode SetAlarmLogFilePrefix(const char *prefix);
    const char *GetAlarmLogFilePrefix(void);

    /* Event callbacks. */
    SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Config load/save. */
    SPxErrorCode SetLoadTrackGroupsEnabled(int isEnabled);
    SPxErrorCode SetSaveTrackGroupsEnabled(int isEnabled);
    SPxErrorCode Load(const char *filename, int sharedConfig=FALSE);
    SPxErrorCode Save(const char *filename, int sharedConfig=FALSE);
    SPxErrorCode Reset(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Event reporting. */
    SPxErrorCode reportEvent(Event_t *evt);

    /* Alarm sounds. */
    const char *getCustomAlarmSoundName(unsigned int customAlarmIndex);
    const char *getCustomAlarmSoundFile(unsigned int customAlarmIndex);
    SPxErrorCode startStopCustomAlarmSound(unsigned int customAlarmIndex, int isStart, int playOnce);
    SPxErrorCode startStopBuiltInAlarmSound(SPxAlarmSound_t alarm, int isStart, int playOnce);

    /* File logging. */
    SPxErrorCode logAlarmToFile(SPxAlarm *alarm);

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

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxUniTrackDatabase *m_trackDB;                     /* Track database. */
    SPxAreaObj *m_areaDB;                               /* Area database. */
    SPxCallbackList *m_eventCBList;                     /* Event callback list. */
    SPxLatLong_t m_refLL;                               /* Reference lat/long for reporting. */
    int m_isRefLLSet;                                   /* Is the reference set? */
    int m_alarmTracksAreBold;                           /* Whether alarm tracks should be rendered as bold. */
    int m_alarmTracksAreRed;                            /* Whether alarm tracks should be rendered in red (if supported). */
    SPxUniTrack::TrackType_t m_alarmTrackTypes;         /* Track types to alarm on. */
    SPxTimer m_alarmSoundsTimer;                        /* Timer used to play alarm sounds. */
    int m_builtInAlarmRefCounts[SPX_NUM_ALARM_SOUNDS];  /* Ref count for playing each built-in alarm sound. */
    int m_builtInAlarmOnceRefCounts[SPX_NUM_ALARM_SOUNDS];  /* Ref count for playing each built-in alarm sound once. */
    SPxTimer m_alarmTimeoutTimer;                       /* Timer used to check for alarm timeouts. */
    SPxTimer m_netMesgTimer;                            /* Timer used for sending network messages. */
    NetMesgType_t m_netMesgType;                        /* Network message type. */
    SPxPacketSender m_netMesgSender;                    /* Network message sender. */
    char *m_configFile;                                 /* Most recent alarm definitions file. */
    int m_loadTrackGroupsEnabled;                       /* Handle loading of track groups? */
    int m_saveTrackGroupsEnabled;                       /* Handle saving of track groups? */

    /*
     * Private functions.
     */

    /* Track database event handlers. */
    void handleTrackCreate(SPxUniTrack *track);
    void handleTrackUpdate(SPxUniTrack *track);
    void handleTrackDelete(SPxUniTrack *track);

    /* Area delete handler. */
    void handleShapeDelete(SPxAreaObj *shape);
    void handleShapeDeleteAll(void);
    void handleShapeReloadAll(void);

    /* Track context. */
    SPxAlarmTrack *getTrackContext(SPxUniTrack *track, int allowCreate);

    /* Triggered alarms. */
    SPxAlarm *getNewAlarm(void);
    SPxErrorCode setAlarmInUse(SPxAlarm *alarm, int isInUse);
    SPxAlarm *getAlarmFromID(UINT32 id);
    SPxAlarm *getAlarmFromIndex(unsigned int index);
    SPxAlarm *findAlarm(SPxAlarmConfig *config, SPxUniTrack *track);
    SPxErrorCode deleteAlarm(SPxAlarm *alarm);

    /* Alarm configs. */
    SPxAlarmConfig *getNewConfig(void);
    SPxErrorCode setConfigInUse(SPxAlarmConfig *config, int isInUse);
    SPxAlarmConfig *getConfigFromID(UINT32 id);
    SPxAlarmConfig *getConfigFromIndex(unsigned int index);
    SPxErrorCode deleteConfig(SPxAlarmConfig *config);

    /* Alarm conditions. */
    SPxAlarmCondition *getConditionFromID(UINT32 id, SPxAlarmConfig **configRtn);

    /* Alarm actions. */
    SPxAlarmAction *getActionFromID(UINT32 id, SPxAlarmConfig **configRtn);

    /* Reporting. */
    SPxErrorCode checkConfigs(SPxUniTrack *track);
    SPxErrorCode reportAlarm(SPxUniTrack *track, SPxAlarmConfig *config);
    SPxErrorCode updateAlarm(SPxAlarm *alarm, int isTriggered);

    /* Network messages. */
    void checkSendNetMesgs(void);
    SPxErrorCode sendNetMesgTTM(SPxUniTrack *track, SPxAlarm *alarm);
    SPxErrorCode sendNetMesg(const void *mesg, unsigned int sizeBytes);

    /* Alarm sounds. */
    SPxErrorCode addCustomAlarmSound(unsigned int customAlarmIndex, const char *filename);
    void playAlarmSounds(void);

    /* Alarm timeouts. */
    void checkAlarmTimeouts(void);

    /* Remote control parameters. */
    SPxErrorCode setRcParameter(char *name, char *value);
    SPxErrorCode getRcParameter(char *name, char *valueBuf, int bufLen);
    SPxErrorCode setDepRcParameter(char *name, char *value);
    SPxErrorCode getDepRcParameter(char *name, char *valueBuf, int bufLen);

    /*
     * Private static functions.
     */

    static int trackDatabaseEventHandler(void *invokingObjPtr,
                                         void *userObjPtr,
                                         void *eventPtr);

    static int trackDatabaseDeletedHandler(void *invokingObjPtr,
                                           void *userObjPtr,
                                           void *argPtr);

    static void areaDatabaseEventHandler(SPxAreaObj *area, 
                                         UINT32 flags, 
                                         void *userArg);

    static void alarmSoundsTimer(void *userArg);

    static void alarmTimeoutsTimer(void *userArg);

    static void alarmNetMesgsTimer(void *userArg);

    static int isTrackGroupUsedInCondition(void *invokingObject,
                                           void *userObject,
                                           void *arg);

}; /* SPxAlarmDatabase */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_ALARM_DATABASE_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
