/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPx Critical Section wrapper class.
*
* Revision Control:
*   08/08/22 v1.13   AGC	Revert last change as no longer required.
*
* Previous Changes:
*   08/08/22 1.12   AGC	Remove dependency on SPxError translation unit.
*   25/06/18 1.11   AGC	Correctly suppress cppcheck warning.
*   22/04/15 1.10   SP 	Add IsInitialised().
*   07/11/14 1.9    AGC	Support using mutex on Windows.
*   04/10/13 1.8    AGC	Simplify headers.
*   29/03/12 1.7    AGC	Derive from new base class SPxLock.
*			Add TryEnter() function.
*   27/06/11 1.6    AGC	Add optional parameter to constructor,
*			that if TRUE, initialises the critical
*			section immediately.
*   15/09/10 1.5    REW	Make destructor virtual.
*   20/05/08 1.4    REW	Add sanity check 'm_locked' flag.
*   03/07/07 1.3    REW	Warn about imminent deadlock on linux.
*   08/06/07 1.2    REW	Implementation on linux.
*   07/06/07 1.1    REW	Initial Version
**********************************************************************/

#ifndef _SPX_CRITICALSECTION_H
#define _SPX_CRITICALSECTION_H

/* Need platform types. */
#include "SPxLibUtils/SPxCommon.h"

/* Need error codes. */
#include "SPxLibUtils/SPxError.h"

/* For base class. */
#include "SPxLibUtils/SPxLock.h"

/* Need mutex types/functions on linux. */
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

/*
 * SPx Critical Section class
 */
class SPxCriticalSection : public SPxLock
{
private:
	/*
	 * Private fields.
	 */
#ifdef	_WIN32
	CRITICAL_SECTION m_criticalSection;
	HANDLE m_mutex;
#else
	pthread_mutex_t m_mutex;	/* The mutex */
	pthread_t m_lockedBy;		/* Thread that has locked mutex */
#endif
	int m_initialised;		/* Flag to say initialised */
	int m_locked;			/* Flag to say already locked */
	int m_isMutex;			/* Flag indicating internal object
					 * is mutex rather than critical section.
					 */

	/*
	 * Private functions.
	 */

public:
	/*
	 * Public fields.
	 */

	/*
	 * Public functions.
	 */
	/* Constructor and destructor. */
	explicit SPxCriticalSection(int init=FALSE, int isMutex=FALSE);
	virtual ~SPxCriticalSection(void);

	/* Initialisation */
	// cppcheck-suppress virtualCallInConstructor
	virtual SPxErrorCode Initialise(void);
	virtual int IsInitialised(void) { return m_initialised; }

	/* Entering and leaving */
	virtual SPxErrorCode Enter(void);
	virtual SPxErrorCode TryEnter(void);
	virtual SPxErrorCode Leave(void);

	/* Get platform specific object - be careful. */
#ifdef _WIN32
	SPxErrorCode GetObject(CRITICAL_SECTION **handle);
	SPxErrorCode GetObject(HANDLE *handle);
#else
	SPxErrorCode GetObject(pthread_mutex_t **handle);
#endif

	friend class SPxCondVar;
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_CRITICALSECTION_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
