/*********************************************************************
 *
 * (c) Copyright 2018, 2020, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxConditionGroup class, which is used to hold
 *   and process SPxConditions.
 *
 * Revision Control:
 *   05/05/21 v1.14  SP 	No longer need to handle track delete.
 *
 * Previous Changes:
 *   16/04/21 1.13  SP 	Make operator specific to argument.
 *   16/04/21 1.12  SP 	Support range and time limit operators.
 *   13/04/21 1.11  SP 	Rename operation to operator.
 *   08/04/21 1.10  SP 	Support condition ID.
 *   10/02/21 1.9   AGC	Support setting arg string during condition create.
 *   08/10/20 1.8   SP 	Add RESTRICT_SAVE flag.
 *   08/10/20 1.7   SP 	Support restrictions.
 *   14/11/18 1.6   BTB	Support shared condition groups.
 *   12/11/18 1.5   BTB	Handle reloading and deleting all shapes.
 *   26/09/18 1.4   SP 	Do background processing from timer.
 *   25/06/18 1.3   AGC	Correctly suppress cppcheck warning.
 *   18/06/18 1.2   SP 	Add optional arg to isConditionOperationValid().
 *   03/05/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CONDITION_GROUP_H
#define _SPX_CONDITION_GROUP_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxCondition.h"

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
class SPxUniTrackDatabase;
class SPxUniTrack;

/*
 * Define our class.
 */
class SPxConditionGroup : public SPxObj
{
    /* These classes can access our internals directly. */
    friend class SPxCondition;

public:
    /*
     * Public types.
     */

    /* States. */
    typedef UINT32 State_t;
    static const UINT32 STATE_NOT_CONFIGURED    = 0x00000001;
    static const UINT32 STATE_DISABLED          = 0x00000002;
    static const UINT32 STATE_ENABLED           = 0x00000004;
    static const UINT32 STATE_ANY               = 0xFFFFFFFF;

    /* Change flags. */
    typedef UINT32 ChangeFlags_t;
    static const UINT32 CHANGE_DISPLAY_NAME     = 0x00000001;
    static const UINT32 CHANGE_STATE            = 0x00000002;
    static const UINT32 CHANGE_OWNER            = 0x00000004;
    static const UINT32 CHANGE_SUBTYPE          = 0x00000008;

    /* Restriction flags. */
    typedef UINT32 RestrictionFlags_t;
    static const UINT32 RESTRICT_SAVE               = 0x00000001;
    static const UINT32 RESTRICT_DELETE             = 0x00000002;
    static const UINT32 RESTRICT_RENAME             = 0x00000004;
    static const UINT32 RESTRICT_STATE              = 0x00000008;
    static const UINT32 RESTRICT_CREATE_CONDITION   = 0x00000010;
    static const UINT32 RESTRICT_DELETE_CONDITION   = 0x00000020;
    static const UINT32 RESTRICT_ALL                = 0xFFFFFFFF;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by a friend class.
     */

    /* Configuration. */
    virtual SPxUniTrackDatabase *GetTrackDB(void) { return m_trackDB; }
    virtual SPxTime_t GetResetTime(void) const { return m_resetTime; }

    virtual UINT32 GetID(void) { return m_id; }

    virtual SPxErrorCode SetDisplayName(const char *name);
    virtual const char *GetDisplayName(void);

    virtual SPxErrorCode SetState(State_t state);
    virtual State_t GetState(void) const { return m_state; }
    virtual const char *GetStateString(void);

    void SetIsShared(int isShared) { m_isShared = isShared; }
    int GetIsShared(void) { return m_isShared; }
    virtual const char *GetSharedString(void);

    virtual SPxErrorCode SetRestrictions(RestrictionFlags_t flags);
    virtual RestrictionFlags_t GetRestrictions(void) { return m_restrictions; }
    virtual int IsRestricted(RestrictionFlags_t flags);

    /* Conditions. */
    virtual int CanConditionBeCreated(SPxCondition::Type_t type);
    virtual int IsConditionArgOperatorValid(SPxCondition::Type_t type,
                                            SPxCondition::Operator_t op);
    virtual SPxCondition *CreateCondition(SPxCondition::Type_t type,
                                          SPxErrorCode *errRtn=NULL,
                                          const char *argString=NULL);
    virtual unsigned int GetNumConditions(void);
    virtual SPxCondition *GetConditionFromIndex(unsigned int index);
    virtual SPxCondition *GetConditionFromID(UINT32 ID);
    virtual SPxErrorCode IterateConditions(SPxCallbackListFn_t fn, void *userArg);
    virtual SPxErrorCode DeleteCondition(SPxCondition *condition);
    virtual SPxErrorCode DeleteAllConditions(void);
    virtual const char *GetConditionsSummaryString(void);

    /*
     * Public static functions.
     */

    static const char *GetStateString(State_t state);

    static const char *GetSharedString(int isShared);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Protected functions may be accessed by the SPxUniTrackDatabase. */

    /* Constructor and destructor. */
    SPxConditionGroup(SPxUniTrackDatabase *trackDB);
    virtual ~SPxConditionGroup(void);

    /* General. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode reset(void);
    virtual SPxErrorCode initialise(const char *prefix);
    virtual SPxErrorCode updateState(void);
    virtual int testConditions(SPxUniTrack *track);
    virtual unsigned int getNumTriggers(void);
    virtual SPxErrorCode getTrigger(unsigned int index, 
                                    SPxCondition::Trigger_t *triggerRtn);
    virtual void handleShapeDelete(SPxAreaObj *area);
    virtual void handleShapeDeleteAll(void);
    virtual void handleShapeReloadAll(void);
    virtual void updateDisarmed(void);

    /* Config load/save. */
    virtual SPxErrorCode loadGroup(FILE *fPtr, 
                                   const char *line, 
                                   const char *keyword, 
                                   const char *arg);
    virtual SPxErrorCode saveGroup(FILE *fPtr);
    virtual SPxErrorCode saveState(FILE *fPtr);

    /* Implement these functions in a derived class. */
    virtual SPxErrorCode reportConditionCreateEvent(SPxCondition *condition)=0;
    virtual SPxErrorCode reportConditionDeleteEvent(SPxCondition *condition)=0;
    virtual SPxErrorCode reportChangeEvent(ChangeFlags_t flags)=0;

    /* Override these functions in a derived class. */
    virtual SPxCondition *getNewCondition(void);

    /*
     * Protected static functions.
     */

    static void resetNextID(void);

private:

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General .*/
    mutable SPxCriticalSection m_mutex;     /* Mutex. */
    SPxTimer m_timer;                       /* Background timer. */
    SPxUniTrackDatabase *m_trackDB;         /* Track database. */
    UINT32 m_id;                            /* Group ID. */
    SPxTime_t m_resetTime;                  /* Last reset time. */
    State_t m_state;                        /* State. */
    int m_isShared;                         /* Is this a shared condition? */
    RestrictionFlags_t m_restrictions;      /* Restriction flags. */

    /*
     * Private functions.
     */

    /* General. */
    int isConfigValid(void);
    SPxErrorCode handleChange(ChangeFlags_t flags);
    SPxErrorCode setState(State_t state);

    /* Conditions. */
    SPxErrorCode setConditionInUse(SPxCondition *condition, int isInUse);
    SPxCondition *createCondition(SPxCondition::Type_t type,
                                  SPxErrorCode *errRtn,
                                  const char *argString);
    SPxErrorCode deleteCondition(SPxCondition *condition);
    SPxErrorCode deleteConditionAndBuddy(SPxCondition *condition);
    void addTrigger(const SPxCondition::Trigger_t *trigger);
    void updateConditionsSummaryString(void);
    int isConditionArgOperatorValid(SPxCondition::Type_t type,
                                    SPxCondition::Operator_t op,
                                    SPxCondition *excCond=NULL);

    /*
     * Private static functions.
     */

    static void doBackgroundProcessing(void * userArg);

}; /* SPxConditionGroup */

#endif /* SPX_CONDITION_GROUP_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
