/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxUniTrackCondition class, which is used by 
 *   SPxUniTrackFilter to store individual conditions.
 *
 * Revision Control:
 *   03/05/18 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_UNI_TRACK_CONDITION_H
#define _SPX_UNI_TRACK_CONDITION_H

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
class SPxUniTrackFilter;
class SPxUniTrack;
class SPxAreaObj;
class SPxUniTrackDatabase;

/*
 * Define our class.
 */
class SPxUniTrackCondition : public SPxCondition
{
    /* These classes can access our internals directly. */
    friend class SPxUniTrackFilter;

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
     * class can only be instantiated by the SPxUniTrackFilter.
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
    SPxUniTrackCondition(SPxUniTrackFilter *filter);
    virtual ~SPxUniTrackCondition(void);

    /* The following implement functions in the base class. */
    virtual SPxErrorCode reportChangeEvent(ChangeFlags_t flags);

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
    SPxUniTrackFilter *m_filter;    /* Track filter. */

    /*
     * Private functions.
     */

    /* General. */

    /*
     * Private static functions.
     */

}; /* SPxUniTrackCondition */

#endif /* SPX_UNI_TRACK_CONDITION_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
