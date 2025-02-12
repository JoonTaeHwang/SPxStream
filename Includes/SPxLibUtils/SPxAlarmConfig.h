/*********************************************************************
 *
 * (c) Copyright 2017 - 2018, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmConfig class, which is used by the 
 *   SPxAlarmDatabase to store configured alarms.
 *
 * Revision Control:
 *   08/04/21 v1.18  SP 	Support action ID.
 *
 * Previous Changes:
 *   25/06/18 1.17  AGC	Correctly suppress cppcheck warning.
 *   24/05/18 1.16  SP 	Move action processing to SPxAlarm.
 *   22/05/18 1.15  SP 	Support changes to SPxAlarmAction for action options.
 *   03/05/18 1.14  SP 	Pass CPA track to startStopActions().
 *   03/05/18 1.13  SP 	Derive from SPxConditionGroup.
 *   13/12/17 1.12  SP 	Remove unused parameters.
 *   11/12/17 1.11  SP 	Changes to support CPA alarms.
 *   22/11/17 1.10  SP 	Add support for gates using SPxAreaObj.
 *   17/11/17 1.9   SP 	Support file load and save.
 *   13/11/17 1.8   SP 	Further support for alarm actions.
 *   08/11/17 1.7   SP 	Add support for alarm actions.
 *   03/11/17 1.6   SP 	Use SPxAreaObj for areas.
 *   01/11/17 1.5   SP 	Support renamed class.
 *   31/10/17 1.4   SP 	Rework to support alarm clauses.
 *   25/10/17 1.3   SP 	Add SetOwnerName() and IsMatch().
 *   23/10/17 1.2   SP 	Further development.
 *   18/10/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_CONFIG_H
#define _SPX_ALARM_CONFIG_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxConditionGroup.h"
#include "SPxLibUtils/SPxAlarmCondition.h"
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
class SPxAlarmDatabase;
class SPxUniTrack;

/*
 * Define our class.
 */
class SPxAlarmConfig : public SPxConditionGroup
{
    /* These classes can access our internals directly. */
    friend class SPxAlarmDatabase;
    friend class SPxAlarmCondition;
    friend class SPxAlarmAction;
    friend class SPxAlarm;

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

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by the SPxAlarmDatabase.
     */

    /* Configuration. */
    virtual SPxAlarmDatabase *GetAlarmDB(void) { return m_alarmDB; }

    /* Actions. */
    virtual SPxAlarmAction *CreateAction(SPxAlarmAction::Type_t type,
                                         SPxErrorCode *errRtn=NULL);
    virtual SPxAlarmAction *FindAction(SPxAlarmAction::Type_t type);
    virtual unsigned int GetNumActions(void);
    virtual SPxAlarmAction *GetActionFromIndex(unsigned int index);
    virtual SPxAlarmAction *GetActionFromID(UINT32 id);
    virtual SPxErrorCode IterateActions(SPxCallbackListFn_t fn, void *userArg);
    virtual SPxErrorCode DeleteAction(SPxAlarmAction *action);
    virtual SPxErrorCode DeleteAllActions(void);
    virtual const char *GetActionsSummaryString(void);

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

    /* Constructor and destructor. */
    SPxAlarmConfig(SPxAlarmDatabase *alarmDB);
    virtual ~SPxAlarmConfig(void);

    /* The following implement functions in the base class. */
    virtual SPxErrorCode reportConditionCreateEvent(SPxCondition *condition);
    virtual SPxErrorCode reportConditionDeleteEvent(SPxCondition *condition);
    virtual SPxErrorCode reportChangeEvent(ChangeFlags_t flags);

    /* The following override functions in the base class. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode reset(void);
    virtual SPxCondition *getNewCondition(void);

    /* Config load/save. */
    virtual SPxErrorCode load(FILE *fPtr);
    virtual SPxErrorCode save(FILE *fPtr);

private:

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    mutable SPxCriticalSection m_mutex; /* Mutex. */
    SPxAlarmDatabase *m_alarmDB;        /* Alarm database . */

    /*
     * Private functions.
     */

    /* General. */

    /* Actions. */
    SPxAlarmAction *getNewAction(void);
    SPxErrorCode setActionInUse(SPxAlarmAction *action, int isInUse);
    SPxAlarmAction *createAction(SPxAlarmAction::Type_t type,
                                 SPxErrorCode *errRtn);
    SPxErrorCode deleteAction(SPxAlarmAction *action);
    SPxAlarmAction *findAction(SPxAlarmAction::Type_t type);
    void updateActionsSummaryString(void);

    /*
     * Private static functions.
     */

}; /* SPxAlarmConfig */

#endif /* SPX_ALARM_CONFIG_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
