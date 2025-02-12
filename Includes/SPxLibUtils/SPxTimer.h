/*********************************************************************
*
* (c) Copyright 2011 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for object that allows a function to be run 
*   repeatably at a fixed intervals.
*
* Revision Control:
*   16/11/20 v1.24   AGC	Support reset of timing.
*
* Previous Changes:
*   22/10/20 1.23   AGC	Add IsScheduled().
*   01/08/18 1.22   AGC	Add optional debug.
*   26/06/18 1.21   AGC	Fix cppcheck suppression.
*   25/06/18 1.20   AGC	Correctly suppress cppcheck warning.
*   15/06/18 1.19   AGC	Support setting thread name.
*   09/05/17 1.18   AGC	Fix cppcheck warnings.
*   24/04/17 1.17   AGC	Add Shutdown() function for fixing #449.
*   12/12/16 1.16   AGC	Support calling Schedule() from within handler.
*   21/09/16 1.15   AGC	Support use with external mutex.
*   10/07/15 1.14   AGC	Prevent crash if handler removes itself.
*   29/04/15 1.13   AGC	Allow external drivers.
*   27/02/15 1.12   AGC	Allow virtual override of static Schedule() function.
*   24/09/14 1.11   AGC	Correct rescheduling of member functions.
*   15/09/14 1.10   AGC	Support member functions.
*   04/07/14 1.9    AGC	Rename private structure.
*   04/10/13 1.8    AGC	Simplify headers.
*   01/10/12 1.7    AGC	Move implementation to cpp file.
*   08/05/12 1.6    AGC	Make SPxTimer destructor virtual.
*   30/04/12 1.5    AGC	Make interface functions virtual.
*   17/11/11 1.4    AGC	Add private copy constructor and assigment operator.
*   03/08/11 1.3    AGC	Use pointer to SPxTimerFunc in container.
*   27/07/11 1.2    AGC	Support Linux.
*			Support non-integer intervals.
*   13/07/11 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_TIMER_H
#define _SPX_TIMER_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxEventHandler.h"
#include "SPxLibUtils/SPxSharedPtr.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxThread;

/* The types of delay available. */
enum SPxTimerDelay
{
    /* The time between the end of one execution of the function 
     * and the start of the next execution of the function is constant. 
     */
    SPX_TIMER_FIXED_DELAY = 0,
    /* Time time between the start of one execution of the funciton
     * and the start of the next execution of the function is constant.
     */
    SPX_TIMER_FIXED_RATE  = 1	
};

class SPxTimer
{
public:
    typedef void(*StaticFunc)(void*);
    typedef void(*MemFunc)(void);
    typedef SPxEventHandler<MemFunc> Handler;
    typedef SPxSharedPtr<Handler> SharedHandler;

    /* Public functions. */
    explicit SPxTimer(int createThread=TRUE, SPxCriticalSection *externalMutex=NULL);
    virtual ~SPxTimer(void);
    virtual SPxErrorCode Schedule(StaticFunc task, SPxTimerDelay delayType,
			  UINT32 initialDelayMsecs, double intervalMsecs,
			  void *userArg, int inFunc=FALSE, int reset=FALSE)
    {
	return Schedule<StaticFunc, void>(task, delayType, initialDelayMsecs, intervalMsecs,
	    userArg, inFunc, reset);
    }
    template<typename F, typename O>
    SPxErrorCode Schedule(F task, SPxTimerDelay delayType,
			  UINT32 initialDelayMsecs, double intervalMsecs,
			  O *obj, int inFunc=FALSE, int reset=FALSE)
    {
	SharedHandler handler = SPxMakeShared<Handler>();
	handler->Add(task, obj);
	return add(handler, delayType, initialDelayMsecs, intervalMsecs, inFunc, reset);
    }
    SPxErrorCode Unschedule(StaticFunc task, void *userArg, int inFunc=FALSE)
    {
	return Unschedule<StaticFunc, void>(task, userArg, inFunc);
    }
    template<typename F, typename O>
    SPxErrorCode Unschedule(F task, O *obj, int inFunc=FALSE)
    {
	Handler handler;
	handler.Add(task, obj);
	return remove(handler, inFunc);
    }
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode UnscheduleAll(int inFunc=FALSE);

    int IsScheduled(StaticFunc task, void *userArg, int inFunc=FALSE)
    {
	return IsScheduled<StaticFunc, void>(task, userArg, inFunc);
    }
    template<typename F, typename O>
    int IsScheduled(F task, O *obj, int inFunc=FALSE)
    {
	Handler handler;
	handler.Add(task, obj);
	return isScheduled(handler, inFunc);
    }

    void SetThreadName(const char *threadName);

    virtual SPxErrorCode Poll(unsigned int timeoutMsecs,
			      SPxCriticalSection *externalMutex=NULL);

    SPxErrorCode Shutdown(void);

    SPxErrorCode SetVerbosity(UINT8 verbosity);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    static void *threadFnWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);
    SPxErrorCode add(const SharedHandler &handler, SPxTimerDelay delayType,
	UINT32 initialDelayMsecs, double intervalMsecs, int inFunc, int reset);
    SPxErrorCode remove(const Handler &handler, int inFunc);
    int isScheduled(const Handler &handler, int inFunc);

    void
#if defined(__GNUC__) || defined(__clang__)
    __attribute__((format(printf, 3, 4)))
#endif
    writeDebug(UINT8 verbosity, const char *format, ...) const;

    /* Prevent copying. */
    SPxTimer(const SPxTimer&) SPX_FUNCTION_DELETE;
    SPxTimer& operator=(const SPxTimer&) SPX_FUNCTION_DELETE;

}; /* SPxTimer */

#endif /* _SPX_TIMER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
