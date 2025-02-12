/*********************************************************************
*
* (c) Copyright 2014 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxOptional.h,v $
* ID: $Id: SPxOptional.h,v 1.11 2017/01/24 12:15:56 rew Exp $
*
* Purpose:
*   Header for SPxOptional - container for an object that may not
*   be there. If it is not there, an error code is available instead.
*
*   Example usage:
*
*	class Obj
*	{
*	public:
*	    Obj(int a, int b) {}
*	};
*
*	SPxOptional<Obj> funcObj()
*	{
*	    // Directly construct optional like it is the value (avoids copy).
*	    return SPxOptional<Obj>(4, 5);
*	}
*
*	SPxOptional<int> func()
*	{
*	    if( bad() )
*	    {
*		return SPX_ERR_BAD_ARGUMENT;	// Error code converts to optional.
*	    }
*
*	    return 4;	// Value converts to optional.
*	}
*
*	SPxOptional<int> val = func();
*
*	if( val ) // Tests if val is available.
*	{
*	    useVal(val);    // useVal takes "int" variable.
*	}
*
*	SPX_RETURN_IF_FAIL(val);    // Val is used as error code.
*
*	int valToUse = val.value_or(5);	// valToUse is val if available, otherwise 5.
*
*
* Revision Control:
*   24/01/17 v1.11   AGC	Fix copy constructor with non-aligned types.
*
* Previous Changes:
*   07/12/16 1.10   AGC	Fix cppcheck warnings.
*   05/09/16 1.9    AGC	Fix last change.
*   02/09/16 1.8    AGC	Disable warning on VS2010 behaviour change.
*   08/07/16 1.7    AGC	Prevent misuse.
*   19/05/16 1.6    AGC	Removed unused stdint header.
*   18/05/16 1.5    AGC	Use correct alignment.
*   16/02/15 1.4    AGC	Improve conversion operator behaviour.
*   09/02/15 1.3    AGC	Disable cppcheck pointer cast warnings.
*   10/11/14 1.2    AGC	Fix shadow warnings.
*   07/11/14 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_OPTIONAL_H
#define _SPX_OPTIONAL_H

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTypes.h"

template<typename T>
class SPxOptional
{
    /*
     * The safe bool idiom is used here where a pointer to a private
     * member function is returned as the 'boolean' type where
     * required. This avoids accidental implicit conversion from
     * a bool type a type that could be used as an SPxOptional
     * constructor argument.
     */
    typedef void (SPxOptional::*bool_type)(void) const;
public:
    SPxOptional() : m_err(SPX_NO_ERROR)
    {
	alloc();
#ifdef _WIN32
/* Disable warning about behaviour change in VS2010. */
#pragma warning(push)
#pragma warning(disable: 4345)
#endif
	new (m_ptr) T();
#ifdef _WIN32
#pragma warning(pop)
#endif
    }
    SPxOptional(SPxErrorCode err) : m_err(err)
    {
	alloc();
	if( m_err == SPX_NO_ERROR )
	{
#ifdef _WIN32
/* Disable warning about behaviour change in VS2010. */
#pragma warning(push)
#pragma warning(disable: 4345)
#endif
	    new (m_ptr) T();
#ifdef _WIN32
#pragma warning(pop)
#endif
	}
	else
	{
	    m_ptr[0] = '\0';
	}
    }
    SPxOptional(const T &val) : m_err(SPX_NO_ERROR)
    {
	alloc();
	new (m_ptr) T(val);
    }
    SPxOptional(const SPxOptional<T> &other) : m_err(other.m_err)
    {
	alloc();
	if( m_err == SPX_NO_ERROR )
	{
	    new (m_ptr) T(*other);
	}
	else
	{
	    m_ptr[0] = '\0';
	}
    }
    template<typename Arg1>
    SPxOptional(Arg1 arg1) : m_err(SPX_NO_ERROR)
    {
	alloc();
	new (m_ptr) T(arg1);
    }
    template<typename Arg1, typename Arg2>
    SPxOptional(Arg1 arg1, Arg2 arg2) : m_err(SPX_NO_ERROR)
    {
	alloc();
	new (m_ptr) T(arg1, arg2);
    }
    template<typename Arg1, typename Arg2, typename Arg3>
    SPxOptional(Arg1 arg1, Arg2 arg2, Arg3 arg3) : m_err(SPX_NO_ERROR)
    {
	alloc();
	new (m_ptr) T(arg1, arg2, arg3);
    }
    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    SPxOptional(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) : m_err(SPX_NO_ERROR)
    {
	alloc();
	new (m_ptr) T(arg1, arg2, arg3, arg4);
    }
    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    SPxOptional(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) : m_err(SPX_NO_ERROR)
    {
	alloc();
	new (m_ptr) T(arg1, arg2, arg3, arg4, arg5);
    }
    ~SPxOptional()
    {
	destroy();
    }

    SPxOptional<T>& operator=(SPxErrorCode err)
    {
	m_err = err;
	return *this;
    }
    template<typename U>
    SPxOptional<T>& operator=(const U &val)
    {
	destroy();
	m_err = SPX_NO_ERROR;
	new (m_ptr) T(val);
	return *this;
    }
    SPxOptional<T>& operator=(const SPxOptional<T> &other)
    {
	destroy();
	alloc();
	m_err = other.m_err;
	if( m_err == SPX_NO_ERROR )
	{
	    new (m_ptr) T(*other);
	}
	return *this;
    }

    const T* operator->() const
    {
	return get();
    }
    T* operator->()
    {
	return get();
    }
    const T& operator*() const
    {
	return *get();
    }
    T& operator*()
    {
	return *get();
    }
    const T& value() const
    {
	return *get();
    }
    T& value()
    {
	return *get();
    }
    SPxErrorCode error() const
    {
	return m_err;
    }

    operator bool_type() const { return (m_err == SPX_NO_ERROR) ? &SPxOptional::m_bt : 0; }
   
    template<class U>
    T value_or(const U& val) const
    {
	return (m_err == SPX_NO_ERROR) ? *get() : val;
    }
    template<class U>
    T value_or(U& val) const
    {
	return (m_err == SPX_NO_ERROR) ? *get() : val;
    }

private:
    SPxErrorCode m_err;
    /* Allocate enough memory so that object can be created with correct alignment. */
    unsigned char m_val[sizeof(T) + SPxAlignof(T) - 1];
    unsigned char *m_ptr;
    void m_bt() const {} /* Member used for safe bool idiom. */

    void alloc()
    {
	/* Zero first element of value to suppress cppcheck warning. */
	m_val[0] = 0;

	/* Enforce correct alignment for memory. */
	m_ptr = reinterpret_cast<unsigned char*>(((reinterpret_cast<uintptr_t>(m_val) + SPxAlignof(T) - 1) & ~(SPxAlignof(T) - 1)));
    }
    void destroy()
    {
	if( m_err == SPX_NO_ERROR )
	{
	    get()->~T();
	}
    }
    T* get()
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<T*>(m_ptr);
    }
    const T* get() const
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<const T*>(m_ptr);
    }
};

/* Non-member functions. */

template<typename T1, typename T2>
inline bool operator==(const SPxOptional<T1>& a, const SPxOptional<T2>& b)
{
    return *a == *b;
}

template<typename T1, typename T2>
inline bool operator!=(const SPxOptional<T1>& a, const SPxOptional<T2>& b)
{
    return *a != *b;
}

template<typename T1, typename T2>
inline bool operator<(const SPxOptional<T1>& a, const SPxOptional<T2>& b)
{
    return *a < *b;
}

template<typename T1, typename T2>
inline bool operator<=(const SPxOptional<T1>& a, const SPxOptional<T2>& b)
{
    return *a <= *b;
}

template<typename T1, typename T2>
inline bool operator>(const SPxOptional<T1>& a, const SPxOptional<T2>& b)
{
    return *a > *b;
}

template<typename T1, typename T2>
inline bool operator>=(const SPxOptional<T1>& a, const SPxOptional<T2>& b)
{
    return *a >= *b;
}

template<typename T1, typename T2>
inline bool operator==(const SPxOptional<T1>& a, const T2& b)
{
    return *a == b;
}

template<typename T1, typename T2>
inline bool operator!=(const SPxOptional<T1>& a, const T2& b)
{
    return *a != b;
}

template<typename T1, typename T2>
inline bool operator<(const SPxOptional<T1>& a, const T2& b)
{
    return *a < b;
}

template<typename T1, typename T2>
inline bool operator<=(const SPxOptional<T1>& a, const T2& b)
{
    return *a <= b;
}

template<typename T1, typename T2>
inline bool operator>(const SPxOptional<T1>& a, const T2& b)
{
    return *a > b;
}

template<typename T1, typename T2>
inline bool operator>=(const SPxOptional<T1>& a, const T2& b)
{
    return *a >= b;
}

template<typename T1, typename T2>
inline bool operator==(const T1& a, const SPxOptional<T2>& b)
{
    return a == *b;
}

template<typename T1, typename T2>
inline bool operator!=(const T1& a, const SPxOptional<T2>& b)
{
    return a != *b;
}

template<typename T1, typename T2>
inline bool operator<(const T1& a, const SPxOptional<T2>& b)
{
    return a < *b;
}

template<typename T1, typename T2>
inline bool operator<=(const T1& a, const SPxOptional<T2>& b)
{
    return a <= *b;
}

template<typename T1, typename T2>
inline bool operator>(const T1& a, const SPxOptional<T2>& b)
{
    return a > *b;
}

template<typename T1, typename T2>
inline bool operator>=(const T1& a, const SPxOptional<T2>& b)
{
    return a >= *b;
}

#endif /* _SPX_OPTIONAL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
