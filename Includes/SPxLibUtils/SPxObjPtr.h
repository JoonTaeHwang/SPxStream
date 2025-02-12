/*********************************************************************
*
* (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxObjPtr class that automatically nullifies a pointer
*   when an SPxObj is deleted.
*
*   This is useful when an SPxObj that could be deleted is being
*   stored in another object. By storing it within an
*   SPxObjPtr, the object can be used safely regardless of the order
*   of deletion of SPx objects.
*
*   SPxObjPtr<SPxObj> m_obj; // Reference to an SPxObj* not managed by us.
*
*   When using the object, lock it to prevent deletion during use.
*
*	SPxAutoLock lock(m_obj);    // Lock the object (safe even after deletion).
*
*	if( m_obj )		    // Check if the object is still valid.
*	{
*	    m_obj->SetName();	    // Use the object.
*	}
*
* Revision Control:
*   08/07/21 v1.8    AGC	Fix N/C #875 - preserved owned flag when omitted from reset().
*
* Previous Changes:
*   30/05/18 1.7    AGC	Fix N/C #562 - operator> incorrect.
*   09/05/17 1.6    AGC	Fix cppcheck warnings.
*   22/01/16 1.5    AGC	Check for AddDeletionCallback() success.
*   01/05/14 1.4    AGC	Fix cppcheck warning.
*   25/04/14 1.3    AGC	Allow ownership using SPxObjPtr.
*   26/07/13 1.2    AGC	Prevent unused argument warning on objDeleteFn.
*   21/12/12 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_OBJ_PTR_H
#define _SPX_OBJ_PTR_H

/* Standarad headers. */
#include <cstddef>
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxAutoLock.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxLock.h"

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
 * SPxObjPtr class for SPxObj pointers.
 */
template<typename T>
class SPxObjPtr : public SPxLock
{
public:
    typedef T** pp_type;
    typedef const T** cpp_type;
    typedef T* p_type;
    typedef const T* cp_type;
    typedef T el_type;
    typedef const T cel_type;

    /* Construction and destruction. */
    SPxObjPtr() : m_mutex(TRUE), m_obj(p_type()), m_owned(FALSE) {}
    explicit SPxObjPtr(p_type p, int owned=FALSE) : m_mutex(TRUE), m_obj(p), m_owned(owned)
    {
	if( m_obj )
	{
	    /* Add callback to new object we are tracking. */
	    m_obj->AddDeletionCallback((void*)objDeleteFn, this);
	}
    }
    ~SPxObjPtr()
    {
	SPxAutoLock lock(m_mutex);
	reset();
    }

    el_type& operator*() { return *get(); }
    cel_type& operator*() const { return *get(); }
    p_type operator->() { return get(); }
    cp_type operator->() const { return get(); }
    pp_type operator&() { return &m_obj; }
    cpp_type operator&() const { return &m_obj; }
    p_type get() { return m_obj;  }
    cp_type get() const { return m_obj;  }
    operator p_type() { return get(); }
    operator cp_type() const { return get(); }

    void reset(p_type p=p_type())
    {
	reset(p, m_owned);
    }
    void reset(p_type p, int owned)
    {
	p_type tmp(m_obj);
	m_obj = p;
	p = tmp;
	if( p )
	{
	    /* Remove callback from existing object we are tracking. */
	    p->RemoveDeletionCallback((void*)objDeleteFn, this);

	    if( m_owned )
	    {
		/* Delete object when owned. */
		delete p;
	    }
	}
	m_owned = owned;
	if( m_obj )
	{
	    /* Add callback to new object we are tracking. */
	    if( m_obj->AddDeletionCallback((void*)objDeleteFn, this, TRUE) != SPX_NO_ERROR )
	    {
		/* Object is being deleted. */
		m_obj = NULL;
	    }
	}
    }

    /* Functions from SPxLock. */
    virtual SPxErrorCode Initialise(void)
    {
	return SPX_NO_ERROR;
    }
    virtual SPxErrorCode Enter(void)
    {
	return m_mutex.Enter();
    }
    virtual SPxErrorCode TryEnter(void)
    {
	return m_mutex.TryEnter();
    }
    virtual SPxErrorCode Leave(void)
    {
	return m_mutex.Leave();
    }

private:
    /* Private variables. */
    SPxCriticalSection m_mutex;
    p_type m_obj;   /* Pointer. */
    int m_owned;    /* Is the pointer owned by this object? */

    /* Private functions. */
    static int objDeleteFn(void *invokingObject, void *userObject, void * /*arg*/)
    {
	p_type obj = static_cast<p_type>(invokingObject);
	SPxObjPtr *ptr = static_cast<SPxObjPtr*>(userObject);
	
	/* Lock the mutex while nullifying the pointer. */
	SPxAutoLock lock(ptr->m_mutex);

	/* If the pointer matches, the the object we are tracking
	 * is being deleted, so nullify our pointer.
	 */
	if( ptr->m_obj == obj )
	{
	    ptr->m_obj = NULL;
	}
	return SPX_NO_ERROR;
    }

    /* Disable copy and assignment operators. */
    SPxObjPtr(const SPxObjPtr&) SPX_FUNCTION_DELETE;
    SPxObjPtr& operator=(const SPxObjPtr&) SPX_FUNCTION_DELETE;

}; /* SPxObjPtr */

/* Non-member functions. */

template<typename T1, typename T2>
inline bool operator==(const SPxObjPtr<T1>& a, const SPxObjPtr<T2>& b)
{
    return a.get() == b.get();
}

template<typename T1, typename T2>
inline bool operator!=(const SPxObjPtr<T1>& a, const SPxObjPtr<T2>& b)
{
    return a.get() != b.get();
}

template<typename T1, typename T2>
inline bool operator<(const SPxObjPtr<T1>& a, const SPxObjPtr<T2>& b)
{
    return a.get() < b.get();
}

template<typename T1, typename T2>
inline bool operator<=(const SPxObjPtr<T1>& a, const SPxObjPtr<T2>& b)
{
    return a.get() <= b.get();
}

template<typename T1, typename T2>
inline bool operator>(const SPxObjPtr<T1>& a, const SPxObjPtr<T2>& b)
{
    return a.get() > b.get();
}

template<typename T1, typename T2>
inline bool operator>=(const SPxObjPtr<T1>& a, const SPxObjPtr<T2>& b)
{
    return a.get() >= b.get();
}

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_OBJ_PTR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
