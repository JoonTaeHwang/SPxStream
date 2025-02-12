/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPromise.h,v $
* ID: $Id: SPxPromise.h,v 1.1 2014/11/07 14:42:47 rew Exp $
*
* Purpose:
*   Header for SPxPromise object that represents the result of an
*   operation that may not have completed.
*
* Revision Control:
*   07/11/14 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_PROMISE_H
#define _SPX_PROMISE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoLock.h"
#include "SPxLibUtils/SPxCondVar.h"
#include "SPxLibUtils/SPxFuture.h"
#include "SPxLibUtils/SPxFutureState.h"
#include "SPxLibUtils/SPxSharedPtr.h"
#include "SPxLibUtils/SPxTypes.h"

/* Generic version of SPxPromise class. */
template<typename T>
class SPxPromise
{
public:
    SPxPromise() : m_state(SPxMakeShared<SPxFutureState<T> >()) {}
    ~SPxPromise() {}

    /* The value is now ready. */
    void set_value(typename SPxAddCrefIfNotRef<T>::type value)
    {
	SPxAutoLock lock(m_state->mutex);
	m_state->value = value;
	m_state->ready = true;
	m_state->cond.SignalAll();
    }

    /* Get the future associated with this promise. */
    SPxFuture<T> get_future()
    {
	return SPxFuture<T>(m_state);
    }

private:
    SPxSharedPtr<SPxFutureState<T> > m_state;
};

/* Specialisation for when the result is void. */
template<>
class SPxPromise<void>
{
public:
    SPxPromise() : m_state(SPxMakeShared<SPxFutureState<void> >()) {}
    ~SPxPromise() {}

    /* The operation has now completed. */
    void set_value()
    {
	SPxAutoLock lock(m_state->mutex);
	m_state->ready = true;
	m_state->cond.SignalAll();
    }

    /* Get the future associated with this promise. */
    SPxFuture<void> get_future()
    {
	return SPxFuture<void>(m_state);
    }

private:
    SPxSharedPtr<SPxFutureState<void> > m_state;
};

#endif /* _SPX_PROMISE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
