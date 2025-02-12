/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for the SPxAlarmServer remote control interface.
 *
 * Revision Control:
 *   12/07/21 v1.7   AJH 	Add retrieval of trigger/state change times.
 *
 * Previous Changes:
 *   09/06/21 1.6   AJH	Add GetTrigAlarmCPA().
 *   28/04/21 1.5   SP 	Support state filtering in
 *                      DeleteTrigAlarms() and SetTrigAlarmStates().
 *   26/04/21 1.4   SP 	More triggered alarm functions.
 *   20/04/21 1.3   SP 	Rework track group functions.
 *   15/04/21 1.2   SP 	Fix various issues.
 *   14/04/21 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_REMOTE_AS_H
#define _SPX_REMOTE_AS_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibNet/SPxRemoteServer.h"
#include "SPxLibUtils/SPxAlarmConfig.h"
#include "SPxLibUtils/SPxAlarmCondition.h"
#include "SPxLibUtils/SPxAlarmAction.h"
#include "SPxLibUtils/SPxAlarm.h"

/* Forward declare any classes required. */

/*
 * Define our class.
 */
class SPxRemoteAS : public SPxRemoteServer
{
public:

    /*
     * Public types.
     */

    /* Configured alarm ID. */
    typedef unsigned int ConfAlarmID_t ;

    /* Configured alarm condition ID. */
    typedef unsigned int ConditionID_t;

    /* Configured alarm action ID. */
    typedef unsigned int ActionID_t;

    /* Triggered alarm ID. */
    typedef unsigned int TrigAlarmID_t;

    /* Track group ID. */
    typedef unsigned int TrackGroupID_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxRemoteAS(void);
    virtual ~SPxRemoteAS(void);

    /* General. */
    SPxErrorCode GetTitle(char *title, unsigned int bufLen);
    SPxErrorCode GetVersion(UINT32 *version);
    SPxErrorCode GetVersion(char *version, unsigned int bufLen);
    SPxErrorCode Reset(void);
    SPxErrorCode Load(const char *filename);
    SPxErrorCode Save(const char *filename);

    /* Configured alarms. */
    SPxErrorCode GetNumConfAlarms(unsigned int *count);
    SPxErrorCode GetConfAlarm(unsigned int index, ConfAlarmID_t *caID);
    SPxErrorCode CreateConfAlarm(ConfAlarmID_t *caID);
    SPxErrorCode DeleteConfAlarm(ConfAlarmID_t caID);
    SPxErrorCode SetConfAlarmName(ConfAlarmID_t caID, const char *name);
    SPxErrorCode GetConfAlarmName(ConfAlarmID_t caID, char *name, unsigned int bufLen);
    SPxErrorCode SetConfAlarmState(ConfAlarmID_t caID, SPxAlarmConfig::State_t state);
    SPxErrorCode GetConfAlarmState(ConfAlarmID_t caID, SPxAlarmConfig::State_t *state);

    /* Configured alarm conditions. */
    SPxErrorCode GetNumConditions(ConfAlarmID_t caID, unsigned int *count);
    SPxErrorCode GetCondition(ConfAlarmID_t caID, unsigned int index, ConditionID_t *condID);
    SPxErrorCode CreateCondition(ConfAlarmID_t caID, SPxCondition::Type_t type, ConditionID_t *condID);
    SPxErrorCode DeleteCondition(ConditionID_t condID);
    SPxErrorCode GetConditionType(ConditionID_t condID, SPxCondition::Type_t *type);
    SPxErrorCode SetConditionArg(ConditionID_t condID, const char *arg);
    SPxErrorCode GetConditionArg(ConditionID_t condID, char *arg, unsigned int bufLen);
    SPxErrorCode SetConditionArg(ConditionID_t condID, double arg);
    SPxErrorCode GetConditionArg(ConditionID_t condID, double *arg);
    SPxErrorCode SetConditionArgOp(ConditionID_t condID, SPxCondition::Operator_t op);
    SPxErrorCode GetConditionArgOp(ConditionID_t condID, SPxCondition::Operator_t *op);
    SPxErrorCode SetConditionRange(ConditionID_t condID, double metres);
    SPxErrorCode GetConditionRange(ConditionID_t condID, double *metres);
    SPxErrorCode SetConditionRangeOp(ConditionID_t condID, SPxCondition::Operator_t op);
    SPxErrorCode GetConditionRangeOp(ConditionID_t condID, SPxCondition::Operator_t *op);
    SPxErrorCode SetConditionTime(ConditionID_t condID, double seconds);
    SPxErrorCode GetConditionTime(ConditionID_t condID, double *seconds);
    SPxErrorCode SetConditionTimeOp(ConditionID_t condID, SPxCondition::Operator_t op);
    SPxErrorCode GetConditionTimeOp(ConditionID_t condID, SPxCondition::Operator_t *op);
    SPxErrorCode SetConditionEnabled(ConditionID_t condID, int isEnabled);
    SPxErrorCode GetConditionEnabled(ConditionID_t condID, int *isEnabled);
    SPxErrorCode GetConditionBuddy(ConditionID_t condID, ConditionID_t *buddyCondID);

    /* Configured alarm actions. */
    SPxErrorCode GetNumActions(ConfAlarmID_t caID, unsigned int *count);
    SPxErrorCode GetAction(ConfAlarmID_t caID, unsigned int index, ActionID_t *actID);
    SPxErrorCode CreateAction(ConfAlarmID_t caID, SPxAlarmAction::Type_t type, ActionID_t *actID);
    SPxErrorCode DeleteAction(ActionID_t actID);
    SPxErrorCode GetActionType(ActionID_t actID, SPxAlarmAction::Type_t *type);
    SPxErrorCode SetActionEnabled(ActionID_t actID, int isEnabled);
    SPxErrorCode GetActionEnabled(ActionID_t actID, int *isEnabled);
    SPxErrorCode SetActionOption(ActionID_t actID, unsigned int value);
    SPxErrorCode GetActionOption(ActionID_t actID, unsigned int *value);

    /* Triggered alarms. */
    SPxErrorCode GetNumTrigAlarms(unsigned int *count, SPxAlarm::State_t states=SPxAlarm::STATE_ANY);
    SPxErrorCode GetTrigAlarm(unsigned int index, TrigAlarmID_t *taID);
    SPxErrorCode DeleteTrigAlarm(TrigAlarmID_t taID);
    SPxErrorCode DeleteTrigAlarms(SPxAlarm::State_t states=SPxAlarm::STATE_ANY);
    SPxErrorCode SetTrigAlarmState(TrigAlarmID_t taID, SPxAlarm::State_t newState);
    SPxErrorCode SetTrigAlarmStates(SPxAlarm::State_t newState, SPxAlarm::State_t states=SPxAlarm::STATE_ANY);
    SPxErrorCode GetTrigAlarmState(TrigAlarmID_t taID, SPxAlarm::State_t *state);
    SPxErrorCode GetTrigAlarmConf(TrigAlarmID_t taID, ConfAlarmID_t *caID);
    SPxErrorCode GetTrigAlarmTrack1Desc(TrigAlarmID_t taID, char *desc, unsigned int bufLen);
    SPxErrorCode GetTrigAlarmTrack1Name(TrigAlarmID_t taID, char *name, unsigned int bufLen);
    SPxErrorCode GetTrigAlarmTrack2Desc(TrigAlarmID_t taID, char *desc, unsigned int bufLen);
    SPxErrorCode GetTrigAlarmTrack2Name(TrigAlarmID_t taID, char *name, unsigned int bufLen);
    SPxErrorCode GetTrigAlarmCPA(TrigAlarmID_t taID, double *cpaMetresRtn, double *tcpaSecsRtn,
	SPxLatLong_t *startLL1Rtn, SPxLatLong_t *endLL1Rtn,
	SPxLatLong_t *startLL2Rtn, SPxLatLong_t *endLL2Rtn);
    SPxErrorCode GetTrigAlarmTriggers(TrigAlarmID_t taID, char *triggers, unsigned int bufLen);
    SPxErrorCode GetTrigAlarmTriggerTime(TrigAlarmID_t taID, double *secsRtn);
    SPxErrorCode GetTrigAlarmStateChangeTime(TrigAlarmID_t taID, double *secsRtn);

    /* Track groups. */
    SPxErrorCode GetNumTrackGroups(unsigned int *count);
    SPxErrorCode GetTrackGroup(unsigned int index, TrackGroupID_t *tgID);
    SPxErrorCode CreateTrackGroup(TrackGroupID_t *tgID);
    SPxErrorCode DeleteTrackGroup(TrackGroupID_t tgID);
    SPxErrorCode SetTrackGroupName(TrackGroupID_t tgID, const char *name);
    SPxErrorCode GetTrackGroupName(TrackGroupID_t tgID, char *name, unsigned int bufLen);
    SPxErrorCode GetNumTrackGroupDescs(TrackGroupID_t tgID, unsigned int *count);
    SPxErrorCode AddTrackGroupDesc(TrackGroupID_t tgID);
    SPxErrorCode RemoveTrackGroupDesc(TrackGroupID_t tgID, unsigned int index);
    SPxErrorCode SetTrackGroupDesc(TrackGroupID_t tgID, unsigned int index, const char *desc);
    SPxErrorCode GetTrackGroupDesc(TrackGroupID_t tgID, unsigned int index, char *desc, unsigned int bufLen);

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

    /*
     * Private functions.
     */

    /* Alarm Database parameter get functions. */
    SPxErrorCode getParam(const char *param,
                          char *value,
                          unsigned int valueBufLen);

    SPxErrorCode getParam(const char *param,
                          unsigned int suffix1,
                          char *value,
                          unsigned int bufLen);

    SPxErrorCode getParam(const char *param,
                          unsigned int suffix1,
                          unsigned int suffix2,
                          char *value,
                          unsigned int bufLen);

    SPxErrorCode getParam(const char *param,
                          double *value);

    SPxErrorCode getParam(const char *param,
                          unsigned int suffix1,
                          double *value);

    SPxErrorCode getParam(const char *param,
                          unsigned int *value);

    SPxErrorCode getParam(const char *param,
                          unsigned int suffix1,
                          unsigned int *value);

    SPxErrorCode getParam(const char *param,
                          unsigned int suffix1,
                          unsigned int suffix2,
                          unsigned int *value);

    SPxErrorCode getParam(const char *param,
                          unsigned int suffix1,
                          int *value);

    /* Alarm Database parameter set functions. */
    SPxErrorCode setParam(const char *param,
                          const char *value);

    SPxErrorCode setParam(const char *param,
                          unsigned int suffix1,
                          const char *value);

    SPxErrorCode setParam(const char *param,
                          unsigned int suffix1,
                          unsigned int suffix2,
                          const char *value);

    SPxErrorCode setParam(const char *param,
                          unsigned int suffix1,
                          double value);

    SPxErrorCode setParam(const char *param,
                          unsigned int value);

    SPxErrorCode setParam(const char *param,
                          unsigned int suffix1,
                          unsigned int value);

    SPxErrorCode setParam(const char *param,
                          unsigned int suffix1,
                          unsigned int suffix2,
                          unsigned int value);

    SPxErrorCode setParam(const char *param,
                          unsigned int suffix1,
                          int value);

    /* Alarm Database object creation functions. */
    SPxErrorCode createObj(const char *param,
                           unsigned int value,
                           unsigned int *id);

    SPxErrorCode createObj(const char *param,
                           unsigned int suffix1,
                           unsigned int value,
                           unsigned int *id);

    /*
     * Private static functions.
     */

}; /* SPxRemoteAS */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* _SPX_REMOTE_AS_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
