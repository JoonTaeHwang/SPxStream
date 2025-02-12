/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxFuture.h,v $
* ID: $Id: SPxFuture.h,v 1.3 2014/11/10 16:39:40 rew Exp $
*
* Purpose:
*   Header for SPxFuture object that represents the result of an
*   operation that may not have completed.
*
*   SPxFuture objects are created by SPxPromise objects - specifically
*   the get_future() function.
*
*   SPxFuture objects are also returned by the SPxAsync(), SPxThen()
*   SPxWhenAll() and SPxWhenAny() functions (see SPxAsync.h).
*
*   The result of an SPxFuture object can be retrieved only when it
*   is ready. Calling get() before the result is ready will block
*   until it is ready and then return the result.
*
*   The ready() function will return true iff the result is ready.
*
* Revision Control:
*   10/11/14 v1.3    AGC	Fix more icc warnings.
*
* Previous Changes:
*   07/11/14 1.2    AGC	Fix icc warnings.
*   07/11/14 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_FUTURE_H
#define _SPX_FUTURE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoLock.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxFutureState.h"
#include "SPxLibUtils/SPxSharedPtr.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxTuple.h"

/* Forward declarations. */
template<typename T> class SPxPromise;
template<> class SPxPromise<void>;
template<typename T> class SPxFuture;
template<> class SPxFuture<void>;

namespace SPxAsyncNS
{
    template<typename T0, typename T1>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1> > whenAny2(SPxTuple<SPxFuture<T0>, SPxFuture<T1> >);
    template<typename T0, typename T1, typename T2>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > whenAny3(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> >);
    template<typename T0, typename T1, typename T2, typename T3>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > whenAny4(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> >);
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > whenAny5(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> >);
}

/* Define the generic version of SPxFuture. */
template<typename T>
class SPxFuture
{
public:
    SPxFuture() : m_state(SPxMakeShared<SPxFutureState<T> >()) { m_state->ready = true; }
    SPxFuture(typename SPxAddCrefIfNotRef<T>::type value) : m_state(SPxMakeShared<SPxFutureState<T> >(value)) {}
    ~SPxFuture() {}

    bool ready() const
    {
	SPxAutoLock lock(m_state->mutex);
	return m_state->ready;
    }
    T get() const
    {
	wait();
	return m_state->value;
    }
    void wait() const
    {
	return m_state->wait();
    }
    SPxErrorCode wait_for(UINT32 timeoutMS) const
    {
	return m_state->wait_for(timeoutMS);
    }

private:
    SPxFuture(const SPxSharedPtr<SPxFutureState<T> >& state) : m_state(state) {}

    SPxSharedPtr<SPxFutureState<T> > m_state;

    friend class SPxPromise<T>;

    template<typename T2, typename R>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(*)(T2));
    template<typename T2, typename R, typename Arg2>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(*)(T2, Arg2), typename SPxAddCrefIfNotRef<Arg2>::type);
    template<typename T2, typename R, typename Arg2, typename Arg3>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(*)(T2, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type);
    template<typename T2, typename R, typename Arg2, typename Arg3, typename Arg4>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(*)(T2, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, typename SPxAddCrefIfNotRef<Arg4>::type);
    template<typename T2, typename R, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(*)(T2, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, typename SPxAddCrefIfNotRef<Arg4>::type, typename SPxAddCrefIfNotRef<Arg5>::type);
    template<typename T2, typename R, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(O::*)(T2), OC*);
    template<typename T2, typename R, typename Arg2, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(O::*)(T2, Arg2), typename SPxAddCrefIfNotRef<Arg2>::type, OC*);
    template<typename T2, typename R, typename Arg2, typename Arg3, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(O::*)(T2, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, OC*);
    template<typename T2, typename R, typename Arg2, typename Arg3, typename Arg4, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(O::*)(T2, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, typename SPxAddCrefIfNotRef<Arg4>::type, OC*);
    template<typename T2, typename R, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<T2>&, R(O::*)(T2, Arg2, Arg3, Arg5), typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, typename SPxAddCrefIfNotRef<Arg4>::type, typename SPxAddCrefIfNotRef<Arg5>::type, OC*);

    template<typename T0, typename T1>
    friend SPxTuple<SPxFuture<T0>, SPxFuture<T1> > SPxAsyncNS::whenAny2(SPxTuple<SPxFuture<T0>, SPxFuture<T1> >);
    template<typename T0, typename T1, typename T2>
    friend SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > SPxAsyncNS::whenAny3(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> >);
    template<typename T0, typename T1, typename T2, typename T3>
    friend SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > SPxAsyncNS::whenAny4(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> >);
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    friend SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > SPxAsyncNS::whenAny5(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> >);
};

/* Void specialisation of SPxFuture that has not retrievable result value. */
template<>
class SPxFuture<void>
{
public:
    SPxFuture() : m_state(SPxMakeShared<SPxFutureState<void> >()) { m_state->ready = true; }
    ~SPxFuture() {}

    bool ready() const
    {
	SPxAutoLock lock(m_state->mutex);
	return m_state->ready;
    }
    void get() const
    {
	wait();
    }
    void wait() const
    {
	return m_state->wait();
    }
    SPxErrorCode wait_for(UINT32 timeoutMS) const
    {
	return m_state->wait_for(timeoutMS);
    }

private:
    SPxFuture(const SPxSharedPtr<SPxFutureState<void> >& state) : m_state(state) {}

    SPxSharedPtr<SPxFutureState<void> > m_state;

    friend class SPxPromise<void>;

    template<typename R>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(*)());
    template<typename R, typename Arg1>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(*)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type);
    template<typename R, typename Arg1, typename Arg2>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(*)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type, typename SPxAddCrefIfNotRef<Arg2>::type);
    template<typename R, typename Arg1, typename Arg2, typename Arg3>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(*)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type, typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type);
    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(*)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type, typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, typename SPxAddCrefIfNotRef<Arg4>::type);
    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(*)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type, typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, typename SPxAddCrefIfNotRef<Arg4>::type, typename SPxAddCrefIfNotRef<Arg5>::type);
    template<typename R, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(O::*)(), OC*);
    template<typename R, typename Arg1, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(O::*)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type, OC*);
    template<typename R, typename Arg1, typename Arg2, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(O::*)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type, typename SPxAddCrefIfNotRef<Arg2>::type, OC*);
    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(O::*)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type, typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, OC*);
    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(O::*)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type, typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, typename SPxAddCrefIfNotRef<Arg4>::type, OC*);
    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O, typename OC>
    friend SPxFuture<R> SPxThen(const SPxFuture<void>&, R(O::*)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type, typename SPxAddCrefIfNotRef<Arg2>::type, typename SPxAddCrefIfNotRef<Arg3>::type, typename SPxAddCrefIfNotRef<Arg4>::type, typename SPxAddCrefIfNotRef<Arg5>::type, OC*);

    template<typename T0, typename T1>
    friend SPxTuple<SPxFuture<T0>, SPxFuture<T1> > SPxAsyncNS::whenAny2(SPxTuple<SPxFuture<T0>, SPxFuture<T1> >);
    template<typename T0, typename T1, typename T2>
    friend SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > SPxAsyncNS::whenAny3(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> >);
    template<typename T0, typename T1, typename T2, typename T3>
    friend SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > SPxAsyncNS::whenAny4(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> >);
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    friend SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > SPxAsyncNS::whenAny5(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> >);
};

#endif /* _SPX_FUTURE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
