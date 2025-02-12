/*********************************************************************
*
* (c) Copyright 2012 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLock.h,v $
* ID: $Id: SPxLock.h,v 1.4 2016/02/12 14:03:31 rew Exp $
*
* Purpose:
*   Header for SPx Lock abstract base class for lockable objects.
*
* Revision Control:
*   12/02/16 v1.4    AGC	Add default implementations of Initialise() and TryEnter().
*
* Previous Changes:
*   18/11/13 1.3    AGC	Fix clang warning.
*   04/10/13 1.2    AGC	Simplify headers.
*   29/03/12 1.1    REW	Initial Version
**********************************************************************/

#ifndef _SPX_LOCK_H
#define _SPX_LOCK_H

/* Need error codes. */
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * SPx Lock class
 */
class SPxLock
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxLock(void) {}
    virtual ~SPxLock(void) {}

    /* Initialisation */
    virtual SPxErrorCode Initialise(void) { return SPX_NO_ERROR; }

    /* Entering and leaving */
    virtual SPxErrorCode Enter(void)=0;
    virtual SPxErrorCode TryEnter(void) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode Leave(void)=0;

private:
    /* Prevent copying. */
    SPxLock(const SPxLock&) {}
    SPxLock& operator=(const SPxLock&) { return *this; }

}; /* SPxLock */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_LOCK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
