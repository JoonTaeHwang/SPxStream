/*********************************************************************
*
* (c) Copyright 2012 - 2014, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPx Auto Lock class that automatically locks/unlocks
*   an SPxLock (Critical secton or semaphore) on construction/destruction.
*
* Revision Control:
*   02/08/18 v1.9    AGC	Use SPX_FUNCTION_DELETE to hide functions.
*
* Previous Changes:
*   22/06/18 1.8    AGC	Suppress cppcheck warnings.
*   14/10/14 1.7    AGC	Support already locked locks.
*   08/11/13 1.6    AGC	Fix cppcheck warnings.
*   24/10/13 1.5    AGC	Add Leave() function.
*   04/10/13 1.4    AGC	Simplify headers.
*   30/07/13 1.3    AGC	Support taking lock as pointer.
*   01/10/12 1.2    AGC	Add optional enter parameter for constructor.
*   29/03/12 1.1    AGC	Initial Version base on SPxAutoMutex.
**********************************************************************/

#ifndef _SPX_AUTO_LOCK_H
#define _SPX_AUTO_LOCK_H

/* For TRUE/FALSE. */
#include "SPxLibUtils/SPxTypes.h"

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
class SPxLock;

/*
 * SPx Auto Lock class
 */
class SPxAutoLock
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxAutoLock(SPxLock &lock, int tryEnter=FALSE, int enter=TRUE, int locked=FALSE);
    explicit SPxAutoLock(SPxLock *lock, int tryEnter=FALSE, int enter=TRUE, int locked=FALSE);
    virtual ~SPxAutoLock(void);
    void Retain(void) { m_retain = TRUE; }
    int IsLocked(void) { return m_locked; }
    void Leave(void);

private:
    /*
     * Private fields.
     */
    SPxLock& m_lock;
    SPxLock* m_lockPtr;
    int m_havePtr;
    int m_locked;
    int m_retain;

    /*
     * Private functions.
     */
    // cppcheck-suppress uninitMemberVarPrivate
    SPxAutoLock(const SPxAutoLock& other) SPX_FUNCTION_DELETE;
    // cppcheck-suppress operatorEqVarError
    SPxAutoLock& operator=(const SPxAutoLock &) SPX_FUNCTION_DELETE;

}; /* SPxAutoLock */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_AUTO_LOCK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
