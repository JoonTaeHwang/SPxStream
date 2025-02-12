/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPx Shared Ptr class that automatically deletes
*   an object on once all shared pointers pointing to the same object
*   are destroyed.
*
*   Copying an SPxSharedPtr increases the reference count.
*
*   SPxSharedPtr objects may be stored in standard library containers,
*   unlike SPxAutoPtr objects.
*
*   Example:
*
*	SPxSharedPtr<SPxObj> obj1(new SPxObj());
*	SPxSharedPtr<SPxObj> obj2 = obj1; // Copy.
*
*	obj1->SetName(); // Accesses underlying pointer.
*
*	// SPxObj deleted using standard delete operator on 
*	// destruction of 'obj2' and 'obj1'.
*
*
* Revision Control:
*   26/08/22 v1.17   AGC	Workaround cppcheck parsing issues.
*
* Previous Changes:
*   26/02/21 1.16   AGC	Support use with custom deleter when destructor hidden.
*   30/10/20 1.15   AGC	Workaround cppcheck parsing issues.
*   14/10/19 1.14   AGC	Suppress cppcheck warning.
*   16/07/18 1.13   AGC	Add nine/ten argument SPxMakeShared().
*   20/04/17 1.12   AGC	Add eight argument SPxMakeShared().
*   31/01/17 1.11   AGC	Add seven argument SPxMakeShared().
*   23/09/16 1.10   AGC	Add six argument SPxMakeShared().
*   24/08/16 1.9    AGC	Avoid VS2010 warnings from stdint.h.
*   23/08/16 1.8    AGC	Use new version of SPxAtomic.
*   18/05/16 1.7    AGC	Use correct alignment in SPxMakeShared().
*   26/06/14 1.6    AGC	Need to include <new> header.
*   24/02/14 1.5    AGC	Explicitly disallow self assignment.
* 			Prevent accidental creation of SPxSharedPtr for array.
* 			Remove const from internal destroy() function.
*   17/02/14 1.4    AGC	Add SPxMakeShared() overloads with 4 or 5 args.
*   15/01/14 1.3    AGC	Use correct default deleter for SPxSharedPtr.
*   26/11/13 1.2    AGC	Rename state_base to SPxSharedPtrState,
*			and move out of base class.
*   26/11/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_SHARED_PTR_H
#define _SPX_SHARED_PTR_H

/* Standard headers. */
/* VS2010 can give warnings from redefinitions of macros defined
 * in both stdint.h and intsafe.h.
 */
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4005)
#include <intsafe.h>
#endif
#include <stdint.h>
#ifdef _WIN32
#pragma warning(pop)
#endif
#include <new>

/* For atomic variable. */
#include "SPxLibUtils/SPxAtomic.h"

/* For SPxAlign/SPxAlignof. */
#include "SPxLibUtils/SPxTypes.h"

namespace SPxSharedPtrNS
{
    template<typename T, typename State>
    static unsigned char *GetAlignedPtr(unsigned char *& objPtr, unsigned char*& statePtr)
    {
	unsigned char *mem = new unsigned char[SPxAlignof(T) - 1 + sizeof(T) + SPxAlignof(State) - 1 + sizeof(State)];
	objPtr = reinterpret_cast<unsigned char*>(reinterpret_cast<uintptr_t>(mem + SPxAlignof(T) - 1) & ~(SPxAlignof(T) - 1));
	statePtr = reinterpret_cast<unsigned char*>(reinterpret_cast<uintptr_t>(objPtr + sizeof(T) + SPxAlignof(State) - 1) & ~(SPxAlignof(State) - 1));
	return mem;
    }
}

/* Default deleters for pointers. */
template<typename T>
struct SPxDefaultDeleteShared
{
    void operator()(T* p) const
    {
	delete p;
    }
};

/* This structure is defined here so it is not
* dependent on the SPxSharedPtr template parameters.
*/
struct SPxSharedPtrState
{
    explicit SPxSharedPtrState(unsigned char *mem=0)
	: m_count(1), m_mem(mem) {}
    virtual ~SPxSharedPtrState() {}
    virtual void destroy()=0;

    mutable SPxAtomic<int> m_count;
    unsigned char *m_mem;
};

/*
 * SPxSharedPtr class.
 */
template<typename T, typename D=SPxDefaultDeleteShared<T> >
class SPxSharedPtr
{
public:
    /* Types. */
    typedef T** pp_type;
    typedef const T** cpp_type;
    typedef T* p_type;
    typedef const T* cp_type;
    typedef T el_type;
    typedef const T cel_type;
    typedef D d_type;
    typedef D cd_type;

    /* Construction and destruction. */
    SPxSharedPtr() : m_state(), m_ptr() {}
    template<typename U>
    explicit SPxSharedPtr(U* ptr) : m_state(new state<U, D>(ptr, d_type())), m_ptr(ptr) {}
    template<typename U, typename E>
    SPxSharedPtr(U* ptr, E del) : m_state(new state<U, E>(ptr, del)), m_ptr(ptr) {}
    SPxSharedPtr(const SPxSharedPtr& other)
	// cppcheck-suppress copyCtorPointerCopying
	: m_state(other.m_state), m_ptr(other.m_ptr) { addRef(); }
    template<typename U>
    SPxSharedPtr(const SPxSharedPtr<U>& other)
	: m_state(other.m_state), m_ptr(other.m_ptr) { addRef(); }
    ~SPxSharedPtr() { removeRef(); }

    SPxSharedPtr& operator=(const SPxSharedPtr& other)
    {
	if( this != &other )
	{
	    removeRef();
	    m_state = other.m_state;
	    m_ptr = other.m_ptr;
	    addRef();
	}
	return *this;
    }

    el_type& operator*() { return *get(); }
    cel_type& operator*() const { return *get(); }
    p_type operator->() { return get(); }
    cp_type operator->() const { return get(); }
    p_type get() { return m_ptr; }
    cp_type get() const { return m_ptr; }
    operator p_type() { return get(); }
    operator cp_type() const { return get(); }

    void reset() { removeRef(); }
    template<typename U>
    void reset(U *ptr)
    {
	bool isEmpty = (ptr == NULL);
	if( !isEmpty && (ptr == get()) )
	{
	    /* Self assignment not allowed. */
	    throw "Self assignment of SPxSharedPtr is undefined";
	}
	removeRef();
	m_state = new state<U, D>(ptr, d_type());
	m_ptr = ptr;
    }
    template<typename U, typename E>
    void reset(U *ptr, E del)
    {
	bool isEmpty = (ptr == NULL);
	if( !isEmpty && (ptr == get()) )
	{
	    /* Self assignment not allowed. */
	    throw "Self assignment of SPxSharedPtr is undefined";
	}
	removeRef();
	m_state = new state<U, E>(ptr, del);
	m_ptr = ptr;
    }
    long use_count() const
    {
	if( m_state )
	{
	    return m_state->m_count;
	}
	return 0;
    }
    bool unique() const
    {
	return (use_count() == 1);
    }

private:
    /* Private types. */
    template<typename U, typename E=D>
    struct state : public SPxSharedPtrState
    {
	state(U *p, E d)
	    : m_ptr(p), m_del(d) {}
	virtual void destroy()
	{
	    m_del(m_ptr);
	}

	U* m_ptr;
	E m_del;
    };
    template<typename U, typename E=D>
    struct stateWithMem : public SPxSharedPtrState
    {
	stateWithMem(U *p, unsigned char *mem)
	    : SPxSharedPtrState(mem), m_ptr(p), m_del() {}
	virtual void destroy()
	{
	    m_ptr->~U();
	}

	U* m_ptr;
	E m_del;
    };

    /* Private variables. */
    SPxSharedPtrState* m_state;
    p_type m_ptr;

    /* Private functions. */
    void addRef()
    {
	if( m_state )
	{
	    ++m_state->m_count;
	}
    }
    void removeRef()
    {
	if( m_state )
	{
	    if( (--m_state->m_count) == 0 )
	    {
		m_state->destroy();
		if( m_state->m_mem )
		{
		    unsigned char *mem = m_state->m_mem;
		    m_state->~SPxSharedPtrState();
		    delete[] mem;
		}
		else
		{
		    delete m_state;
		}
	    }
	}
	m_state = 0;
	m_ptr = 0;
    }

    /* Constructor called by SPxMakeShared. */
    template<typename U, typename E>
    SPxSharedPtr(U* ptr, stateWithMem<U, E> *st)
	: m_state(st), m_ptr(ptr) {}

    template<typename U, typename E>
    friend class SPxSharedPtr;

    template<typename U>
    friend SPxSharedPtr<U> SPxMakeShared();
    template<typename U, typename Arg1>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1);
    template<typename U, typename Arg1, typename Arg2>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2);
    template<typename U, typename Arg1, typename Arg2, typename Arg3>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3);
    template<typename U, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4);
    template<typename U, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5);
    template<typename U, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6);
    template<typename U, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7);
    template<typename U, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8);
    template<typename U, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9);
    template<typename U, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
    friend SPxSharedPtr<U> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10);

}; /* SPxSharedPtr */

/* Class to prevent accidental use with unsupported arrays. */
template<typename T, typename D>
class SPxSharedPtr<T[], D>
{
public:
    SPxSharedPtr()
    {
	SPX_STATIC_ASSERT_ALWAYS(T, SPxSharedPtr_cannot_be_used_with_arrays);
    }
};

/* Non-member functions. */
template<typename T>
inline SPxSharedPtr<T> SPxMakeShared()
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T();
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1, typename Arg2>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2, arg3);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2, arg3, arg4);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2, arg3, arg4, arg5);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2, arg3, arg4, arg5, arg6);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}


template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
inline SPxSharedPtr<T> SPxMakeShared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10)
{
    typedef typename SPxSharedPtr<T>::template stateWithMem<T> state_type;
    unsigned char *objMem = 0;
    unsigned char *stateMem = 0;
    unsigned char *mem = SPxSharedPtrNS::GetAlignedPtr<T, state_type>(objMem, stateMem);
    T *obj = new (objMem) T(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    state_type *state = new (stateMem) state_type(obj, mem);
    return SPxSharedPtr<T>(obj, state);
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator==(const SPxSharedPtr<T1, D1>& a, const SPxSharedPtr<T2, D2>& b)
{
    return a.get() == b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator!=(const SPxSharedPtr<T1, D1>& a, const SPxSharedPtr<T2, D2>& b)
{
    return a.get() != b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator<(const SPxSharedPtr<T1, D1>& a, const SPxSharedPtr<T2, D2>& b)
{
    return a.get() < b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator<=(const SPxSharedPtr<T1, D1>& a, const SPxSharedPtr<T2, D2>& b)
{
    return a.get() <= b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator>(const SPxSharedPtr<T1, D1>& a, const SPxSharedPtr<T2, D2>& b)
{
    return a.get() > b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator>=(const SPxSharedPtr<T1, D1>& a, const SPxSharedPtr<T2, D2>& b)
{
    return a.get() >= b.get();
}

#endif /* _SPX_SHARED_PTR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
