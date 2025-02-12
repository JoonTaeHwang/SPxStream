/*********************************************************************
*
* (c) Copyright 2013 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPx event handler class.
*
*	This class is similar to the SPxCallbackList class, but with
*	a few enhancements.
*
*	1. Rather than static functions, member functions are installed.
*
*	2. There is no practical limit to the number of event handlers
*	that can be installed.
*
*	3. Currently a maximum of three arguments to each callback.
*
*
*	To use the event handler in a class:
*
*		class SPxClass
*		{
*		public:
*		    // This typedef is not required, but keeps the code tidy and
*		    // advertises the required handler function type to other classes.
*		    typedef SPxErrorCode(*HandlerType)(int arg1, const SPxTime_t *arg2);
*	
*		    // Add/Remove handler function must be called in header.
*		    // To support calling callbacks in any object, these must be templated.
*		    template<typename F, typename O>
*		    SPxErrorCode AddHandler(F fn, O *obj)
*		    {
*			// Arguments to Add() are checked at compile time.
*			return m_fns.Add(fn, obj);
*		    }
*		    template<typename F, typename O)
*		    SPxErrorCode RemoveHandler(F fn, O *obj)
*		    {
*			// Arguments to Remove() are checked at compile time.
*			return m_fns.Remove(fn, obj);
*		    }
*	
*		    // Calling the operator() function can be in the source file.
*		    SPxErrorCode Invoke(int arg1, const SPxTime_t *arg2)
*		    {
*			// Arguments to operator() are checked at compile time.
*			return m_fns(arg1, arg2);
*		    }
*	
*		private:
*		    SPxEventHandler<HandlerType> m_fns;
*		}
*
*	To consume the event handler:
*
*		class SPxOtherClass
*		{
*		public:
*		    void SomeFunc()
*		    {
*			// Arguments checked at compiler time.
*			// Function to install is a member NOT static function.
*			m_class->Add(this, &SPxOtherClass::Handler);
*		    }
*
*		private:
*		    SPxErrorCode Handler(int arg1, const SPxTime_t *arg2);
*		}
*
* Revision Control:
*   27/02/19 v1.14   AGC	Support 5 arguments.
*
* Previous Changes:
*   23/10/17 1.13   AGC	Support const member functions.
*   22/06/17 1.12   AGC	Fix undefined behaviour when handlers compared
*			from base/derived of same object.
*   29/04/16 1.11   AGC	Suppress remaining cppcheck warnings.
*   20/04/16 1.10   AGC	Supress erroneous warnings with cppcheck 1.73.
*   11/11/15 1.9    AGC	Fix undefined behaviour when comparing event handlers.
*   29/04/15 1.8    AGC	Make invocation function const.
*   17/03/15 1.7    AGC	Fix cppcheck warnings.
*			Support handlers with 4 arguments.
*   15/09/14 1.6    AGC	Add comparions functions.
*   26/06/14 1.5    AGC	Remove vector dependency.
*   17/02/14 1.4    AGC	Add Empty() function.
*			Tidy up example in this comment.
*			Use operator() for invocation.
*   27/11/13 1.3    AGC	Suppress erroneous cppcheck warnings.
*   26/11/13 1.2    AGC	Fix icc warnings.
*			Fix memory leak in Remove().
*   22/11/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_EVENT_HANDLER_H
#define _SPX_EVENT_HANDLER_H

/* For SPxAutoLock. */
#include "SPxLibUtils/SPxAutoLock.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxSharedPtr. */
#include "SPxLibUtils/SPxSharedPtr.h"

/* This class is required to support void and non-void return types. */
template<typename R>
class SPxEventHandlerRet
{
public:
    SPxEventHandlerRet() : m_val() {}
    SPxEventHandlerRet(R val) : m_val(val) {}
    SPxEventHandlerRet& operator=(R val)
    {
	m_val = val;
	return *this;
    }
    operator R() { return m_val; }
private:
    R m_val;
};

template<>
class SPxEventHandlerRet<void>
{
public:
    SPxEventHandlerRet() {}
};

template<typename T>
class SPxEventHandlerCtr
{
public:
    SPxEventHandlerCtr()
	: m_size(0), m_capacity(0), m_data() {}
    virtual ~SPxEventHandlerCtr() {}

    void push_back(T *h)
    {
	if( m_capacity <= m_size )
	{
	    int newCapacity = (m_size + 1) * 2;
	    SPxAutoPtr<SPxSharedPtr<T>[]> newData(new SPxSharedPtr<T>[newCapacity]);
	    for(int i = 0; i < m_size; ++i)
	    {
		newData[i] = m_data[i];
	    }
	    m_data.reset(newData.release());
	}
	m_data[m_size].reset(h);
	++m_size;
    }
    void erase(int index)
    {
	for(int i = index; i < (m_size - 1); ++i)
	{
	    m_data[i] = m_data[i+1];
	}
	if( m_size > 0 )
	{
	    m_data[m_size-1].reset();
	}
	--m_size;
    }
    T *operator[](int index)
    {
	return m_data[index];
    }
    const T *operator[](int index) const
    {
	return m_data[index];
    }
    int size() const { return m_size; }
    bool empty() const { return (m_size == 0); }
    void clear()
    {
	m_size = 0;
	m_capacity = 0;
	m_data.reset();
    }

private:
    int m_size;
    int m_capacity;
    SPxAutoPtr<SPxSharedPtr<T>[]> m_data;
};

/* Initial class declaration. */
template<typename F> class SPxEventHandler;

/* Specialise class for function with no arguments. */
template<typename R>
class SPxEventHandler<R(*)()>
{
public:
    SPxEventHandler() : m_mutex(TRUE), m_fns() {}
    virtual ~SPxEventHandler() {}

    SPxErrorCode Add(R (*fn)(void*), void* userArg)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new HandlerBase(fn, userArg));
	return SPX_NO_ERROR;
    }

    SPxErrorCode Remove(R (*fn)(void*), void* userArg)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if( h && (h->GetObj() == userArg) && (h->m_fn == fn) )
	    {
		m_fns.erase(i);
		return err;
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }

    template<typename O>
    SPxErrorCode Add(R (O::*fn)(), O *obj)
    {
	return add<O, R(O::*)(), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Add(R(O::*fn)() const, O *obj)
    {
	return add<O, R(O::*)() const, true>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R(O::*fn)(), O *obj)
    {
	return remove<O, R(O::*)(), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R (O::*fn)() const, O *obj)
    {
	return remove<O, R(O::*)() const, true>(fn, obj);
    }

    R operator()() const
    {
	SPxEventHandlerRet<R> ret;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    const HandlerBase *h = m_fns[i];
	    if( h )
	    {
		invoke(ret, h);
	    }
	}

	return static_cast<R>(ret);
    }

    bool Empty() const
    {
	return m_fns.empty();
    }

    void Clear()
    {
	SPxAutoLock lock(m_mutex);
	m_fns.clear();
    }

    bool operator==(const SPxEventHandler<R(*)()>& rhs) const
    {
	if( m_fns.size() != rhs.m_fns.size() )
	{
	    return false;
	}
	for(int i = 0; i < m_fns.size(); ++i)
	{
	    if( m_fns[i]->GetObj() != rhs.m_fns[i]->GetObj() )
	    {
		return false;
	    }

	    if( *m_fns[i] != *rhs.m_fns[i] )
	    {
		return false;
	    }
	}

	return true;
    }

    bool operator!=(const SPxEventHandler<R(*)()>& rhs) const
    {
	return !operator==(rhs);
    }

private:
    struct HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	HandlerBase(R (*fn)(void*)=NULL, void *userArg=NULL) : m_fn(fn), m_userArg(userArg) {}
	virtual ~HandlerBase() {}
	virtual R Invoke() const { return m_fn(m_userArg); }
	virtual void *GetObj() const { return m_userArg; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(m_userArg)); }
	virtual bool IsStatic() const { return true; }
	virtual bool IsConst() const { return false; }
	virtual bool operator==(const HandlerBase& rhs) const
	{
	    return (m_fn == rhs.m_fn) && (m_userArg == rhs.m_userArg);
	}
	bool operator!=(const HandlerBase& rhs) const
	{
	    return !operator==(rhs);
	}

	R (*m_fn)(void*);
	void *m_userArg;
    };
    template<typename O, typename F, bool C>
    struct Handler : public HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	Handler(O *_obj, F fn) : m_obj(_obj), m_mfn(fn) {}
	virtual ~Handler() {}
	virtual R Invoke() const { return (m_obj->*m_mfn)(); }
	virtual void *GetObj() const { return m_obj; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(O)); }
	virtual bool IsStatic() const { return false; }
	virtual bool IsConst() const { return C; }
	bool HasFn(F fn) const { return fn == m_mfn; }

	bool operator==(const HandlerBase& rhs) const
	{
	    if( (rhs.IsStatic() != IsStatic()) ||
		(rhs.GetObj() != GetObj()) ||
	        (rhs.GetObjSize() != GetObjSize()) ||
	        (rhs.IsConst() != IsConst()))
	    {
		return false;
	    }
	    const Handler<O, F, C> &h = static_cast<const Handler<O, F, C>&>(rhs);
	    return HasFn(h.m_mfn);
	}
	
	O *m_obj;
	F m_mfn;
    };

    mutable SPxCriticalSection m_mutex;
    SPxEventHandlerCtr<HandlerBase> m_fns;

    void invoke(SPxEventHandlerRet<void>&, const HandlerBase* h) const
    { h->Invoke(); }
    template<typename Ret>
    void invoke(SPxEventHandlerRet<Ret>& ret, const HandlerBase* h) const
    { ret = h->Invoke(); }

    template<typename O, typename F, bool C>
    SPxErrorCode add(F fn, O *obj)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new Handler<O, F, C>(obj, fn));
	return SPX_NO_ERROR;
    }

    template<typename O, typename F, bool C>
    SPxErrorCode remove(F fn, O *obj)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if(h && (h->GetObj() == obj) && (h->IsConst() == C))
	    {
		Handler<O, F, C>* o = static_cast<Handler<O, F, C>*>(h);
		if(o->HasFn(fn))
		{
		    m_fns.erase(i);
		    return err;
		}
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }
};

/* Specialise class for function with 1 argument. */
template<typename R, typename Arg1>
class SPxEventHandler<R(*)(Arg1)>
{
public:
    SPxEventHandler() : m_mutex(TRUE), m_fns() {}
    virtual ~SPxEventHandler() {}

    SPxErrorCode Add(R (*fn)(Arg1, void*), void* userArg)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new HandlerBase(fn, userArg));
	return SPX_NO_ERROR;
    }

    SPxErrorCode Remove(R (*fn)(Arg1, void*), void* userArg)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if( h && (h->GetObj() == userArg) && (h->m_fn == fn) )
	    {
		m_fns.erase(i);
		return err;
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }

    template<typename O>
    SPxErrorCode Add(R (O::*fn)(Arg1), O *obj)
    {
	return add<O, R(O::*)(Arg1), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Add(R(O::*fn)(Arg1) const, O *obj)
    {
	return add<O, R(O::*)(Arg1) const, true>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R (O::*fn)(Arg1), O *obj)
    {
	return remove<O, R(O::*)(Arg1), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R(O::*fn)(Arg1) const, O *obj)
    {
	return remove<O, R(O::*)(Arg1) const, true>(fn, obj);
    }

    R operator()(Arg1 arg1) const
    {
	SPxEventHandlerRet<R> ret;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    const HandlerBase *h = m_fns[i];
	    if(h)
	    {
		invoke(ret, h, arg1);
	    }
	}

	return static_cast<R>(ret);
    }

    bool Empty() const
    {
	return m_fns.empty();
    }

    void Clear()
    {
	SPxAutoLock lock(m_mutex);
	m_fns.clear();
    }

    bool operator==(const SPxEventHandler<R(*)(Arg1)>& rhs) const
    {
	if( m_fns.size() != rhs.m_fns.size() )
	{
	    return false;
	}
	for(int i = 0; i < m_fns.size(); ++i)
	{
	    if( m_fns[i]->GetObj() != rhs.m_fns[i]->GetObj() )
	    {
		return false;
	    }

	    if( *m_fns[i] != *rhs.m_fns[i] )
	    {
		return false;
	    }
	}

	return true;
    }

    bool operator!=(const SPxEventHandler<R(*)(Arg1)>& rhs) const
    {
	return !operator==(rhs);
    }

private:
    struct HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	HandlerBase(R (*fn)(Arg1, void*)=NULL, void* userArg=NULL) : m_fn(fn), m_userArg(userArg) {}
	virtual ~HandlerBase() {}
	virtual R Invoke(Arg1 arg1) const { return m_fn(arg1, m_userArg); }
	virtual void *GetObj() const { return m_userArg; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(m_userArg)); }
	virtual bool IsStatic() const { return true; }
	virtual bool IsConst() const { return false; }
	virtual bool operator==(const HandlerBase& rhs) const
	{
	    return (m_fn == rhs.m_fn) && (m_userArg == rhs.m_userArg);
	}
	bool operator!=(const HandlerBase& rhs) const
	{
	    return !operator==(rhs);
	}

	R (*m_fn)(Arg1, void*);
	void *m_userArg;
    };
    template<typename O, typename F, bool C>
    struct Handler : public HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	Handler(O *_obj, F fn) : m_obj(_obj), m_mfn(fn) {}
	virtual ~Handler() {}
	virtual R Invoke(Arg1 arg1) const { return (m_obj->*m_mfn)(arg1); }
	virtual void *GetObj() const { return m_obj; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(O)); }
	virtual bool IsStatic() const { return false; }
	virtual bool IsConst() const { return C; }
	bool HasFn(F fn) const { return fn == m_mfn; }
	
	bool operator==(const HandlerBase& rhs) const
	{
	    if ((rhs.IsStatic() != IsStatic()) ||
		(rhs.GetObj() != GetObj()) ||
		(rhs.GetObjSize() != GetObjSize()) ||
		(rhs.IsConst() != IsConst()))
	    {
		return false;
	    }
	    const Handler<O, F, C> &h = static_cast<const Handler<O, F, C>&>(rhs);
	    return HasFn(h.m_mfn);
	}

	O *m_obj;
	F m_mfn;
    };

    mutable SPxCriticalSection m_mutex;
    SPxEventHandlerCtr<HandlerBase> m_fns;

    void invoke(SPxEventHandlerRet<void>&, const HandlerBase* h, Arg1 arg1) const
    { h->Invoke(arg1); }
    template<typename Ret>
    void invoke(SPxEventHandlerRet<Ret>& ret, const HandlerBase* h, Arg1 arg1) const
    { ret = h->Invoke(arg1); }

    template<typename O, typename F, bool C>
    SPxErrorCode add(F fn, O *obj)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new Handler<O, F, C>(obj, fn));
	return SPX_NO_ERROR;
    }

    template<typename O, typename F, bool C>
    SPxErrorCode remove(F fn, O *obj)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if(h && (h->GetObj() == obj) && (h->IsConst() == C))
	    {
		Handler<O, F, C>* o = static_cast<Handler<O, F, C>*>(h);
		if(o->HasFn(fn))
		{
		    m_fns.erase(i);
		    return err;
		}
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }
};

/* Specialise class for function with 2 arguments. */
template<typename R, typename Arg1, typename Arg2>
class SPxEventHandler<R(*)(Arg1, Arg2)>
{
public:
    SPxEventHandler() : m_mutex(TRUE), m_fns() {}
    virtual ~SPxEventHandler() {}

    SPxErrorCode Add(R (*fn)(Arg1, Arg2, void*), void *userArg)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new HandlerBase(fn, userArg));
	return SPX_NO_ERROR;
    }

    SPxErrorCode Remove(R (*fn)(Arg1, Arg2, void*), void *userArg)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if( h && (h->GetObj() == userArg) && (h->m_fn == fn) )
	    {
		m_fns.erase(i);
		return err;
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }

    template<typename O>
    SPxErrorCode Add(R (O::*fn)(Arg1, Arg2), O *obj)
    {
	return add<O, R(O::*)(Arg1, Arg2), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Add(R(O::*fn)(Arg1, Arg2) const, O *obj)
    {
	return add<O, R(O::*)(Arg1, Arg2) const, true>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R(O::*fn)(Arg1, Arg2), O *obj)
    {
	return remove<O, R(O::*)(Arg1, Arg2), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R (O::*fn)(Arg1, Arg2) const, O *obj)
    {
	return remove<O, R(O::*)(Arg1, Arg2) const, true>(fn, obj);
    }

    R operator()(Arg1 arg1, Arg2 arg2) const
    {
	SPxEventHandlerRet<R> ret;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    const HandlerBase *h = m_fns[i];
	    if( h )
	    {
		invoke(ret, h, arg1, arg2);
	    }
	}

	return static_cast<R>(ret);
    }

    bool Empty() const
    {
	return m_fns.empty();
    }

    void Clear()
    {
	SPxAutoLock lock(m_mutex);
	m_fns.clear();
    }

    bool operator==(const SPxEventHandler<R(*)(Arg1, Arg2)>& rhs) const
    {
	if( m_fns.size() != rhs.m_fns.size() )
	{
	    return false;
	}
	for(int i = 0; i < m_fns.size(); ++i)
	{
	    if( m_fns[i]->GetObj() != rhs.m_fns[i]->GetObj() )
	    {
		return false;
	    }

	    if( *m_fns[i] != *rhs.m_fns[i] )
	    {
		return false;
	    }
	}

	return true;
    }

    bool operator!=(const SPxEventHandler<R(*)(Arg1, Arg2)>& rhs) const
    {
	return !operator==(rhs);
    }

private:
    struct HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	HandlerBase(R (*fn)(Arg1, Arg2, void*)=NULL, void* userArg=NULL) : m_fn(fn), m_userArg(userArg) {}
	virtual ~HandlerBase() {}
	virtual R Invoke(Arg1 arg1, Arg2 arg2) const { return m_fn(arg1, arg2, m_userArg); }
	virtual void *GetObj() const { return m_userArg; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(m_userArg)); }
	virtual bool IsStatic() const { return true; }
	virtual bool IsConst() const { return false; }
	virtual bool operator==(const HandlerBase& rhs) const
	{
	    return (m_fn == rhs.m_fn) && (m_userArg == rhs.m_userArg);
	}
	bool operator!=(const HandlerBase& rhs) const
	{
	    return !operator==(rhs);
	}

	R (*m_fn)(Arg1, Arg2, void*);
	void *m_userArg;
    };
    template<typename O, typename F, bool C>
    struct Handler : public HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	Handler(O *_obj, F fn) : m_obj(_obj), m_mfn(fn) {}
	virtual ~Handler() {}
	virtual R Invoke(Arg1 arg1, Arg2 arg2) const { return (m_obj->*m_mfn)(arg1, arg2); }
	virtual void *GetObj() const { return m_obj; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(O)); }
	virtual bool IsStatic() const { return false; }
	virtual bool IsConst() const { return C; }
	bool HasFn(F fn) const { return fn == m_mfn; }
	
	bool operator==(const HandlerBase& rhs) const
	{
	    if ((rhs.IsStatic() != IsStatic()) ||
		(rhs.GetObj() != GetObj()) ||
		(rhs.GetObjSize() != GetObjSize()))
	    {
		return false;
	    }
	    const Handler<O, F, C> &h = static_cast<const Handler<O, F, C>&>(rhs);
	    return HasFn(h.m_mfn);
	}

	O *m_obj;
	F m_mfn;
    };

    mutable SPxCriticalSection m_mutex;
    SPxEventHandlerCtr<HandlerBase> m_fns;

    void invoke(SPxEventHandlerRet<void>&, const HandlerBase* h, Arg1 arg1, Arg2 arg2) const
    { h->Invoke(arg1, arg2); }
    template<typename Ret>
    void invoke(SPxEventHandlerRet<Ret>& ret, const HandlerBase* h, Arg1 arg1, Arg2 arg2) const
    { ret = h->Invoke(arg1, arg2); }
    
    template<typename O, typename F, bool C>
    SPxErrorCode add(F fn, O *obj)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new Handler<O, F, C>(obj, fn));
	return SPX_NO_ERROR;
    }

    template<typename O, typename F, bool C>
    SPxErrorCode remove(F fn, O *obj)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if(h && (h->GetObj() == obj) && (h->IsConst() == C))
	    {
		Handler<O, F, C>* o = static_cast<Handler<O, F, C>*>(h);
		if(o->HasFn(fn))
		{
		    m_fns.erase(i);
		    return err;
		}
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }
};

/* Specialise class for function with 3 arguments. */
template<typename R, typename Arg1, typename Arg2, typename Arg3>
class SPxEventHandler<R(*)(Arg1, Arg2, Arg3)>
{
public:
    SPxEventHandler() : m_mutex(TRUE), m_fns() {}
    virtual ~SPxEventHandler() {}

    SPxErrorCode Add(R (*fn)(Arg1, Arg2, Arg3, void*), void* userArg)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new HandlerBase(fn, userArg));
	return SPX_NO_ERROR;
    }

    SPxErrorCode Remove(R (*fn)(Arg1, Arg2, Arg3, void*), void* userArg)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if( h && (h->GetObj() == userArg) && (h->m_fn == fn) )
	    {
		m_fns.erase(i);
		return err;
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }

    template<typename O>
    SPxErrorCode Add(R (O::*fn)(Arg1, Arg2, Arg3), O *obj)
    {
	return add<O, R(O::*)(Arg1, Arg2, Arg3), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Add(R(O::*fn)(Arg1, Arg2, Arg3) const, O *obj)
    {
	return add<O, R(O::*)(Arg1, Arg2, Arg3) const, true>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R(O::*fn)(Arg1, Arg2, Arg3), O *obj)
    {
	return remove<O, R(O::*)(Arg1, Arg2, Arg3), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R (O::*fn)(Arg1, Arg2, Arg3) const, O *obj)
    {
	return remove<O, R(O::*)(Arg1, Arg2, Arg3) const, true>(fn, obj);
    }

    R operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3) const
    {
	SPxEventHandlerRet<R> ret;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    const HandlerBase *h = m_fns[i];
	    if( h )
	    {
		invoke(ret, h, arg1, arg2, arg3);
	    }
	}

	return static_cast<R>(ret);
    }

    bool Empty() const
    {
	return m_fns.empty();
    }

    void Clear()
    {
	SPxAutoLock lock(m_mutex);
	m_fns.clear();
    }

    bool operator==(const SPxEventHandler<R(*)(Arg1, Arg2, Arg3)>& rhs) const
    {
	if( m_fns.size() != rhs.m_fns.size() )
	{
	    return false;
	}
	for(int i = 0; i < m_fns.size(); ++i)
	{
	    if( m_fns[i]->GetObj() != rhs.m_fns[i]->GetObj() )
	    {
		return false;
	    }

	    if( *m_fns[i] != *rhs.m_fns[i] )
	    {
		return false;
	    }
	}

	return true;
    }

    bool operator!=(const SPxEventHandler<R(*)(Arg1, Arg2, Arg3)>& rhs) const
    {
	return !operator==(rhs);
    }

private:
    struct HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	HandlerBase(R (*fn)(Arg1, Arg2, Arg3, void*)=NULL, void* userArg=NULL) : m_fn(fn), m_userArg(userArg) {}
	virtual ~HandlerBase() {}
	virtual R Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3) const { return m_fn(arg1, arg2, arg3, m_userArg); }
	virtual void *GetObj() const { return m_userArg; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(m_userArg)); }
	virtual bool IsStatic() const { return true; }
	virtual bool IsConst() const { return false; }
	virtual bool operator==(const HandlerBase& rhs) const
	{
	    return (m_fn == rhs.m_fn) && (m_userArg == rhs.m_userArg);
	}
	bool operator!=(const HandlerBase& rhs) const
	{
	    return !operator==(rhs);
	}

	R (*m_fn)(Arg1, Arg2, Arg3, void*);
	void *m_userArg;
    };
    template<typename O, typename F, bool C>
    struct Handler : public HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	Handler(O *_obj, F fn) : m_obj(_obj), m_mfn(fn) {}
	virtual ~Handler() {}
	virtual R Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3) const { return (m_obj->*m_mfn)(arg1, arg2, arg3); }
	virtual void *GetObj() const { return m_obj; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(O)); }
	virtual bool IsStatic() const { return false; }
	virtual bool IsConst() const { return C; }
	bool HasFn(F fn) const { return fn == m_mfn; }
	
	bool operator==(const HandlerBase& rhs) const
	{
	    if ((rhs.IsStatic() != IsStatic()) ||
		(rhs.GetObj() != GetObj()) ||
		(rhs.GetObjSize() != GetObjSize()) ||
		(rhs.IsConst() != IsConst()))
	    {
		return false;
	    }
	    const Handler<O, F, C> &h = static_cast<const Handler<O, F, C>&>(rhs);
	    return HasFn(h.m_mfn);
	}

	O *m_obj;
	F m_mfn;
    };

    mutable SPxCriticalSection m_mutex;
    SPxEventHandlerCtr<HandlerBase> m_fns;

    void invoke(SPxEventHandlerRet<void>&, const HandlerBase* h, Arg1 arg1, Arg2 arg2, Arg3 arg3) const
    { h->Invoke(arg1, arg2, arg3); }
    template<typename Ret>
    void invoke(SPxEventHandlerRet<Ret>& ret, const HandlerBase* h, Arg1 arg1, Arg2 arg2, Arg3 arg3) const
    { ret = h->Invoke(arg1, arg2, arg3); }

    template<typename O, typename F, bool C>
    SPxErrorCode add(F fn, O *obj)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new Handler<O, F, C>(obj, fn));
	return SPX_NO_ERROR;
    }

    template<typename O, typename F, bool C>
    SPxErrorCode remove(F fn, O *obj)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if(h && (h->GetObj() == obj) && (h->IsConst() == C))
	    {
		Handler<O, F, C>* o = static_cast<Handler<O, F, C>*>(h);
		if(o->HasFn(fn))
		{
		    m_fns.erase(i);
		    return err;
		}
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }
};

/* Specialise class for function with 4 arguments. */
template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
class SPxEventHandler<R(*)(Arg1, Arg2, Arg3, Arg4)>
{
public:
    SPxEventHandler() : m_mutex(TRUE), m_fns() {}
    virtual ~SPxEventHandler() {}

    SPxErrorCode Add(R (*fn)(Arg1, Arg2, Arg3, Arg4, void*), void* userArg)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new HandlerBase(fn, userArg));
	return SPX_NO_ERROR;
    }

    SPxErrorCode Remove(R (*fn)(Arg1, Arg2, Arg3, Arg4, void*), void* userArg)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if( h && (h->GetObj() == userArg) && (h->m_fn == fn) )
	    {
		m_fns.erase(i);
		return err;
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }

    template<typename O>
    SPxErrorCode Add(R (O::*fn)(Arg1, Arg2, Arg3, Arg4), O *obj)
    {
	return add<O, R(O::*)(Arg1, Arg2, Arg3, Arg4), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Add(R(O::*fn)(Arg1, Arg2, Arg3, Arg4) const, O *obj)
    {
	return add<O, R(O::*)(Arg1, Arg2, Arg3, Arg4) const, true>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R(O::*fn)(Arg1, Arg2, Arg3, Arg4), O *obj)
    {
	return remove<O, R(O::*)(Arg1, Arg2, Arg3, Arg4), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R (O::*fn)(Arg1, Arg2, Arg3, Arg4) const, O *obj)
    {
	return remove<O, R(O::*)(Arg1, Arg2, Arg3, Arg4) const, true>(fn, obj);
    }

    R operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) const
    {
	SPxEventHandlerRet<R> ret;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    const HandlerBase *h = m_fns[i];
	    if( h )
	    {
		invoke(ret, h, arg1, arg2, arg3, arg4);
	    }
	}

	return static_cast<R>(ret);
    }

    bool Empty() const
    {
	return m_fns.empty();
    }

    void Clear()
    {
	SPxAutoLock lock(m_mutex);
	m_fns.clear();
    }

    bool operator==(const SPxEventHandler<R(*)(Arg1, Arg2, Arg3, Arg4)>& rhs) const
    {
	if( m_fns.size() != rhs.m_fns.size() )
	{
	    return false;
	}
	for(int i = 0; i < m_fns.size(); ++i)
	{
	    if( m_fns[i]->GetObj() != rhs.m_fns[i]->GetObj() )
	    {
		return false;
	    }

	    if( *m_fns[i] != *rhs.m_fns[i] )
	    {
		return false;
	    }
	}

	return true;
    }

    bool operator!=(const SPxEventHandler<R(*)(Arg1, Arg2, Arg3, Arg4)>& rhs) const
    {
	return !operator==(rhs);
    }

private:
    struct HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	HandlerBase(R (*fn)(Arg1, Arg2, Arg3, Arg4, void*)=NULL, void* userArg=NULL) : m_fn(fn), m_userArg(userArg) {}
	virtual ~HandlerBase() {}
	virtual R Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) const { return m_fn(arg1, arg2, arg3, arg4, m_userArg); }
	virtual void *GetObj() const { return m_userArg; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(m_userArg)); }
	virtual bool IsStatic() const { return true; }
	virtual bool IsConst() const { return false; }
	virtual bool operator==(const HandlerBase& rhs) const
	{
	    return (m_fn == rhs.m_fn) && (m_userArg == rhs.m_userArg);
	}
	bool operator!=(const HandlerBase& rhs) const
	{
	    return !operator==(rhs);
	}

	R (*m_fn)(Arg1, Arg2, Arg3, Arg4, void*);
	void *m_userArg;
    };
    template<typename O, typename F, bool C>
    struct Handler : public HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	Handler(O *_obj, F fn) : m_obj(_obj), m_mfn(fn) {}
	virtual ~Handler() {}
	virtual R Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) const { return (m_obj->*m_mfn)(arg1, arg2, arg3, arg4); }
	virtual void *GetObj() const { return m_obj; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(O)); }
	virtual bool IsStatic() const { return false; }
	virtual bool IsConst() const { return C; }
	bool HasFn(F fn) const { return fn == m_mfn; }
	
	bool operator==(const HandlerBase& rhs) const
	{
	    if ((rhs.IsStatic() != IsStatic()) ||
		(rhs.GetObj() != GetObj()) ||
		(rhs.GetObjSize() != GetObjSize()) ||
		(rhs.IsConst() != IsConst()))
	    {
		return false;
	    }
	    const Handler<O, F, C> &h = static_cast<const Handler<O, F, C>&>(rhs);
	    return HasFn(h.m_mfn);
	}

	O *m_obj;
	F m_mfn;
    };

    mutable SPxCriticalSection m_mutex;
    SPxEventHandlerCtr<HandlerBase> m_fns;

    void invoke(SPxEventHandlerRet<void>&, const HandlerBase* h, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) const
    { h->Invoke(arg1, arg2, arg3, arg4); }
    template<typename Ret>
    void invoke(SPxEventHandlerRet<Ret>& ret, const HandlerBase* h, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) const
    { ret = h->Invoke(arg1, arg2, arg3, arg4); }

    template<typename O, typename F, bool C>
    SPxErrorCode add(F fn, O *obj)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new Handler<O, F, C>(obj, fn));
	return SPX_NO_ERROR;
    }

    template<typename O, typename F, bool C>
    SPxErrorCode remove(F fn, O *obj)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if(h && (h->GetObj() == obj) && (h->IsConst() == C))
	    {
		Handler<O, F, C>* o = static_cast<Handler<O, F, C>*>(h);
		if(o->HasFn(fn))
		{
		    m_fns.erase(i);
		    return err;
		}
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }
};

/* Specialise class for function with 5 arguments. */
template<typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
class SPxEventHandler<R(*)(Arg1, Arg2, Arg3, Arg4, Arg5)>
{
public:
    SPxEventHandler() : m_mutex(TRUE), m_fns() {}
    virtual ~SPxEventHandler() {}

    SPxErrorCode Add(R(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5, void*), void* userArg)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new HandlerBase(fn, userArg));
	return SPX_NO_ERROR;
    }

    SPxErrorCode Remove(R(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5, void*), void* userArg)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if(h && (h->GetObj() == userArg) && (h->m_fn == fn))
	    {
		m_fns.erase(i);
		return err;
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }

    template<typename O>
    SPxErrorCode Add(R(O::*fn)(Arg1, Arg2, Arg3, Arg4, Arg5), O *obj)
    {
	return add<O, R(O::*)(Arg1, Arg2, Arg3, Arg4, Arg5), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Add(R(O::*fn)(Arg1, Arg2, Arg3, Arg4, Arg5) const, O *obj)
    {
	return add<O, R(O::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const, true>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R(O::*fn)(Arg1, Arg2, Arg3, Arg4, Arg5), O *obj)
    {
	return remove<O, R(O::*)(Arg1, Arg2, Arg3, Arg4, Arg5), false>(fn, obj);
    }

    template<typename O>
    SPxErrorCode Remove(R(O::*fn)(Arg1, Arg2, Arg3, Arg4, Arg5) const, O *obj)
    {
	return remove<O, R(O::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const, true>(fn, obj);
    }

    R operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) const
    {
	SPxEventHandlerRet<R> ret;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    const HandlerBase *h = m_fns[i];
	    if(h)
	    {
		invoke(ret, h, arg1, arg2, arg3, arg4, arg5);
	    }
	}

	return static_cast<R>(ret);
    }

    bool Empty() const
    {
	return m_fns.empty();
    }

    void Clear()
    {
	SPxAutoLock lock(m_mutex);
	m_fns.clear();
    }

    bool operator==(const SPxEventHandler<R(*)(Arg1, Arg2, Arg3, Arg4, Arg5)>& rhs) const
    {
	if(m_fns.size() != rhs.m_fns.size())
	{
	    return false;
	}
	for(int i = 0; i < m_fns.size(); ++i)
	{
	    if(m_fns[i]->GetObj() != rhs.m_fns[i]->GetObj())
	    {
		return false;
	    }

	    if(*m_fns[i] != *rhs.m_fns[i])
	    {
		return false;
	    }
	}

	return true;
    }

    bool operator!=(const SPxEventHandler<R(*)(Arg1, Arg2, Arg3, Arg4, Arg5)>& rhs) const
    {
	return !operator==(rhs);
    }

private:
    struct HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	HandlerBase(R(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5, void*) = NULL, void* userArg = NULL) : m_fn(fn), m_userArg(userArg) {}
	virtual ~HandlerBase() {}
	virtual R Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) const { return m_fn(arg1, arg2, arg3, arg4, arg5, m_userArg); }
	virtual void *GetObj() const { return m_userArg; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(m_userArg)); }
	virtual bool IsStatic() const { return true; }
	virtual bool IsConst() const { return false; }
	virtual bool operator==(const HandlerBase& rhs) const
	{
	    return (m_fn == rhs.m_fn) && (m_userArg == rhs.m_userArg);
	}
	bool operator!=(const HandlerBase & rhs) const
	{
	    return !operator==(rhs);
	}

	R(*m_fn)(Arg1, Arg2, Arg3, Arg4, Arg5, void*);
	void *m_userArg;
    };
    template<typename O, typename F, bool C>
    struct Handler : public HandlerBase
    {
	// cppcheck-suppress uninitMemberVar
	Handler(O *_obj, F fn) : m_obj(_obj), m_mfn(fn) {}
	virtual ~Handler() {}
	virtual R Invoke(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) const { return (m_obj->*m_mfn)(arg1, arg2, arg3, arg4, arg5); }
	virtual void *GetObj() const { return m_obj; }
	virtual unsigned int GetObjSize() const { return static_cast<unsigned int>(sizeof(O)); }
	virtual bool IsStatic() const { return false; }
	virtual bool IsConst() const { return C; }
	bool HasFn(F fn) const { return fn == m_mfn; }

	bool operator==(const HandlerBase& rhs) const
	{
	    if((rhs.IsStatic() != IsStatic()) ||
		(rhs.GetObj() != GetObj()) ||
	       (rhs.GetObjSize() != GetObjSize()) ||
	       (rhs.IsConst() != IsConst()))
	    {
		return false;
	    }
	    const Handler<O, F, C> &h = static_cast<const Handler<O, F, C>&>(rhs);
	    return HasFn(h.m_mfn);
	}

	O *m_obj;
	F m_mfn;
    };

    mutable SPxCriticalSection m_mutex;
    SPxEventHandlerCtr<HandlerBase> m_fns;

    void invoke(SPxEventHandlerRet<void>&, const HandlerBase* h, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) const
    {
	h->Invoke(arg1, arg2, arg3, arg4, arg5);
    }
    template<typename Ret>
    void invoke(SPxEventHandlerRet<Ret>& ret, const HandlerBase* h, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) const
    {
	ret = h->Invoke(arg1, arg2, arg3, arg4, arg5);
    }

    template<typename O, typename F, bool C>
    SPxErrorCode add(F fn, O *obj)
    {
	SPxAutoLock lock(m_mutex);
	m_fns.push_back(new Handler<O, F, C>(obj, fn));
	return SPX_NO_ERROR;
    }

    template<typename O, typename F, bool C>
    SPxErrorCode remove(F fn, O *obj)
    {
	SPxErrorCode err = SPX_NO_ERROR;

	SPxAutoLock lock(m_mutex);

	for(int i = 0; i < m_fns.size(); ++i)
	{
	    HandlerBase *h = m_fns[i];
	    if(h && (h->GetObj() == obj) && (h->IsConst() == C))
	    {
		Handler<O, F, C>* o = static_cast<Handler<O, F, C>*>(h);
		if(o->HasFn(fn))
		{
		    m_fns.erase(i);
		    return err;
		}
	    }
	}

	err = SPX_ERR_BUFFER_TOO_SMALL;
	SPxError(SPX_ERR_USER, err, 0, 0,
		 "Callback not found");
	return err;
    }
};

#endif /* _SPX_EVENT_HANDLER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
