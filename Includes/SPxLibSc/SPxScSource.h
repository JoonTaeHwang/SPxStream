/*********************************************************************
*
* (c) Copyright 2007, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScSource.h,v $
* ID: $Id: SPxScSource.h,v 1.6 2007/06/18 10:01:28 rew Exp $
*
* Purpose:
*   Header for base SPxScSource class.
*
* Revision Control:
*   18/06/07 v1.6    REW	Remove repeated fade definitions (see SPxSc.h)
*
* Previous Changes:
*   13/06/07 1.5    DGJ	Changed fading enum values.
*   13/06/07 1.4    REW	Remove unused code.
*   08/06/07 1.3    DGJ	Reworked/renamed SPxSc class hierarchy.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*   11/04/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPxScSource_h
#define _SPxScSource_h

/*
 * Other headers required.
 */
/* Our object is derived from the base source class. */
#include "SPxLibSc/SPxSc.h"

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

/* Forward declare any classes we need in our own class in case
 * headers are included in the wrong order.
 */


/*
 * Define the base class for sources of scan converted data.
 *
 * This is derived from the SPxSc class which is the base of all scan
 * conversion classes.
 */
class SPxScSource :public SPxSc
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxScSource(void);
    virtual ~SPxScSource(void);      

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */
}; /* SPxScSource */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPxScSource_h */
/*********************************************************************
*
* End of file
*
**********************************************************************/
