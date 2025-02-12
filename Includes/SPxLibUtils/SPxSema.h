/*********************************************************************
*
* (c) Copyright 2012, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*    Header for SPx Semaphore wrapper class.
*
* Revision Control:
*   25/06/18 v1.2    AGC	Correctly suppress cppcheck warning.
*
* Previous Changes:
*   29/03/12 1.1    AGC	Initial version.
**********************************************************************/

#ifndef _SPX_SEMA_H
#define _SPX_SEMA_H

/* Need common definitions */
#include "SPxLibUtils/SPxCommon.h"

/* Need error codes. */
#include "SPxLibUtils/SPxError.h"

/* For base class. */
#include "SPxLibUtils/SPxLock.h"

/* Need semaphores types/functions on linux. */
#ifndef _WIN32
#include <semaphore.h>
#endif

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
 * SPx Semaphore class
 */
class SPxSema : public SPxLock
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxSema(int m_init=FALSE);
    virtual ~SPxSema(void);

    /* Creation. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Initialise(void);

    /* Lock and unlock. */
    virtual SPxErrorCode Enter(void);
    virtual SPxErrorCode TryEnter(void);
    virtual SPxErrorCode Leave(void);

private:
    /*
     * Private fields.
     */
#ifdef	_WIN32
    HANDLE m_sema;		/* Windows semaphore */
#else
    sem_t m_sema;		/* Counting semaphore */
#endif
    int m_created;		/* Have we created the OS object? */
    int m_lockCount;		/* Number of times the semaphore has been locked. */

    /*
     * Private functions.
     */

}; /* SPxSema */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SEMA_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
