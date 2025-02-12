/*********************************************************************
*
* (c) Copyright 2007 - 2015, 2018, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPx Event wrapper class.
*
* Revision Control:
*   14/06/22 v1.14   AGC	Support initialisation through constructor.
*
* Previous Changes:
*   02/08/18 1.13   AGC	Use SPX_FUNCTION_DELETE to hide functions.
*   22/06/18 1.12   AGC	Suppress cppcheck warning.
*   26/02/15 1.11   AGC	Fix Linux GetEventObject().
*   07/11/14 1.10   AGC	Add SPxEventWaitAnyForever() function.
*			Add SPxEventWaitAnyTimedMsecs() function.
*			Add SPxEventWaitAllForever() function.
*			Support SPxEventWaitAny/All.., functions on Linux.
*   24/09/14 1.9    AGC	Add SPxEventWaitAllTimedMsecs() function.
*   18/11/13 1.8    AGC	Fix clang warning.
*   08/11/13 1.7    AGC	Fix cppcheck warnings.
*   17/11/11 1.6    AGC	Add private copy-constructor and assignment operator.
*   19/08/11 1.5    AGC	Add GetEventObject() function.
*   15/09/10 1.4    REW	Make destructor virtual.
*   02/07/07 1.3    REW	Add WaitTimedMsecs().
*   08/06/07 1.2    REW	Implementation on linux.
*   07/06/07 1.1    REW	Initial Version
**********************************************************************/

#ifndef _SPX_EVENTS_H
#define _SPX_EVENTS_H

/* Need common definitions */
#include "SPxLibUtils/SPxCommon.h"

/* Need error codes. */
#include "SPxLibUtils/SPxError.h"

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
 * SPx Event class
 */
class SPxEvent
{
private:
	/*
	 * Private fields.
	 */
#ifdef	_WIN32
	HANDLE m_eventSignal;	/* Normal windows event */
#else
	sem_t m_sema;		/* Counting semaphore */
#endif
	int m_created;		/* Have we created the OS object? */

	/*
	 * Private functions.
	 */
	// cppcheck-suppress uninitMemberVarPrivate
	SPxEvent(const SPxEvent&) SPX_FUNCTION_DELETE;
	// cppcheck-suppress operatorEqVarError
	SPxEvent& operator=(const SPxEvent &) SPX_FUNCTION_DELETE;

public:
	/*
	 * Public fields.
	 */

	/*
	 * Public functions.
	 */
	/* Constructor and destructor. */
	explicit SPxEvent(int init=FALSE);
	virtual ~SPxEvent(void);

	/* Creation (avoid name clash with windows call). */
	SPxErrorCode SPxCreateEvent(void);

	/* Setting and waiting. */
	SPxErrorCode SignalEvent(void);
	SPxErrorCode WaitForever(void);
	SPxErrorCode WaitTimedMsecs(unsigned int msecs);

	/* Get platform specific object - be careful. */
#ifdef _WIN32
	SPxErrorCode GetEventObject(HANDLE *handle);
#else
	SPxErrorCode GetEventObject(sem_t **semaphore);
#endif

};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

extern SPxErrorCode SPxEventWaitAnyForever(unsigned int numEvents, 
					   SPxEvent **events);
extern SPxErrorCode SPxEventWaitAnyTimedMsecs(unsigned int numEvents, 
					      SPxEvent **events,
					      unsigned int msecs);
extern SPxErrorCode SPxEventWaitAllForever(unsigned int numEvents, 
					   SPxEvent **events);
extern SPxErrorCode SPxEventWaitAllTimedMsecs(unsigned int numEvents,
					      SPxEvent **events,
					      unsigned int msecs);


#endif /* _SPX_EVENTS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
