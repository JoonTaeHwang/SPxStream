/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxUniTrackFilter class, used to define
 *   a track filter.
 *
 * Revision Control:
 *   09/11/18 v1.3   REW	Make exclusion control public.
 *
 * Previous Changes:
 *   30/05/18 1.2   REW	Support exclusion filters.
 *   03/05/18 1.1   SP 	Initial version.
 *********************************************************************/

#ifndef _SPX_UNI_TRACK_FILTER_H
#define _SPX_UNI_TRACK_FILTER_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxConditionGroup.h"
#include "SPxLibNet/SPxUniTrackCondition.h"

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
class SPxUniTrackFilter : public SPxConditionGroup
{
    /* These classes can access our internals directly. */
    friend class SPxUniTrackDatabase;

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
     * class can only be instantiated by the parent.
     */

    /* Filter mode (exclusion or inclusion). */
    void SetIsExclusionFilter(int exclusion);
    int GetIsExclusionFilter(void) const;	/* TRUE or FALSE */

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

    /* Protected functions may be accessed by the SPxUniTrackDatabase. */

    /* Constructor and destructor. */
    SPxUniTrackFilter(SPxUniTrackDatabase *trackDB);
    virtual ~SPxUniTrackFilter(void);

    /* The following implement functions in the base class. */
    virtual SPxErrorCode reportConditionCreateEvent(SPxCondition *condition);
    virtual SPxErrorCode reportConditionDeleteEvent(SPxCondition *condition);
    virtual SPxErrorCode reportChangeEvent(ChangeFlags_t flags);

    /* The following override functions in the base class. */
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

    /*
     * Private functions.
     */

    /* General. */

    /*
     * Private static functions.
     */

}; /* SPxUniTrackFilter */

#endif /* SPX_UNI_TRACK_FILTER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
