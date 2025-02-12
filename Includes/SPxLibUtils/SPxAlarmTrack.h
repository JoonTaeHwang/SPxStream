/*********************************************************************
 *
 * (c) Copyright 2017 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmTrack class, which is used by the 
 *   SPxAlarmDatabase to store track specific alarm data.
 *
 * Revision Control:
 *   10/02/21 v1.17   AGC	Support option for tracks being red when alarming.
 *				Update track cleared alarm count.
 *
 * Previous Changes:
 *   08/06/20 1.16  BTB	Only override colours when alarming.
 *   19/12/19 1.15  BTB	Support making alarming tracks bold.
 *   04/01/19 1.14  BTB	Handle network messages per alarm not per alarm track.
 *   25/06/18 1.13  AGC	Correctly suppress cppcheck warning.
 *   24/05/18 1.12  SP 	Add mutex protection.
 *   23/05/18 1.11  SP 	Add updateNetMesgRefCount().
 *                      Make some functions protected.
 *   01/05/18 1.10  SP 	Move area and gate logic to SPxUniTrack.
 *   23/11/17 1.9   SP 	Set track colour based on alarm state.
 *   22/11/17 1.8   SP 	Add support for gates using SPxAreaObj.
 *   17/11/17 1.7   SP 	Add UpdateHighlightRefCount().
 *   13/11/17 1.6   SP 	Add UpdateAlarmRefCount().
 *   03/11/17 1.5   SP 	Use SPxAreaObj for areas.
 *   01/11/17 1.4   SP 	Rework area logic.
 *   31/10/17 1.3   SP 	Rework to support alarm clauses.
 *   25/10/17 1.2   SP 	Use SPxAreaObj for areas.
 *                      Implement area alarms.
 *   18/10/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_TRACK_H
#define _SPX_ALARM_TRACK_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"

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

/* Forward-declare other classes we use in case headers are in wrong order. */
class SPxAlarmDatabase;
class SPxUniTrack;

/*
 * Define our class.
 */
class SPxAlarmTrack : public SPxObj
{
    /* These classes can access our internals directly. */
    friend class SPxAlarmDatabase;
    friend class SPxAlarm;
    friend class SPxAlarmAction;

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

    /* Access. */
    virtual SPxUniTrack *GetTrack(void) const { return(m_track); }

    /* Alarm handling. */
    virtual int GetAlarmRefCount(void) const { return m_alarmRefCount; }
    virtual int GetHighlightRefCount(void);

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
    SPxAlarmTrack(SPxAlarmDatabase *alarmDB, SPxUniTrack *track,
		  int boldOnAlarm=FALSE,
		  int redOnAlarm=TRUE);
    virtual ~SPxAlarmTrack(void);

    /* General. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode reset(void);

    /* Alarm handling. */
    virtual SPxErrorCode updateAlarmRefCount(int add);
    virtual SPxErrorCode updateClearedAlarmRefCount(int add);
    virtual SPxErrorCode updateHighlightRefCount(void *alarm, int add,
						 UINT32 timeoutSecs);

private:

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxCriticalSection m_mutex;         /* Mutex. */
    SPxAlarmDatabase *m_alarmDB;        /* Alarm database . */
    SPxUniTrack *m_track;               /* Associated track. */
    int m_boldOnAlarm;                  /* Whether track should be bold when alarming. */
    int m_redOnAlarm;                   /* Whether track should be red when alarming. */
    int m_alarmRefCount;                /* Alarm reference count. */
    int m_clearedAlarmRefCount;		/* Cleared alarm reference count. */

    /*
     * Private functions.
     */
    void purgeExpiredHighlights(UINT32 now);

    /*
     * Private static functions.
     */

}; /* SPxAlarmTrack */

#endif /* SPX_ALARM_TRACK_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
