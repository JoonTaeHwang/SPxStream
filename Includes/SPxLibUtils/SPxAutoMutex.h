/*********************************************************************
*
* (c) Copyright 2011, 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAutoMutex.h,v $
* ID: $Id: SPxAutoMutex.h,v 1.4 2013/11/18 17:03:12 rew Exp $
*
* Purpose:
*   Header for SPx Auto Mutex class that automatically locks/unlocks
*   an SPxCriticalSection on construction/destruction.
*
* Revision Control:
*   18/11/13 v1.4    AGC	Fix clang warning.
*
* Previous Changes:
*   29/03/12 1.3    AGC	Derive from new base class SPxAutoLock.
*   17/11/11 1.2    AGC	Use standard form for assignment function.
*   27/06/11 1.1    AGC	Initial Version
**********************************************************************/

#ifndef _SPX_AUTO_MUTEX_H
#define _SPX_AUTO_MUTEX_H

/* Need SPxCriticalSection class. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For base class. */
#include "SPxLibUtils/SPxAutoLock.h"

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

/* Foward declarations. */

/*
 * SPx Auto Mutex class
 */
class SPxAutoMutex : public SPxAutoLock
{
public:
    explicit SPxAutoMutex(SPxCriticalSection &mutex) : SPxAutoLock(mutex) {}
    virtual ~SPxAutoMutex(void) {}

}; /* SPxAutoMutex */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_AUTO_MUTEX_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
