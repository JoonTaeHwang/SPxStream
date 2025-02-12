/*********************************************************************
*
* (c) Copyright 2014, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*    Header for SPx Condition Variable wrapper class.
*
* Revision Control:
*   22/06/18 v1.1    AGC	Fix new cppcheck warning.
*
* Previous Changes:
*   07/11/14 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_COND_VAR_H
#define _SPX_COND_VAR_H

/* Need common definitions */
#include "SPxLibUtils/SPxCommon.h"

/* Need error codes. */
#include "SPxLibUtils/SPxError.h"

/* Need thread types/functions on linux. */
#ifndef _WIN32
#include <pthread.h>
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

/* Forward declarations. */
class SPxCriticalSection;

/*
 * SPx Condition Variable class
 */
class SPxCondVar
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxCondVar(int m_init=FALSE);
    virtual ~SPxCondVar(void);

    /* Creation. */
    SPxErrorCode Initialise(void);

    SPxErrorCode SignalOne(void);
    SPxErrorCode SignalAll(void);
    SPxErrorCode WaitForever(SPxCriticalSection &mutex);
    SPxErrorCode WaitTimedMsecs(SPxCriticalSection &mutex, unsigned int msecs);

private:
    /*
     * Private fields.
     */
#ifdef	_WIN32
    int m_waitingCount;
    CRITICAL_SECTION m_waitingCountMutex;
    HANDLE m_sema;		/* Windows semaphore */
    HANDLE m_event;
    int m_notifyAll;
#else
    pthread_cond_t m_cond;	/* Condition variable */
#endif
    int m_created;

    /*
     * Private functions.
     */

}; /* SPxCondVar */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

extern SPxErrorCode SPxCondVarWaitAnyForever(SPxCriticalSection &mutex,
					     unsigned int numCondVars,
					     SPxCondVar **condVars);
extern SPxErrorCode SPxCondVarWaitAnyTimedMsecs(SPxCriticalSection &mutex,
						 unsigned int numCondVars,
						 SPxCondVar **condVars,
						 unsigned int msecs);

#endif /* _SPX_COND_VAR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
