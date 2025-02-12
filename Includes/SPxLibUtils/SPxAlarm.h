/*********************************************************************
 *
 * (c) Copyright 2017 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarm class, which is used by the 
 *   SPxAlarmDatabase to report triggered alarms.
 *
 * Revision Control:
 *   12/05/21 v1.31  SP 	Only report CPA/TCPA for CPA alarms.
 *
 * Previous Changes:
 *   11/05/21 1.30  SP 	Make track access functions more generic.
 *   28/04/21 1.29  SP 	Support TYPE_LOG_TO_FILE alarm action.
 *   28/04/21 1.28  BTB	Add STATE_UNKNOWN alarm state.
 *   23/04/21 1.27  SP 	Correct typo in TYPE_AUTO_ACKNOWLEDGE.
 *   16/04/21 1.26  SP 	Tidy up formatting.
 *   17/03/21 1.25  AGC	Suppress cppcheck warning.
 *   10/02/21 1.24  AGC	Support auto-reset, auto-delete and highlight shape actions.
 *   10/05/19 1.23  SP 	Add unique alarm ID.
 *   21/03/19 1.22  BTB	Support sound alarm once action.
 *   04/01/19 1.21  BTB Support identifying alarms in network messages.
 *   17/12/18 1.20  BTB Support auto acknowledge and auto clear actions.
 *			Handle actions which stop only on cleared state.
 *   13/11/18 1.19  BTB Handle reload of all shapes.
 *   12/11/18 1.18  BTB Handle deletion of all shapes.
 *   19/08/18 1.17  SP 	Support CPA to ref point.
 *   25/06/18 1.16  AGC	Correctly suppress cppcheck warning.
 *   12/06/18 1.15  SP 	Add GetCPATrack() and GetCPATrackName().
 *   06/06/18 1.14  SP 	Add GetNextState().
 *   24/05/18 1.13  SP 	Move alarm action processing here.
 *   25/04/18 1.12  SP 	Add GetTriggerCount().
 *   15/02/18 1.11  SP 	Handle shape deletion.
 *   13/12/17 1.10  SP 	Remove unused SPxGate.
 *   11/12/17 1.9   SP 	Support CPA conditions.
 *   17/11/17 1.8   SP 	Support acknowledged state.
 *   13/11/17 1.7   SP 	Changes to how events are reported.
 *   08/11/17 1.6   SP 	Support alarm actions.
 *   03/11/17 1.5   SP 	Use SPxAreaObj for areas.
 *   31/10/17 1.4   SP 	Rework to support alarm clauses.
 *   25/10/17 1.3   SP 	Use SPxAreaObj for areas.
 *   23/10/17 1.2   SP 	Further development.
 *   18/10/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_H
#define _SPX_ALARM_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxViewControl.h"
#include "SPxLibUtils/SPxAlarmAction.h"

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

/* Forward-declare other classes we use. */
struct SPxAlarmPriv;
class SPxAlarmDatabase;
class SPxAlarmConfig;
class SPxUniTrack;
class SPxAreaObj;
class SPxRefPoint;

/*
 * Define our class.
 */
class SPxAlarm : public SPxObj
{
    /* The database class can access our internals directly. */
    friend class SPxAlarmDatabase;

public:
    /*
     * Public types.
     */

    /* Alarm states. */
    typedef UINT32 State_t;
    static const UINT32 STATE_UNKNOWN       = 0x00000000;
    static const UINT32 STATE_CLEARED       = 0x00000001;
    static const UINT32 STATE_ACKNOWLEDGED  = 0x00000002;
    static const UINT32 STATE_ACTIVE        = 0x00000004;
    static const UINT32 STATE_EMPTY         = 0x00000008;
    static const UINT32 STATE_ANY           = 0xFFFFFFFF;

    /* Change flags. */
    typedef UINT32 ChangeFlags_t;
    static const UINT32 CHANGE_STATE        = 0x00000001;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by the SPxAlarmDatabase.
     */

    /* Configuration. */
    virtual SPxAlarmConfig *GetConfig(void) { return m_config; }
    virtual const char *GetConfigName(void);

    virtual UINT32 GetID(void) { return m_id; }

    virtual SPxErrorCode SetState(State_t state);
    virtual SPxErrorCode SetNextState(void);
    virtual State_t GetState(void) { return m_state; }
    virtual State_t GetNextState(void);
    virtual const char *GetStateString(void);
    virtual SPxTime_t GetStateChangeTime(void) { return m_stateChangeTime; }

    virtual SPxUniTrack *GetTrack1(void) { return m_track1; }
    virtual const char *GetTrack1Name(int mutexAlreadyLocked=FALSE);
    virtual SPxUniTrack *GetTrack2(void) { return m_track2; }
    virtual const char *GetTrack2Name(int mutexAlreadyLocked=FALSE);

    virtual const char *GetTriggerString(int mutexAlreadyLocked=FALSE);
    virtual SPxTime_t GetTriggerTime(void) { return m_triggerTime; }
    virtual unsigned int GetTriggerCount(void);

    virtual int GetCPA(double *cpaMetresRtn,
                       double *tcpaSecsRtn=NULL,
                       SPxLatLong_t *startLL1Rtn=NULL,
                       SPxLatLong_t *endLL1Rtn=NULL,
                       SPxLatLong_t *startLL2Rtn=NULL,
                       SPxLatLong_t *endLL2Rtn=NULL);
    virtual double GetCPALimitMetres(void) { return m_cpaLimitMetres; }
    virtual double GetTCPALimitSecs(void) { return m_tcpaLimitSecs; }

    virtual SPxTime_t GetResetTime(void) { return m_resetTime; }

    virtual UINT32 GetAcknowledgeTimeout(void) { return m_acknowledgeTimeout; }

    virtual UINT32 GetClearTimeout(void) { return m_clearTimeout; }

    virtual int IsAutoReset(void) const { return m_isAutoReset; }
    virtual int IsAutoDelete(void) const { return m_isAutoDelete; }

    virtual int IsSendingNetMesgs(void) { return m_sendingNetMesgs; }

    virtual UINT32 GetNetMesgAlarmValue(void) { return m_netMesgAlarmValue; }

    /*
     * Public static functions.
     */

    static const char *GetStateString(State_t state);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Protected functions may be accessed by the SPxAlarmDatabase. */

    /* Constructor and destructor. */
    SPxAlarm(SPxAlarmDatabase *alarmDB);
    virtual ~SPxAlarm(void);

    /* Configuration. */
    virtual SPxErrorCode initialise(SPxAlarmConfig *config, 
                                    SPxUniTrack *track);
    virtual SPxErrorCode activate(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode reset(void);
    virtual void handleConfigDelete(SPxAlarmConfig *config);
    virtual void handleActionCreate(SPxAlarmAction *action);
    virtual void handleActionChange(SPxAlarmAction *action, SPxAlarmAction::ChangeFlags_t flags);
    virtual void handleActionDelete(SPxAlarmAction *action);
    virtual void handleTrackDelete(SPxUniTrack *track);
    virtual void handleShapeDelete(SPxAreaObj *shape);
    virtual void handleShapeDeleteAll(void);
    virtual void handleShapeReloadAll(void);
    virtual int isMatch(SPxAlarmConfig *config, SPxUniTrack *track);
    virtual void updateTriggerCount(void);
    virtual void resetTriggerCount(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void checkTimeouts(int mutexAlreadyLocked=FALSE);

private:

    /*
     * Private types.
     */

    /* Use to store additional action information. */
    typedef struct
    {
        SPxAlarmAction *action;         /* Action. */
        UINT32 optionValue;             /* Last option value. */
        int isStarted;                  /* Is action started? */

    } ActionInfo_t;

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxCriticalSection m_mutex;         /* Mutex. */
    SPxAlarmDatabase *m_alarmDB;        /* Alarm database . */
    UINT32 m_id;                        /* Unique ID for this alarm. */
    SPxAlarmConfig *m_config;           /* Alarm config or NULL. */
    UINT32 m_configID;                  /* Associated config ID. */
    State_t m_state;                    /* Alarm state. */
    unsigned int m_triggerCount;        /* Consecutive trigger count. */
    SPxTime_t m_triggerTime;            /* Alarm trigger time. */
    SPxTime_t m_resetTime;              /* Last reset time. */
    SPxTime_t m_stateChangeTime;        /* Last state change time. */
    SPxUniTrack *m_track1;              /* Trigger track. */
    SPxUniTrack *m_track2;              /* Track that was set as a condition argument. */
    SPxAreaObj *m_shape;                /* Shape that was set as a condition argument. */
    SPxRefPoint *m_refPoint;            /* Ref point that was set as a condition argument. */
    int m_isTrackAlarmSet;              /* Is track set to alarm state? */
    int m_isTrackClearedAlarmSet;       /* Is track triggering any cleared alarms? */
    int m_areActionsStarted;            /* Are alarm actions started? */
    int m_areStopOnClearActionsStarted; /* Are stop-on-clear alarm actions started? */
    int m_areStopOnEmptyActionsStarted; /* Are stop-on-empty alarm actions started? */
    UINT32 m_lastCPAUpdateTime;         /* Last time CPA was updated. */
    int m_isCPAAlarm;                   /* Is this alarm triggered by a CPA condition? */
    int m_isCPAValid;                   /* Is CPA valid? */
    double m_cpaMetres;                 /* CPA that triggered alarm in metres. */
    double m_tcpaSecs;                  /* TCPA that triggered alarm in seconds. */
    double m_cpaLimitMetres;            /* Condition CPA limit in metres. */
    double m_tcpaLimitSecs;             /* Condition TCPA limit nin seconds. */
    SPxLatLong_t m_startLL1;            /* CPA vector 1 start position. */
    SPxLatLong_t m_endLL1;              /* CPA vector 1 end position. */
    SPxLatLong_t m_startLL2;            /* CPA vector 2 start position. */
    SPxLatLong_t m_endLL2;              /* CPA vector 2 end position. */
    UINT32 m_acknowledgeTimeout;        /* Timeout to acknowledge an alarm on. */
    UINT32 m_clearTimeout;              /* Timeout to clear an alarm on. */
    int m_isAutoReset;                  /* Should alarms auto-reset? */
    int m_isAutoDelete;                 /* Should alarms auto-delete? */
    int m_sendingNetMesgs;              /* Are we sending network messages for this alarm? */
    UINT32 m_netMesgAlarmValue;         /* Alarm value to send in the network messages. */

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode setState(State_t state);
    SPxErrorCode reportChangeEvent(ChangeFlags_t flags);
    void updateCPA(void);

    /* Alarm actions. */
    SPxErrorCode addAction(SPxAlarmAction *action);
    SPxErrorCode removeAction(SPxAlarmAction *action);
    ActionInfo_t *getActionInfo(SPxAlarmAction *action, int *indexRtn=NULL);
    SPxErrorCode startStopActions(void);
    SPxErrorCode startStopAction(ActionInfo_t *info);
    SPxErrorCode startStopAction(ActionInfo_t *info, int isStart);
    SPxErrorCode startStopSoundAlarmAction(int isStart, UINT32 optionValue);
    SPxErrorCode startStopHighlightTrackAction(int isStart, UINT32 optionValue);
    SPxErrorCode startStopSendNetMesgAction(int isStart, UINT32 optionValue);
    SPxErrorCode startStopAutoAcknowledgeAction(int isStart, UINT32 optionValue);
    SPxErrorCode startStopAutoClearAction(int isStart, UINT32 optionValue);
    SPxErrorCode startStopAutoResetAction(int isStart);
    SPxErrorCode startStopAutoDeleteAction(int isStart);
    SPxErrorCode startStopHighlightShapesAction(int isStart, UINT32 optionValue);
    SPxErrorCode doSoundAlarmOnceAction(UINT32 optionValue);
    SPxErrorCode doLogToFileAction(void);

    /*
     * Private static functions.
     */

    static int addActionIterator(void *configVoid, 
                                 void *objVoid, 
                                 void *actionVoid);

    static int startStopHighlightShapesActionIter(void *invokingObject,
                                                  void *userObject,
                                                  void *arg);

}; /* SPxAlarm */

#endif /* SPX_ALARM_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
