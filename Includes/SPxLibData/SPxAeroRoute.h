/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAeroRoute class.
*
* Revision Control:
*   26/10/20 v1.2    REW	Derive from SPxObj etc.
*
* Previous Changes:
*   23/10/20 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AERO_ROUTE_H
#define _SPX_AERO_ROUTE_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxViewControl.h"	/* For SPxLatLong_t */

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward-declare other classes we use. */
class SPxAeroDb;
class SPxAeroNavAid;
class SPxCriticalSection;

/*
 * Define our class.  It would be overkill to derive from SPxObj.
 */
class SPxAeroRoute : public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxAeroRoute(SPxAeroDb *db);
    virtual ~SPxAeroRoute(void);

    /* Route setting functions. */
    SPxErrorCode SetRoute(const char *str);

    /* Force recalcuation even if route not changed (e.g. db has). */
    SPxErrorCode Recalculate(void);

    /* Retrieval functions. */
    const char *GetRoute(void) const	{ return(m_routeStr); }
    int IsComplete(void) const		{ return(m_isComplete); }
    unsigned int GetNumPoints(void) const;
    SPxErrorCode GetPoint(unsigned int idx, SPxLatLong_t *latLong) const;

    /* Closest distance to route. */
    SPxErrorCode GetDistanceFromRoute(const SPxLatLong_t *latLong,
					double *metresPtr) const;


private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Mutex protection. */
    SPxCriticalSection *m_mutex;	/* Protection mutex  */

    /* Links to other objects. */
    SPxAeroDb *m_db;		/* Database to look up points in */

    /* String describing the route. */
    char *m_routeStr;		/* List of points and corridors (by name). */

    /* Flags/status etc. */
    int m_isComplete;		/* Is the current route error free? */

    /*
     * Private functions.
     */
    /* Internal recalculation (mutex must be locked around it). */
    SPxErrorCode recalculateLocked(void);

    
}; /* SPxAeroRoute */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_AERO_ROUTE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
