/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPx Auto Ptr class that automatically deletes
*   an object on destruction (optionally using a custom deleter).
*
*   SPxAutoPtr objects cannot be copied, but ownership of the pointer can be transferred:
*
*	SPxAutoPtr<SPxObj> autoA(new SPxObj());
*	SPxAutoPtr<SPxObj> autoB;
*
*	// Transfer ownership of SPxObj pointer from autoA to autoB.
*	autoB.reset(autoA.release());
*
*   Example:
*
*	SPxAutoPtr<SPxObj> obj(new SPxObj());
*
*	obj->SetName(); // Accesses underlying pointer.
*
*	// SPxObj deleted using standard delete operator on 
*	// destruction of 'obj'.
*
*
*   Example with array:
*
*	SPxAutoPtr<char[]> value(new char[64]);
*
*	value[0] = 'b'; // Access underlying array.
*
*	// Memory correctly freed (and objects destructed if of class/struct type) on
*	// destruction of 'value'.
*
*
*   Example with custom deleter:
*
*	SPxAutoPtr<mxml_node_t, void(*)(mxml_node_t*)> node(mxmlLoadString(...), mxmlDelete);
*
*	// Use 'node' as though it were an mxml_node_t* including
*	// passing 'node' to functions.
*
*	// mxml node will be deleted using mxmlDelete on destruction of 'node'.
*
*
* Revision Control:
*   06/09/22 v1.19   AGC	Suppress spurious cppcheck ODR warnings.
*
* Previous Changes:
*   26/08/22 1.18   AGC	Workaround cppcheck parsing issues.
*   02/11/20 1.17   AGC	Fix warning in last change.
*   30/10/20 1.16   AGC	Workaround cppcheck parsing issues.
*   11/10/19 1.15   AGC	Avoid cppcheck errors.
*   29/10/18 1.14   AGC	Ignore spurious cppcheck V1.85 errors.
*   09/05/17 1.13   AGC	Fix cppcheck warnings.
*   19/10/16 1.12   AGC	Add operator& overload for T[].
*   29/04/16 1.11   AGC	Fix cppcheck suppression.
*   20/04/16 1.10   AGC	Supress erroneous warnings with cppcheck 1.73.
*			Support changing deleter with reset().
*   07/11/14 1.9    AGC	Add empty() function to array form of SPxAutoPtr.
*   24/02/14 1.8    AGC	Explicitly disallow self assignment.
*   26/11/13 1.7    AGC	Fix operator> implementation.
*			Allow deletion types to not match exactly.
*   08/11/13 1.6    AGC	Fix get_deleter() return type for T[].
*			Fix safe bool idiom for T[] form.
*   01/10/12 1.5    AGC	Remove need for algorithm header.
*   21/08/12 1.4    AGC	Add address-of overload.
*   13/07/12 1.3    AGC	Fix build error on QNX.
*   19/04/12 1.2    AGC	Remove definition of copy constructor/assignment.
*			Handle const/non-const correctly.
*   19/04/12 1.1    AGC	Initial Version base on SPxAutoMutex.
**********************************************************************/
#ifndef _SPX_AUTO_PTR_H
#define _SPX_AUTO_PTR_H

/* Standarad headers. */
#include <cstddef>

/* For SPX_FUNCTION_DELETE. */
#include "SPxLibUtils/SPxTypes.h"

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

/* Default deleters for pointers. */
template<typename T>
// cppcheck-suppress ctuOneDefinitionRuleViolation
struct SPxDefaultDelete
{
    void operator()(T* p) const
    {
	delete p;
    }
};

/* Default deleters for pointers. */
template<typename T>
// cppcheck-suppress ctuOneDefinitionRuleViolation
struct SPxDefaultDelete<T[]>
{
    void operator()(T* p) const
    {
	delete[] p;
    }
};

/*
 * SPxAutoPtr class for pointers.
 */
template<typename T, typename D=SPxDefaultDelete<T> >
class SPxAutoPtr
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
    // cppcheck-suppress uninitMemberVar
    SPxAutoPtr() : m_ptr(), m_del() {}
    // cppcheck-suppress uninitMemberVar
    explicit SPxAutoPtr(p_type p) : m_ptr(p), m_del() {}
    // cppcheck-suppress uninitMemberVar
    template<typename E> SPxAutoPtr(p_type p, E d) : m_ptr(p), m_del(d) {}
    ~SPxAutoPtr() { reset(); }

    el_type& operator*() { return *get(); }
    cel_type& operator*() const { return *get(); }
    p_type operator->() { return get(); }
    cp_type operator->() const { return get(); }
    pp_type operator&() { return &m_ptr; }
    cpp_type operator&() const { return &m_ptr; }
    p_type get() { return m_ptr;  }
    cp_type get() const { return m_ptr;  }
    d_type& get_deleter() { return m_del; }
    cd_type& get_deleter() const { return m_del; }
    operator p_type() { return get(); }
    operator cp_type() const { return get(); }
    p_type release()
    {
	p_type p = get();
	m_ptr = p_type();
	return p;
    }

    void reset(p_type p=p_type())
    {
	p_type tmp(m_ptr);
	m_ptr = p;
	p = tmp;
	bool isEmpty = (p == NULL);
	if( !isEmpty)
	{
	    get_deleter()(p);
	}
    }
    void reset(p_type p, d_type d)
    {
	reset(p);
	m_del = d;
    }

private:
    /* Private variables. */
    p_type m_ptr;   /* Pointer. */
    d_type m_del;   /* Deleter. */

    /* Disable copy and assignment operators. */
    SPxAutoPtr(const SPxAutoPtr&) SPX_FUNCTION_DELETE;
    SPxAutoPtr& operator=(const SPxAutoPtr&) SPX_FUNCTION_DELETE;

}; /* SPxAutoPtr */

/*
 * SPxAutoPtr class for arrays
 */
template<typename T, typename D>
class SPxAutoPtr<T[], D>
{
    /* Access to the element (not a pointer to the element) is not
     * supported for array types directly, so an alternative
     * method is required to allow this type to be used in expressions
     * where a boolean is required (e.g. if( SPxAutoPtr<T>(a) ) ).
     *
     * The safe bool idiom is used here where a pointer to a private
     * member function is returned as the 'boolean' type where
     * required.
     */
    typedef void (SPxAutoPtr::*bool_type)(void) const;
public:
    /* Types. */
    typedef T** pp_type;
    typedef const T** cpp_type;
    typedef T* p_type;
    typedef p_type pointer;
    typedef const T* cp_type;
    typedef cp_type const_pointer;
    typedef T el_type;
    typedef const T cel_type;
    typedef D d_type;
    typedef D cd_type;

    /* Construction and destruction. */
    // cppcheck-suppress uninitMemberVar
    SPxAutoPtr() : m_ptr() {}
    // cppcheck-suppress uninitMemberVar
    explicit SPxAutoPtr(p_type p) : m_ptr(p), m_del(d_type()) {}
    // cppcheck-suppress uninitMemberVar
    SPxAutoPtr(p_type p, d_type d) : m_ptr(p), m_del(d) {}
    ~SPxAutoPtr() { reset(); }

    el_type& operator[](std::size_t i)
    {
	return get()[i];
    }
    cel_type& operator[](std::size_t i) const
    {
	return get()[i];
    }
    p_type get() { return m_ptr; }
    cp_type get() const { return m_ptr; }
    pp_type operator&() { return &m_ptr; }
    cpp_type operator&() const { return &m_ptr; }
    d_type& get_deleter() { return m_del; }
    cd_type& get_deleter() const { return m_del; }
    operator bool_type() const
    {
	cp_type curr = get();
	bool isSet = curr == NULL;
	return isSet ? 0 : &SPxAutoPtr::m_bt; }
    bool empty() const { return false; }

    p_type release()
    {
	p_type p = get();
	m_ptr = p_type();
	return p;
    }

    void reset(p_type p=p_type())
    {
	bool isEmpty = (p == NULL);
	if( !isEmpty && (p == get()) )
	{
	    /* Self assignment not allowed. */
	    throw "Self assignment of SPxAutoPtr is undefined";
	}
	p_type tmp(m_ptr);
	m_ptr = p;
	p = tmp;
	isEmpty = (p == p_type());
	if( !isEmpty)
	{
	    get_deleter()(p);
	}
    }

private:
    /* Private variables. */
    p_type m_ptr;   /* Pointer. */
    d_type m_del;   /* Deleter. */
    void m_bt() const {} /* Member used for safe bool idiom. */

    /* Disable copy and assignment operators. */
    SPxAutoPtr(const SPxAutoPtr&) SPX_FUNCTION_DELETE;
    SPxAutoPtr& operator=(const SPxAutoPtr&) SPX_FUNCTION_DELETE;

}; /* SPxAutoPtr */

/* Non-member functions. */

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator==(const SPxAutoPtr<T1, D1>& a, const SPxAutoPtr<T2, D2>& b)
{
    return a.get() == b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator!=(const SPxAutoPtr<T1, D1>& a, const SPxAutoPtr<T2, D2>& b)
{
    return a.get() != b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator<(const SPxAutoPtr<T1, D1>& a, const SPxAutoPtr<T2, D2>& b)
{
    return a.get() < b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator<=(const SPxAutoPtr<T1, D1>& a, const SPxAutoPtr<T2, D2>& b)
{
    return a.get() <= b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator>(const SPxAutoPtr<T1, D1>& a, const SPxAutoPtr<T2, D2>& b)
{
    return a.get() > b.get();
}

template<typename T1, typename D1, typename T2, typename D2>
inline bool operator>=(const SPxAutoPtr<T1, D1>& a, const SPxAutoPtr<T2, D2>& b)
{
    return a.get() >= b.get();
}

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_AUTO_PTR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
