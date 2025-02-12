/*********************************************************************
 *
 * (c) Copyright 2017 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmCondition class, which is used by the 
 *   SPxAlarmConfig to store individual conditions.
 *
 * Revision Control:
 *   12/05/21 v1.16  SP 	Rework to move CPA and proximity tests to SPxCondition.
 *
 * Previous Changes:
 *   12/05/21 1.15  SP 	Support TYPE_PROXIMITY_TO_ANY_TRACK.
 *   18/09/20 1.14  AGC	Add required forward declaration.
 *   14/05/18 1.13  SP 	Move setting polyline gate reference
 *                     	count to base class.
 *   08/05/18 1.12  SP 	Override setArgShape().
 *   03/05/18 1.11  SP 	Derive from SPxCondition.
 *   25/04/18 1.10  SP 	Add TYPE_GATE_CROSSED.
 *   11/12/17 1.9   SP 	Support forward and reverse gate crossings.
 *                     	Support CPA conditions.
 *                     	Support track name argument.
 *   24/11/17 1.8   SP 	Support more specific arg types.
 *   22/11/17 1.7   SP 	Support gate crossing condition.
 *   17/11/17 1.6   SP 	Support file load and save.
 *   13/11/17 1.5   SP 	Rename setType() to initialise().
 *   08/11/17 1.4   SP 	Further development.
 *   03/11/17 1.3   SP 	Use SPxAreaObj for areas.
 *   01/11/17 1.2   SP 	Rename to SPxAlarmCondition.
 *   31/10/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_CONDITION_H
#define _SPX_ALARM_CONDITION_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
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
class SPxAlarmConfig;
class SPxUniTrack;
class SPxAreaObj;
class SPxUniTrackDatabase;
class SPxAlarmDatabase;

/*
 * Define our class.
 */
class SPxAlarmCondition : public SPxCondition
{
    /* These classes can access our internals directly. */
    friend class SPxAlarmConfig;

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
     * class can only be instantiated by the SPxAlarmConfig.
     */

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
    SPxAlarmCondition(SPxAlarmConfig *config);
    virtual ~SPxAlarmCondition(void);

    /* The following implement/override functions in the base class. */
    virtual SPxErrorCode reportChangeEvent(ChangeFlags_t flags);
    virtual int canTrackTriggerAlarm(SPxUniTrack *track);
    virtual int hasTrackTriggeredAlarm(SPxUniTrack *track);

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
    SPxAlarmConfig *m_config;       /* Alarm config. */
    SPxAlarmDatabase *m_alarmDB;    /* Alarm database. */

    /*
     * Private functions.
     */

    /* General. */

    /*
     * Private static functions.
     */

}; /* SPxAlarmCondition */

#endif /* SPX_ALARM_CONDITION_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
