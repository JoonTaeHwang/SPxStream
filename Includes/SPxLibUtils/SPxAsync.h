/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAsync.h,v $
* ID: $Id: SPxAsync.h,v 1.3 2014/11/10 16:39:40 rew Exp $
*
* Purpose:
*   Header for SPxAsync() functions that can run work on another thread.
*
*   The SPxAsync() functions can be passed member or non-member functions
*   with up to 3 arguments.
*
*   The SPxAsync() functions return an SPxFuture<T> object where T is
*   the return type of the function passed to SPxAsync.
*
*   See the SPxFuture header for how to interact with SPxFuture objects.
*
*   By default, all functions passed to SPxAsync() functions are run
*   within a set of threads managed by a single private SPxThreadPool
*   object. Alternatively, a user-managed thread pool can be passed as
*   an additional first argument to SPxAsync() functions. The only
*   requirement on the user-manager thread pool is that it supports
*   a function named Submit() with the following signature.
*
*	void Submit(void (*task)(void *userArg), void *userArg);
*
*   The Submit() function must queue the provided task up for execution,
*   and execute it at some point in the future (which may be immediate),
*   passing the task the provided user argument.
*
*   The SPxThen() functions allow another function to be run after
*   the completion of an existing SPxFuture. This allows multiple
*   functions to be chained.
*
*   The SPxWhenAll() and SPxWhenAny() functions take between 1 and 5
*   future objects and return another future object that is ready
*   when all/any (as applicable) of the original futures are ready.
*
* Revision Control:
*   10/11/14 v1.3    AGC	Fix icc warnings.
*
* Previous Changes:
*   07/11/14 1.2    AGC	Fix intel build problem in first version.
*   07/11/14 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_ASYNC_H
#define _SPX_ASYNC_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoLock.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCondVar.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxPromise.h"
#include "SPxLibUtils/SPxThreads.h"
#include "SPxLibUtils/SPxThreadPool.h"
#include "SPxLibUtils/SPxTuple.h"

/*********************************************************************
*
* SPxAsync
*
**********************************************************************/

namespace SPxAsyncNS
{
    template<typename R>
    struct Data0
    {
	Data0(R (*func_)())
	    : promise(), thread(), func(func_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (*func)();
    };

    template<typename R, typename Arg1>
    struct Data1
    {
	Data1(R (*func_)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1_)
	    : promise(), thread(), func(func_), arg1(arg1_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (*func)(Arg1 arg1);
	Arg1 arg1;
    };

    template<typename R, typename Arg1, typename Arg2>
    struct Data2
    {
	Data2(R (*func_)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_)
	    : promise(), thread(), func(func_), arg1(arg1_), arg2(arg2_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (*func)(Arg1 arg1, Arg2 arg2);
	Arg1 arg1;
	Arg2 arg2;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3>
    struct Data3
    {
	Data3(R (*func_)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_)
	    : promise(), thread(), func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (*func)(Arg1 arg1, Arg2 arg2, Arg3 arg3);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct Data4
    {
	Data4(R (*func_)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_)
	    : promise(), thread(), func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (*func)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    struct Data5
    {
	Data5(R (*func_)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, typename SPxAddCrefIfNotRef<Arg5>::type arg5_)
	    : promise(), thread(), func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (*func)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	Arg5 arg5;
    };

    template<typename R, typename O>
    struct DataM0
    {
	DataM0(R (O::*func_)(), O *obj_)
	    : promise(), thread(), func(func_), obj(obj_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (O::*func)();
	O *obj;
    };

    template<typename R, typename Arg1, typename O>
    struct DataM1
    {
	DataM1(R (O::*func_)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, O *obj_)
	    : promise(), thread(), func(func_), arg1(arg1_), obj(obj_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (O::*func)(Arg1);
	Arg1 arg1;
	O *obj;
    };

    template<typename R, typename Arg1, typename Arg2, typename O>
    struct DataM2
    {
	DataM2(R (O::*func_)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, O *obj_)
	    : promise(), thread(), func(func_), arg1(arg1_), arg2(arg2_), obj(obj_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (O::*func)(Arg1, Arg2);
	Arg1 arg1;
	Arg2 arg2;
	O *obj;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename O>
    struct DataM3
    {
	DataM3(R (O::*func_)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, O *obj_)
	    : promise(), thread(), func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), obj(obj_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (O::*func)(Arg1, Arg2, Arg3);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	O *obj;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O>
    struct DataM4
    {
	DataM4(R (O::*func_)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, O *obj_)
	    : promise(), thread(), func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), obj(obj_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (O::*func)(Arg1, Arg2, Arg3, Arg4);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	O *obj;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O>
    struct DataM5
    {
	DataM5(R (O::*func_)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, typename SPxAddCrefIfNotRef<Arg5>::type arg5_, O *obj_)
	    : promise(), thread(), func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), obj(obj_) {}
	SPxPromise<R> promise;
	SPxThread thread;
	R (O::*func)(Arg1, Arg2, Arg3, Arg4, Arg5);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	Arg5 arg5;
	O *obj;
    };

    template<typename R>
    struct ThreadFunc0
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data0<R> > data(static_cast<Data0<R>*>(userArg));
	    R val = data->func();
	    data->promise.set_value(val);
	}
    };

    template<>
    struct ThreadFunc0<void>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data0<void> > data(static_cast<Data0<void>*>(userArg));
	    data->func();
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1>
    struct ThreadFunc1
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data1<R, Arg1> > data(static_cast<Data1<R, Arg1>*>(userArg));
	    R val = data->func(data->arg1);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1>
    struct ThreadFunc1<void, Arg1>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data1<void, Arg1> > data(static_cast<Data1<void, Arg1>*>(userArg));
	    data->func(data->arg1);
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename Arg2>
    struct ThreadFunc2
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data2<R, Arg1, Arg2> > data(static_cast<Data2<R, Arg1, Arg2>*>(userArg));
	    R val = data->func(data->arg1, data->arg2);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename Arg2>
    struct ThreadFunc2<void, Arg1, Arg2>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data2<void, Arg1, Arg2> > data(static_cast<Data2<void, Arg1, Arg2>*>(userArg));
	    data->func(data->arg1, data->arg2);
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3>
    struct ThreadFunc3
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data3<R, Arg1, Arg2, Arg3> > data(static_cast<Data3<R, Arg1, Arg2, Arg3>*>(userArg));
	    R val = data->func(data->arg1, data->arg2, data->arg3);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename Arg2, typename Arg3>
    struct ThreadFunc3<void, Arg1, Arg2, Arg3>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data3<void, Arg1, Arg2, Arg3> > data(static_cast<Data3<void, Arg1, Arg2, Arg3>*>(userArg));
	    data->func(data->arg1, data->arg2, data->arg3);
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct ThreadFunc4
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data4<R, Arg1, Arg2, Arg3, Arg4> > data(static_cast<Data4<R, Arg1, Arg2, Arg3, Arg4>*>(userArg));
	    R val = data->func(data->arg1, data->arg2, data->arg3, data->arg4);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct ThreadFunc4<void, Arg1, Arg2, Arg3, Arg4>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data4<void, Arg1, Arg2, Arg3, Arg4> > data(static_cast<Data4<void, Arg1, Arg2, Arg3, Arg4>*>(userArg));
	    data->func(data->arg1, data->arg2, data->arg3, data->arg4);
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    struct ThreadFunc5
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data5<R, Arg1, Arg2, Arg3, Arg4, Arg5> > data(static_cast<Data5<R, Arg1, Arg2, Arg3, Arg4, Arg5>*>(userArg));
	    R val = data->func(data->arg1, data->arg2, data->arg3, data->arg4, data->arg5);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    struct ThreadFunc5<void, Arg1, Arg2, Arg3, Arg4, Arg5>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<Data5<void, Arg1, Arg2, Arg3, Arg4, Arg5> > data(static_cast<Data5<void, Arg1, Arg2, Arg3, Arg4, Arg5>*>(userArg));
	    data->func(data->arg1, data->arg2, data->arg3, data->arg4, data->arg5);
	    data->promise.set_value();
	}
    };

    template<typename R, typename O>
    struct ThreadFuncM0
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM0<R, O> > data(static_cast<DataM0<R, O>*>(userArg));
	    R val = ((data->obj)->*(data->func))();
	    data->promise.set_value(val);
	}
    };

    template<typename O>
    struct ThreadFuncM0<void, O>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM0<void, O> > data(static_cast<DataM0<void, O>*>(userArg));
	    ((data->obj)->*(data->func))();
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename O>
    struct ThreadFuncM1
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM1<R, Arg1, O> > data(static_cast<DataM1<R, Arg1, O>*>(userArg));
	    R val = ((data->obj)->*(data->func))(data->arg1);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename O>
    struct ThreadFuncM1<void, Arg1, O>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM1<void, Arg1, O> > data(static_cast<DataM1<void, Arg1, O>*>(userArg));
	    ((data->obj)->*(data->func))(data->arg1);
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename Arg2, typename O>
    struct ThreadFuncM2
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM2<R, Arg1, Arg2, O> > data(static_cast<DataM2<R, Arg1, Arg2, O>*>(userArg));
	    R val = ((data->obj)->*(data->func))(data->arg1, data->arg2);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename Arg2, typename O>
    struct ThreadFuncM2<void, Arg1, Arg2, O>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM2<void, Arg1, Arg2, O> > data(static_cast<DataM2<void, Arg1, Arg2, O>*>(userArg));
	    ((data->obj)->*(data->func))(data->arg1, data->arg2);
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename O>
    struct ThreadFuncM3
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM3<R, Arg1, Arg2, Arg3, O> > data(static_cast<DataM3<R, Arg1, Arg2, Arg3, O>*>(userArg));
	    R val = ((data->obj)->*(data->func))(data->arg1, data->arg2, data->arg3);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename Arg2, typename Arg3, typename O>
    struct ThreadFuncM3<void, Arg1, Arg2, Arg3, O>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM3<void, Arg1, Arg2, Arg3, O> > data(static_cast<DataM3<void, Arg1, Arg2, Arg3, O>*>(userArg));
	    ((data->obj)->*(data->func))(data->arg1, data->arg2, data->arg3);
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O>
    struct ThreadFuncM4
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM4<R, Arg1, Arg2, Arg3, Arg4, O> > data(static_cast<DataM4<R, Arg1, Arg2, Arg3, Arg4, O>*>(userArg));
	    R val = ((data->obj)->*(data->func))(data->arg1, data->arg2, data->arg3, data->arg4);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O>
    struct ThreadFuncM4<void, Arg1, Arg2, Arg3, Arg4, O>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM4<void, Arg1, Arg2, Arg3, Arg4, O> > data(static_cast<DataM4<void, Arg1, Arg2, Arg3, Arg4, O>*>(userArg));
	    ((data->obj)->*(data->func))(data->arg1, data->arg2, data->arg3, data->arg4);
	    data->promise.set_value();
	}
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O>
    struct ThreadFuncM5
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O> > data(static_cast<DataM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O>*>(userArg));
	    R val = ((data->obj)->*(data->func))(data->arg1, data->arg2, data->arg3, data->arg4, data->arg5);
	    data->promise.set_value(val);
	}
    };

    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O>
    struct ThreadFuncM5<void, Arg1, Arg2, Arg3, Arg4, Arg5, O>
    {
	static void invoke(void *userArg)
	{
	    SPxAutoPtr<DataM5<void, Arg1, Arg2, Arg3, Arg4, Arg5, O> > data(static_cast<DataM5<void, Arg1, Arg2, Arg3, Arg4, Arg5, O>*>(userArg));
	    ((data->obj)->*(data->func))(data->arg1, data->arg2, data->arg3, data->arg4, data->arg5);
	    data->promise.set_value();
	}
    };

    SPxThreadPool *GetThreadPool(void);
}

template<typename TP, typename R>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(*func)())
{
    SPxAsyncNS::Data0<R> *data(new SPxAsyncNS::Data0<R>(func));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFunc0<R>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(*func)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1)
{
    SPxAsyncNS::Data1<R, Arg1> *data(new SPxAsyncNS::Data1<R, Arg1>(func, arg1));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFunc1<R, Arg1>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename Arg2>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(*func)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2)
{
    SPxAsyncNS::Data2<R, Arg1, Arg2> *data(new SPxAsyncNS::Data2<R, Arg1, Arg2>(func, arg1, arg2));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFunc2<R, Arg1, Arg2>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename Arg2, typename Arg3>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(*func)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3)
{
    SPxAsyncNS::Data3<R, Arg1, Arg2, Arg3> *data(new SPxAsyncNS::Data3<R, Arg1, Arg2, Arg3>(func, arg1, arg2, arg3));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFunc3<R, Arg1, Arg2, Arg3>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(*func)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4)
{
    SPxAsyncNS::Data4<R, Arg1, Arg2, Arg3, Arg4> *data(new SPxAsyncNS::Data4<R, Arg1, Arg2, Arg3, Arg4>(func, arg1, arg2, arg3, arg4));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFunc4<R, Arg1, Arg2, Arg3, Arg4>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(*func)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, typename SPxAddCrefIfNotRef<Arg5>::type arg5)
{
    SPxAsyncNS::Data5<R, Arg1, Arg2, Arg3, Arg4, Arg5> *data(new SPxAsyncNS::Data5<R, Arg1, Arg2, Arg3, Arg4, Arg5>(func, arg1, arg2, arg3, arg4, arg5));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFunc5<R, Arg1, Arg2, Arg3, Arg4, Arg5>::invoke, data);
    return future;
}

template<typename TP, typename R, typename O, typename OC>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(O::*func)(), OC *obj)
{
    SPxAsyncNS::DataM0<R, O> *data(new SPxAsyncNS::DataM0<R, O>(func, static_cast<O*>(obj)));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFuncM0<R, O>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename O, typename OC>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(O::*func)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1, OC *obj)
{
    SPxAsyncNS::DataM1<R, Arg1, O> *data(new SPxAsyncNS::DataM1<R, Arg1, O>(func, arg1, static_cast<O*>(obj)));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFuncM1<R, Arg1, O>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename Arg2, typename O, typename OC>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(O::*func)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, OC *obj)
{
    SPxAsyncNS::DataM2<R, Arg1, Arg2, O> *data(new SPxAsyncNS::DataM2<R, Arg1, Arg2, O>(func, arg1, arg2, static_cast<O*>(obj)));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFuncM2<R, Arg1, Arg2, O>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename Arg2, typename Arg3, typename O, typename OC>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(O::*func)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, OC *obj)
{
    SPxAsyncNS::DataM3<R, Arg1, Arg2, Arg3, O> *data(new SPxAsyncNS::DataM3<R, Arg1, Arg2, Arg3, O>(func, arg1, arg2, arg3, static_cast<O*>(obj)));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFuncM3<R, Arg1, Arg2, Arg3, O>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O, typename OC>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(O::*func)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, OC *obj)
{
    SPxAsyncNS::DataM4<R, Arg1, Arg2, Arg3, Arg4, O> *data(new SPxAsyncNS::DataM4<R, Arg1, Arg2, Arg3, Arg4, O>(func, arg1, arg2, arg3, arg4, static_cast<O*>(obj)));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFuncM4<R, Arg1, Arg2, Arg3, Arg4, O>::invoke, data);
    return future;
}

template<typename TP, typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O, typename OC>
inline SPxFuture<R> SPxAsync(TP *threadPool, R(O::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, typename SPxAddCrefIfNotRef<Arg5>::type arg5, OC *obj)
{
    SPxAsyncNS::DataM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O> *data(new SPxAsyncNS::DataM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O>(func, arg1, arg2, arg3, arg4, arg5, static_cast<O*>(obj)));
    SPxFuture<R> future = data->promise.get_future();
    threadPool->Submit(SPxAsyncNS::ThreadFuncM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O>::invoke, data);
    return future;
}

template<typename R>
inline SPxFuture<R> SPxAsync(R(*func)())
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func);
}

template<typename R, typename Arg1>
inline SPxFuture<R> SPxAsync(R(*func)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1);
}

template<typename R, typename Arg1, typename Arg2>
inline SPxFuture<R> SPxAsync(R(*func)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, arg2);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3>
inline SPxFuture<R> SPxAsync(R(*func)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, arg2, arg3);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline SPxFuture<R> SPxAsync(R(*func)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, arg2, arg3, arg4);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline SPxFuture<R> SPxAsync(R(*func)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, typename SPxAddCrefIfNotRef<Arg5>::type arg5)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, arg2, arg3, arg4, arg5);
}

template<typename R, typename O, typename OC>
inline SPxFuture<R> SPxAsync(R(O::*func)(), OC *obj)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, obj);
}

template<typename R, typename Arg1, typename O, typename OC>
inline SPxFuture<R> SPxAsync(R(O::*func)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1, OC *obj)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, obj);
}

template<typename R, typename Arg1, typename Arg2, typename O, typename OC>
inline SPxFuture<R> SPxAsync(R(O::*func)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, OC *obj)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, arg2, obj);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename O, typename OC>
inline SPxFuture<R> SPxAsync(R(O::*func)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, OC *obj)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, arg2, arg3, obj);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O, typename OC>
inline SPxFuture<R> SPxAsync(R(O::*func)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, OC *obj)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, arg2, arg3, arg4, obj);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O, typename OC>
inline SPxFuture<R> SPxAsync(R(O::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, typename SPxAddCrefIfNotRef<Arg5>::type arg5, OC *obj)
{
    return SPxAsync(SPxAsyncNS::GetThreadPool(), func, arg1, arg2, arg3, arg4, arg5, obj);
}

/*********************************************************************
*
* SPxThen
*
**********************************************************************/

namespace SPxAsyncNS
{
    template<typename T, typename R>
    struct DataT1
    {
	DataT1(R(*func_)(T), const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), state(state_) {}

	R(*func)(T);
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename Arg2>
    struct DataT2
    {
	DataT2(R(*func_)(T, Arg2), typename SPxAddCrefIfNotRef<Arg2>::type arg2_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), arg2(arg2_), state(state_) {}

	R(*func)(T, Arg2);
	Arg2 arg2;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename Arg2, typename Arg3>
    struct DataT3
    {
	DataT3(R(*func_)(T, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), arg2(arg2_), arg3(arg3_), state(state_) {}

	R(*func)(T, Arg2, Arg3);
	Arg2 arg2;
	Arg3 arg3;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4>
    struct DataT4
    {
	DataT4(R(*func_)(T, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), arg2(arg2_), arg3(arg3_), arg4(arg4_), state(state_) {}

	R(*func)(T, Arg2, Arg3, Arg4);
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    struct DataT5
    {
	DataT5(R(*func_)(T, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, typename SPxAddCrefIfNotRef<Arg5>::type arg5_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), state(state_) {}

	R(*func)(T, Arg2, Arg3, Arg4, Arg5);
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	Arg5 arg5;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename O>
    struct DataTM1
    {
	DataTM1(R(O::*func_)(T), O *obj_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), obj(obj_), state(state_) {}

	R(O::*func)(T);
	O *obj;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename Arg2, typename O>
    struct DataTM2
    {
	DataTM2(R(O::*func_)(T, Arg2), typename SPxAddCrefIfNotRef<Arg2>::type arg2_, O *obj_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), arg2(arg2_), obj(obj_), state(state_) {}

	R(O::*func)(T, Arg2);
	Arg2 arg2;
	O *obj;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename Arg2, typename Arg3, typename O>
    struct DataTM3
    {
	DataTM3(R(O::*func_)(T, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, O *obj_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), arg2(arg2_), arg3(arg3_), obj(obj_), state(state_) {}

	R(O::*func)(T, Arg2, Arg3);
	Arg2 arg2;
	Arg3 arg3;
	O *obj;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename O>
    struct DataTM4
    {
	DataTM4(R(O::*func_)(T, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, O *obj_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), arg2(arg2_), arg3(arg3_), arg4(arg4_), obj(obj_), state(state_) {}

	R(O::*func)(T, Arg2, Arg3, Arg4);
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	O *obj;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O>
    struct DataTM5
    {
	DataTM5(R(O::*func_)(T, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, typename SPxAddCrefIfNotRef<Arg5>::type arg5_, O *obj_, const SPxSharedPtr<SPxFutureState<T> >& state_)
	    : func(func_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), obj(obj_), state(state_) {}

	R(O::*func)(T, Arg2, Arg3, Arg4, Arg5);
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	Arg5 arg5;
	O *obj;
	SPxSharedPtr<SPxFutureState<T> > state;
    };

    template<typename R>
    struct DataV0
    {
	DataV0(R(*func_)(), const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), state(state_) {}

	R(*func)();
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1>
    struct DataV1
    {
	DataV1(R(*func_)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), state(state_) {}

	R(*func)(Arg1);
	Arg1 arg1;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename Arg2>
    struct DataV2
    {
	DataV2(R(*func_)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), arg2(arg2_), state(state_) {}

	R(*func)(Arg1, Arg2);
	Arg1 arg1;
	Arg2 arg2;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3>
    struct DataV3
    {
	DataV3(R(*func_)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), state(state_) {}

	R(*func)(Arg1, Arg2, Arg3);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct DataV4
    {
	DataV4(R(*func_)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), state(state_) {}

	R(*func)(Arg1, Arg2, Arg3, Arg4);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    struct DataV5
    {
	DataV5(R(*func_)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, typename SPxAddCrefIfNotRef<Arg5>::type arg5_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), state(state_) {}

	R(*func)(Arg1, Arg2, Arg3, Arg4, Arg5);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	Arg5 arg5;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename O>
    struct DataVM0
    {
	DataVM0(R(O::*func_)(), O *obj_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), obj(obj_), state(state_) {}

	R(O::*func)();
	O *obj;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename O>
    struct DataVM1
    {
	DataVM1(R(O::*func_)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, O *obj_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), obj(obj_), state(state_) {}

	R(O::*func)(Arg1);
	Arg1 arg1;
	O *obj;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename Arg2, typename O>
    struct DataVM2
    {
	DataVM2(R(O::*func_)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, O *obj_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), arg2(arg2_), obj(obj_), state(state_) {}

	R(O::*func)(Arg1, Arg2);
	Arg1 arg1;
	Arg2 arg2;
	O *obj;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename O>
    struct DataVM3
    {
	DataVM3(R(O::*func_)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, O *obj_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), obj(obj_), state(state_) {}

	R(O::*func)(Arg1, Arg2, Arg3);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	O *obj;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O>
    struct DataVM4
    {
	DataVM4(R(O::*func_)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, O *obj_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), obj(obj_), state(state_) {}

	R(O::*func)(Arg1, Arg2, Arg3, Arg4);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	O *obj;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O>
    struct DataVM5
    {
	DataVM5(R(O::*func_)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1_, typename SPxAddCrefIfNotRef<Arg2>::type arg2_, typename SPxAddCrefIfNotRef<Arg3>::type arg3_, typename SPxAddCrefIfNotRef<Arg4>::type arg4_, typename SPxAddCrefIfNotRef<Arg5>::type arg5_, O *obj_, const SPxSharedPtr<SPxFutureState<void> >& state_)
	    : func(func_), arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), obj(obj_), state(state_) {}

	R(O::*func)(Arg1, Arg2, Arg3, Arg4, Arg5);
	Arg1 arg1;
	Arg2 arg2;
	Arg3 arg3;
	Arg4 arg4;
	Arg5 arg5;
	O *obj;
	SPxSharedPtr<SPxFutureState<void> > state;
    };

    template<typename T, typename R>
    static R Task1(DataT1<T, R> *data)
    {
	SPxAutoPtr<DataT1<T, R> > dataT(data);
	
	dataT->state->wait();
	return (dataT->func)(dataT->state->value);
    }

    template<typename T, typename R, typename Arg2>
    static R Task2(DataT2<T, R, Arg2> *data)
    {
	SPxAutoPtr<DataT2<T, R, Arg2> > dataT(data);
	
	dataT->state->wait();
	return (dataT->func)(dataT->state->value, dataT->arg2);
    }

    template<typename T, typename R, typename Arg2, typename Arg3>
    static R Task3(DataT3<T, R, Arg2, Arg3> *data)
    {
	SPxAutoPtr<DataT3<T, R, Arg2, Arg3> > dataT(data);
	
	dataT->state->wait();
	return (dataT->func)(dataT->state->value, dataT->arg2, dataT->arg3);
    }

    template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4>
    static R Task4(DataT4<T, R, Arg2, Arg3, Arg4> *data)
    {
	SPxAutoPtr<DataT4<T, R, Arg2, Arg3, Arg4> > dataT(data);
	
	dataT->state->wait();
	return (dataT->func)(dataT->state->value, dataT->arg2, dataT->arg3, dataT->arg4);
    }

    template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    static R Task5(DataT5<T, R, Arg2, Arg3, Arg4, Arg5> *data)
    {
	SPxAutoPtr<DataT5<T, R, Arg2, Arg3, Arg4, Arg5> > dataT(data);
	
	dataT->state->wait();
	return (dataT->func)(dataT->state->value, dataT->arg2, dataT->arg3, dataT->arg4, dataT->arg5);
    }

    template<typename T, typename R, typename O>
    static R TaskM1(DataTM1<T, R, O> *data)
    {
	SPxAutoPtr<DataTM1<T, R, O> > dataT(data);
	
	dataT->state->wait();
	return ((dataT->obj)->*(dataT->func))(dataT->state->value);
    }

    template<typename T, typename R, typename Arg2, typename O>
    static R TaskM2(DataTM2<T, R, Arg2, O> *data)
    {
	SPxAutoPtr<DataTM2<T, R, Arg2, O> > dataT(data);
	
	dataT->state->wait();
	return ((dataT->obj)->*(dataT->func))(dataT->state->value, dataT->arg2);
    }

    template<typename T, typename R, typename Arg2, typename Arg3, typename O>
    static R TaskM3(DataTM3<T, R, Arg2, Arg3, O> *data)
    {
	SPxAutoPtr<DataTM3<T, R, Arg2, Arg3, O> > dataT(data);
	
	dataT->state->wait();
	return ((dataT->obj)->*(dataT->func))(dataT->state->value, dataT->arg2, dataT->arg3);
    }

    template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename O>
    static R TaskM4(DataTM4<T, R, Arg2, Arg3, Arg4, O> *data)
    {
	SPxAutoPtr<DataTM4<T, R, Arg2, Arg3, Arg4, O> > dataT(data);
	
	dataT->state->wait();
	return ((dataT->obj)->*(dataT->func))(dataT->state->value, dataT->arg2, dataT->arg3, dataT->arg4);
    }

    template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O>
    static R TaskM5(DataTM5<T, R, Arg2, Arg3, Arg4, Arg5, O> *data)
    {
	SPxAutoPtr<DataTM5<T, R, Arg2, Arg3, Arg4, Arg5, O> > dataT(data);
	
	dataT->state->wait();
	return ((dataT->obj)->*(dataT->func))(dataT->state->value, dataT->arg2, dataT->arg3, dataT->arg4, dataT->arg5);
    }

    template<typename R>
    static R TaskV0(DataV0<R> *data)
    {
	SPxAutoPtr<DataV0<R> > dataV(data);
	
	dataV->state->wait();
	return (dataV->func)();
    }

    template<typename R, typename Arg1>
    static R TaskV1(DataV1<R, Arg1> *data)
    {
	SPxAutoPtr<DataV1<R, Arg1> > dataV(data);
	
	dataV->state->wait();
	return (dataV->func)(dataV->arg1);
    }

    template<typename R, typename Arg1, typename Arg2>
    static R TaskV2(DataV2<R, Arg1, Arg2> *data)
    {
	SPxAutoPtr<DataV2<R, Arg1, Arg2> > dataV(data);
	
	dataV->state->wait();
	return (dataV->func)(dataV->arg1, dataV->arg2);
    }

    template<typename R, typename Arg1, typename Arg2, typename Arg3>
    static R TaskV3(DataV3<R, Arg1, Arg2, Arg3> *data)
    {
	SPxAutoPtr<DataV3<R, Arg1, Arg2, Arg3> > dataV(data);
	
	dataV->state->wait();
	return (dataV->func)(dataV->arg1, dataV->arg2, dataV->arg3);
    }

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    static R TaskV4(DataV4<R, Arg1, Arg2, Arg3, Arg4> *data)
    {
	SPxAutoPtr<DataV4<R, Arg1, Arg2, Arg3, Arg4> > dataV(data);
	
	dataV->state->wait();
	return (dataV->func)(dataV->arg1, dataV->arg2, dataV->arg3, dataV->arg4);
    }

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    static R TaskV5(DataV5<R, Arg1, Arg2, Arg3, Arg4, Arg5> *data)
    {
	SPxAutoPtr<DataV5<R, Arg1, Arg2, Arg3, Arg4, Arg5> > dataV(data);
	
	dataV->state->wait();
	return (dataV->func)(dataV->arg1, dataV->arg2, dataV->arg3, dataV->arg4, dataV->arg5);
    }

    template<typename R, typename O>
    static R TaskVM0(DataVM0<R, O> *data)
    {
	SPxAutoPtr<DataVM0<R, O> > dataV(data);
	
	dataV->state->wait();
	return ((dataV->obj)->*(dataV->func))();
    }

    template<typename R, typename Arg1, typename O>
    static R TaskVM1(DataVM1<R, Arg1, O> *data)
    {
	SPxAutoPtr<DataVM1<R, Arg1, O> > dataV(data);
	
	dataV->state->wait();
	return ((dataV->obj)->*(dataV->func))(dataV->arg1);
    }

    template<typename R, typename Arg1, typename Arg2, typename O>
    static R TaskVM2(DataVM2<R, Arg1, Arg2, O> *data)
    {
	SPxAutoPtr<DataVM2<R, Arg1, Arg2, O> > dataV(data);
	
	dataV->state->wait();
	return ((dataV->obj)->*(dataV->func))(dataV->arg1, dataV->arg2);
    }

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename O>
    static R TaskVM3(DataVM3<R, Arg1, Arg2, Arg3, O> *data)
    {
	SPxAutoPtr<DataVM3<R, Arg1, Arg2, Arg3, O> > dataV(data);
	
	dataV->state->wait();
	return ((dataV->obj)->*(dataV->func))(dataV->arg1, dataV->arg2, dataV->arg3);
    }

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O>
    static R TaskVM4(DataVM4<R, Arg1, Arg2, Arg3, Arg4, O> *data)
    {
	SPxAutoPtr<DataVM4<R, Arg1, Arg2, Arg3, Arg4, O> > dataV(data);
	
	dataV->state->wait();
	return ((dataV->obj)->*(dataV->func))(dataV->arg1, dataV->arg2, dataV->arg3, dataV->arg4);
    }

    template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O>
    static R TaskVM5(DataVM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O> *data)
    {
	SPxAutoPtr<DataVM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O> > dataV(data);
	
	dataV->state->wait();
	return ((dataV->obj)->*(dataV->func))(dataV->arg1, dataV->arg2, dataV->arg3, dataV->arg4, dataV->arg5);
    }
}

template<typename T, typename R>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(*func)(T))
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1);
    }

    SPxAsyncNS::DataT1<T, R> *data = new SPxAsyncNS::DataT1<T, R>(func, future.m_state);
    return SPxAsync(SPxAsyncNS::Task1<T, R>, data);
}

template<typename T, typename R, typename Arg2>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(*func)(T, Arg2), typename SPxAddCrefIfNotRef<Arg2>::type arg2)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, arg2);
    }

    SPxAsyncNS::DataT2<T, R, Arg2> * data = new SPxAsyncNS::DataT2<T, R, Arg2>(func, arg2, future.m_state);
    return SPxAsync(SPxAsyncNS::Task2<T, R, Arg2>, data);
}

template<typename T, typename R, typename Arg2, typename Arg3>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(*func)(T, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, arg2, arg3);
    }

    SPxAsyncNS::DataT3<T, R, Arg2, Arg3> * data = new SPxAsyncNS::DataT3<T, R, Arg2, Arg3>(func, arg2, arg3, future.m_state);
    return SPxAsync(SPxAsyncNS::Task3<T, R, Arg2, Arg3>, data);
}

template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(*func)(T, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, arg2, arg3, arg4);
    }

    SPxAsyncNS::DataT4<T, R, Arg2, Arg3, Arg4> * data = new SPxAsyncNS::DataT4<T, R, Arg2, Arg3, Arg4>(func, arg2, arg3, arg4, future.m_state);
    return SPxAsync(SPxAsyncNS::Task4<T, R, Arg2, Arg3, Arg4>, data);
}

template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(*func)(T, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, typename SPxAddCrefIfNotRef<Arg5>::type arg5)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, arg2, arg3, arg4, arg5);
    }

    SPxAsyncNS::DataT5<T, R, Arg2, Arg3, Arg4, Arg5> * data = new SPxAsyncNS::DataT5<T, R, Arg2, Arg3, Arg4, Arg5>(func, arg2, arg3, arg4, arg5, future.m_state);
    return SPxAsync(SPxAsyncNS::Task5<T, R, Arg2, Arg3, Arg4, Arg5>, data);
}

template<typename T, typename R, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(O::*func)(T), OC *obj)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, obj);
    }

    SPxAsyncNS::DataTM1<T, R, O> * data = new SPxAsyncNS::DataTM1<T, R, O>(func, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskM1<T, R, O>, data);
}

template<typename T, typename R, typename Arg2, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(O::*func)(T, Arg2), typename SPxAddCrefIfNotRef<Arg2>::type arg2, OC *obj)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, arg2, obj);
    }

    SPxAsyncNS::DataTM2<T, R, Arg2, O> * data = new SPxAsyncNS::DataTM2<T, R, Arg2, O>(func, arg2, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskM2<T, R, Arg2, O>, data);
}

template<typename T, typename R, typename Arg2, typename Arg3, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(O::*func)(T, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, OC *obj)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, arg2, arg3, obj);
    }

    SPxAsyncNS::DataTM3<T, R, Arg2, Arg3, O> * data = new SPxAsyncNS::DataTM3<T, R, Arg2, Arg3, O>(func, arg2, arg3, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskM3<T, R, Arg2, Arg3, O>, data);
}

template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(O::*func)(T, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, OC *obj)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, arg2, arg3, arg4, obj);
    }

    SPxAsyncNS::DataTM4<T, R, Arg2, Arg3, Arg4, O> * data = new SPxAsyncNS::DataTM4<T, R, Arg2, Arg3, Arg4, O>(func, arg2, arg3, arg4, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskM4<T, R, Arg2, Arg3, Arg4, O>, data);
}

template<typename T, typename R, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<T> &future, R(O::*func)(T, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, typename SPxAddCrefIfNotRef<Arg5>::type arg5, OC *obj)
{
    if( future.ready() )
    {
	T arg1 = future.get();
	return SPxAsync(func, arg1, arg2, arg3, arg4, arg5, obj);
    }

    SPxAsyncNS::DataTM5<T, R, Arg2, Arg3, Arg4, Arg5, O> * data = new SPxAsyncNS::DataTM5<T, R, Arg2, Arg3, Arg4, Arg5, O>(func, arg2, arg3, arg4, arg5, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskM5<T, R, Arg2, Arg3, Arg4, Arg5, O>, data);
}

template<typename R>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(*func)())
{
    if( future.ready() )
    {
	return SPxAsync(func);
    }

    SPxAsyncNS::DataV0<R> *data = new SPxAsyncNS::DataV0<R>(func, future.m_state);
    return SPxAsync(SPxAsyncNS::TaskV0<R>, data);
}

template<typename R, typename Arg1>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(*func)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1);
    }

    SPxAsyncNS::DataV1<R, Arg1> *data = new SPxAsyncNS::DataV1<R, Arg1>(func, arg1, future.m_state);
    return SPxAsync(SPxAsyncNS::TaskV1<R, Arg1>, data);
}

template<typename R, typename Arg1, typename Arg2>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(*func)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, arg2);
    }

    SPxAsyncNS::DataV2<R, Arg1, Arg2> *data = new SPxAsyncNS::DataV2<R, Arg1, Arg2>(func, arg1, arg2, future.m_state);
    return SPxAsync(SPxAsyncNS::TaskV2<R, Arg1, Arg2>, data);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(*func)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, arg2, arg3);
    }

    SPxAsyncNS::DataV3<R, Arg1, Arg2, Arg3> *data = new SPxAsyncNS::DataV3<R, Arg1, Arg2, Arg3>(func, arg1, arg2, arg3, future.m_state);
    return SPxAsync(SPxAsyncNS::TaskV3<R, Arg1, Arg2, Arg3>, data);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(*func)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, arg2, arg3, arg4);
    }

    SPxAsyncNS::DataV4<R, Arg1, Arg2, Arg3, Arg4> *data = new SPxAsyncNS::DataV4<R, Arg1, Arg2, Arg3, Arg4>(func, arg1, arg2, arg3, arg4, future.m_state);
    return SPxAsync(SPxAsyncNS::TaskV4<R, Arg1, Arg2, Arg3, Arg4>, data);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(*func)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, typename SPxAddCrefIfNotRef<Arg5>::type arg5)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, arg2, arg3, arg4, arg5);
    }

    SPxAsyncNS::DataV5<R, Arg1, Arg2, Arg3, Arg4, Arg5> *data = new SPxAsyncNS::DataV5<R, Arg1, Arg2, Arg3, Arg4, Arg5>(func, arg1, arg2, arg3, arg4, arg5, future.m_state);
    return SPxAsync(SPxAsyncNS::TaskV5<R, Arg1, Arg2, Arg3, Arg4, Arg5>, data);
}

template<typename R, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(O::*func)(), OC *obj)
{
    if( future.ready() )
    {
	return SPxAsync(func, obj);
    }

    SPxAsyncNS::DataVM0<R, O> *data = new SPxAsyncNS::DataVM0<R, O>(func, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskVM0<R, O>, data);
}

template<typename R, typename Arg1, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(O::*func)(Arg1), typename SPxAddCrefIfNotRef<Arg1>::type arg1, OC *obj)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, obj);
    }

    SPxAsyncNS::DataVM1<R, Arg1, O> *data = new SPxAsyncNS::DataVM1<R, Arg1, O>(func, arg1, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskVM1<R, Arg1, O>, data);
}

template<typename R, typename Arg1, typename Arg2, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(O::*func)(Arg1, Arg2), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, OC *obj)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, arg2, obj);
    }

    SPxAsyncNS::DataVM2<R, Arg1, Arg2, O> *data = new SPxAsyncNS::DataVM2<R, Arg1, Arg2, O>(func, arg1, arg2, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskVM2<R, Arg1, Arg2, O>, data);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(O::*func)(Arg1, Arg2, Arg3), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, OC *obj)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, arg2, arg3, obj);
    }

    SPxAsyncNS::DataVM3<R, Arg1, Arg2, Arg3, O> *data = new SPxAsyncNS::DataVM3<R, Arg1, Arg2, Arg3, O>(func, arg1, arg2, arg3, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskVM3<R, Arg1, Arg2, Arg3, O>, data);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(O::*func)(Arg1, Arg2, Arg3, Arg4), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, OC *obj)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, arg2, arg3, arg4, obj);
    }

    SPxAsyncNS::DataVM4<R, Arg1, Arg2, Arg3, Arg4, O> *data = new SPxAsyncNS::DataVM4<R, Arg1, Arg2, Arg3, Arg4, O>(func, arg1, arg2, arg3, arg4, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskVM4<R, Arg1, Arg2, Arg3, Arg4, O>, data);
}

template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename O, typename OC>
inline SPxFuture<R> SPxThen(const SPxFuture<void> &future, R(O::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), typename SPxAddCrefIfNotRef<Arg1>::type arg1, typename SPxAddCrefIfNotRef<Arg2>::type arg2, typename SPxAddCrefIfNotRef<Arg3>::type arg3, typename SPxAddCrefIfNotRef<Arg4>::type arg4, typename SPxAddCrefIfNotRef<Arg5>::type arg5, OC *obj)
{
    if( future.ready() )
    {
	return SPxAsync(func, arg1, arg2, arg3, arg4, arg5, obj);
    }

    SPxAsyncNS::DataVM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O> *data = new SPxAsyncNS::DataVM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O>(func, arg1, arg2, arg3, arg4, arg5, static_cast<O*>(obj), future.m_state);
    return SPxAsync(SPxAsyncNS::TaskVM5<R, Arg1, Arg2, Arg3, Arg4, Arg5, O>, data);
}

/*********************************************************************
*
* SPxWhenAll/SPxWhenAny
*
**********************************************************************/

namespace SPxAsyncNS
{
    template<typename C>
    static void whenAllCont(typename C::const_iterator begin, typename C::const_iterator end)
    {
	for(typename C::const_iterator it = begin; it != end; ++it)
	{
	    it->wait();
	}
    }
    template<typename T0>
    static SPxTuple<SPxFuture<T0> > whenAll1(SPxTuple<SPxFuture<T0> > f)
    {
	SPxGet<0>(f).wait();
	return f;
    }
    template<typename T0, typename T1>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1> > whenAll2(SPxTuple<SPxFuture<T0>, SPxFuture<T1> > f)
    {
	SPxGet<0>(f).wait();
	SPxGet<1>(f).wait();
	return f;
    }
    template<typename T0, typename T1, typename T2>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > whenAll3(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > f)
    {
	SPxGet<0>(f).wait();
	SPxGet<1>(f).wait();
	SPxGet<2>(f).wait();
	return f;
    }
    template<typename T0, typename T1, typename T2, typename T3>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > whenAll4(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > f)
    {
	SPxGet<0>(f).wait();
	SPxGet<1>(f).wait();
	SPxGet<2>(f).wait();
	SPxGet<3>(f).wait();
	return f;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > whenAll5(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > f)
    {
	SPxGet<0>(f).wait();
	SPxGet<1>(f).wait();
	SPxGet<2>(f).wait();
	SPxGet<3>(f).wait();
	SPxGet<4>(f).wait();
	return f;
    }
    template<typename T0>
    static SPxTuple<SPxFuture<T0> > whenAny1(SPxTuple<SPxFuture<T0> > f)
    {
	SPxGet<0>(f).wait();
	return f;
    }
    template<typename T0, typename T1>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1> > whenAny2(SPxTuple<SPxFuture<T0>, SPxFuture<T1> > f)
    {
	SPxCriticalSection mutex(TRUE, TRUE);
	SPxAutoLock lock(mutex);
	SPxCondVar *condVars[2] = { &SPxGet<0>(f).m_state->cond, &SPxGet<1>(f).m_state->cond };
	SPxCondVarWaitAnyForever(mutex, 2, condVars);
	return f;
    }
    template<typename T0, typename T1, typename T2>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > whenAny3(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > f)
    {
	SPxCriticalSection mutex(TRUE, TRUE);
	SPxAutoLock lock(mutex);
	SPxCondVar *condVars[3] =
	{
	    &SPxGet<0>(f).m_state->cond,
	    &SPxGet<1>(f).m_state->cond,
	    &SPxGet<2>(f).m_state->cond
	};
	SPxCondVarWaitAnyForever(mutex, 3, condVars);
	return f;
    }
    template<typename T0, typename T1, typename T2, typename T3>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > whenAny4(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > f)
    {
	SPxCriticalSection mutex(TRUE, TRUE);
	SPxAutoLock lock(mutex);
	SPxCondVar *condVars[4] =
	{
	    &SPxGet<0>(f).m_state->cond,
	    &SPxGet<1>(f).m_state->cond,
	    &SPxGet<2>(f).m_state->cond,
	    &SPxGet<3>(f).m_state->evcondent
	};
	SPxCondVarWaitAnyForever(mutex, 4, condVars);
	return f;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    static SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > whenAny5(
	SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > f)
    {
	SPxCriticalSection mutex(TRUE, TRUE);
	SPxAutoLock lock(mutex);
	SPxCondVar *condVars[5] =
	{
	    &SPxGet<0>(f).m_state->cond,
	    &SPxGet<1>(f).m_state->cond,
	    &SPxGet<2>(f).m_state->cond,
	    &SPxGet<3>(f).m_state->cond,
	    &SPxGet<4>(f).m_state->cond
	};
	SPxCondVarWaitAnyForever(mutex, 5, condVars);
	return f;
    }
}

template<typename C>
inline SPxFuture<void> SPxWhenAll(const C& container)
{
    return SPxAsync(SPxAsyncNS::whenAllCont<C>, container.begin(), container.end());
}

template<typename T0>
inline SPxFuture<SPxTuple<SPxFuture<T0> > > SPxWhenAll(const SPxFuture<T0>& f0)
{
    return SPxAsync(SPxAsyncNS::whenAll1<T0>, SPxMakeTuple(f0));
}

template<typename T0, typename T1>
inline SPxFuture<SPxTuple<SPxFuture<T0>, SPxFuture<T1> > > SPxWhenAll(
    const SPxFuture<T0>& f0, const SPxFuture<T1>& f1)
{
    return SPxAsync(SPxAsyncNS::whenAll2<T0, T1>, SPxMakeTuple(f0, f1));
}

template<typename T0, typename T1, typename T2>
inline SPxFuture<SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > > SPxWhenAll(
    const SPxFuture<T0>& f0, const SPxFuture<T1>& f1, const SPxFuture<T2>& f2)
{
    return SPxAsync(SPxAsyncNS::whenAll3<T0, T1, T2>, SPxMakeTuple(f0, f1, f2));
}

template<typename T0, typename T1, typename T2, typename T3>
inline SPxFuture<SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > > SPxWhenAll(
    const SPxFuture<T0>& f0, const SPxFuture<T1>& f1,
    const SPxFuture<T2>& f2, const SPxFuture<T3>& f3)
{
    return SPxAsync(SPxAsyncNS::whenAll4<T0, T1, T2, T3>, SPxMakeTuple(f0, f1, f2, f3));
}

template<typename T0, typename T1, typename T2, typename T3, typename T4>
inline SPxFuture<SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > > SPxWhenAll(
    const SPxFuture<T0>& f0, const SPxFuture<T1>& f1, const SPxFuture<T2>& f2,
    const SPxFuture<T3>& f3, const SPxFuture<T4>& f4)
{
    return SPxAsync(SPxAsyncNS::whenAll5<T0, T1, T2, T3, T4>, SPxMakeTuple(f0, f1, f2, f3, f4));
}

template<typename T0>
inline SPxFuture<SPxTuple<SPxFuture<T0> > > SPxWhenAny(const SPxFuture<T0>& f0)
{
    return SPxAsync(SPxAsyncNS::whenAny1<T0>, SPxMakeTuple(f0));
}

template<typename T0, typename T1>
inline SPxFuture<SPxTuple<SPxFuture<T0>, SPxFuture<T1> > > SPxWhenAny(const SPxFuture<T0>& f0,
							       const SPxFuture<T1>& f1)
{
    return SPxAsync(SPxAsyncNS::whenAny2<T0, T1>, SPxMakeTuple(f0, f1));
}

template<typename T0, typename T1, typename T2>
inline SPxFuture<SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2> > > SPxWhenAny(
    const SPxFuture<T0>& f0, const SPxFuture<T1>& f1, const SPxFuture<T2>& f2)
{
    return SPxAsync(SPxAsyncNS::whenAny3<T0, T1, T2>, SPxMakeTuple(f0, f1, f2));
}

template<typename T0, typename T1, typename T2, typename T3>
inline SPxFuture<SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3> > > SPxWhenAny(
    const SPxFuture<T0>& f0, const SPxFuture<T1>& f1, const SPxFuture<T2>& f2,
    const SPxFuture<T3>& f3)
{
    return SPxAsync(SPxAsyncNS::whenAny4<T0, T1, T2, T3>, SPxMakeTuple(f0, f1, f2, f3));
}

template<typename T0, typename T1, typename T2, typename T3, typename T4>
inline SPxFuture<SPxTuple<SPxFuture<T0>, SPxFuture<T1>, SPxFuture<T2>, SPxFuture<T3>, SPxFuture<T4> > > SPxWhenAny(
    const SPxFuture<T0>& f0, const SPxFuture<T1>& f1, const SPxFuture<T2>& f2,
    const SPxFuture<T3>& f3, const SPxFuture<T4>& f4)
{
    return SPxAsync(SPxAsyncNS::whenAny5<T0, T1, T2, T3, T4>, SPxMakeTuple(f0, f1, f2, f3, f4));
}

#endif /* _SPX_ASYNC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
