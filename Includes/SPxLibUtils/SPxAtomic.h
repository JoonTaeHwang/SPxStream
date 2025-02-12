/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxAtomic class that supports atomic operations
*   on int, unsigned int, long, and unsigned long types.
*
*   Atomic operations are supported using compiler intrinsics
*   where available, and assembly code otherwise.
*
*
* Revision Control:
*   26/08/22 v1.6    AGC	Remove operator&.
*
* Previous Changes:
*   09/05/17 1.5    AGC	Fix cppcheck warnings.
*   09/03/17 1.4    AGC	Improve headers for uintptr_t.
*   08/03/17 1.3    AGC	Add support for bitwise atomic operations.
*			Add support for atomic pointers.
*   23/08/16 1.2    AGC	SPxAtomic now modeled on std::atomic interfaces.
*   26/11/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_ATOMIC_H
#define _SPX_ATOMIC_H

/* Required for uintptr_t. */
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

/* For SPX_FUNCTION_DELETE. */
#include "SPxLibUtils/SPxTypes.h"

/*
 * SPxAtomic class.
 */
template<typename T>
class SPxAtomic
{
public:
    /* Construction and destruction. */
    SPxAtomic(void);
    SPxAtomic(T val);

    T operator=(T val);

    T operator+=(T val);
    T operator-=(T val);
    T operator&=(T val);
    T operator|=(T val);
    T operator^=(T val);

    T operator++();
    T operator++(int);
    T operator--();
    T operator--(int);

    operator T() const;

private:
    /* Private variables. */
    volatile T m_val;

    SPxAtomic(const SPxAtomic<T>& other) SPX_FUNCTION_DELETE;
    SPxAtomic<T>& operator=(const SPxAtomic<T>& other) SPX_FUNCTION_DELETE;

}; /* SPxAtomic */

template<>
class SPxAtomic<void*>
{
public:
    /* Construction and destruction. */
    SPxAtomic(void) {}
    SPxAtomic(void* val) : m_val(reinterpret_cast<uintptr_t>(val)) {}

    void* operator=(void* val) { return reinterpret_cast<void*>(static_cast<uintptr_t>(m_val = reinterpret_cast<uintptr_t>(val))); }
    
    operator void*() { return get(); }
    operator const void*() const { return get(); }

    const void* operator->() const { return get(); }
    void* operator->() { return get(); }

    void* get() { return reinterpret_cast<void*>(static_cast<uintptr_t>(m_val)); }
    const void* get() const { return reinterpret_cast<const void*>(static_cast<uintptr_t>(m_val)); }

private:
    /* Private variables. */
    SPxAtomic<uintptr_t> m_val;

    SPxAtomic(const SPxAtomic<void*>& other) SPX_FUNCTION_DELETE;
    SPxAtomic<void*>& operator=(const SPxAtomic<void*>& other) SPX_FUNCTION_DELETE;
};

template<typename T>
class SPxAtomic<T*>
{
public:
    /* Construction and destruction. */
    SPxAtomic(void) {}
    SPxAtomic(T* val) : m_val(reinterpret_cast<uintptr_t>(val)) {}

    T* operator=(T* val) { return reinterpret_cast<T*>(static_cast<uintptr_t>(m_val = reinterpret_cast<uintptr_t>(val))); }
    
    operator T*() { return get(); }
    operator const T*() const { return get(); }

    const T& operator*() const { return *get(); }
    T& operator*() { return *get(); }
    const T* operator->() const { return get(); }
    T* operator->() { return get(); }

    T* get() { return reinterpret_cast<T*>(static_cast<uintptr_t>(m_val)); }
    const T* get() const { return reinterpret_cast<const T*>(static_cast<uintptr_t>(m_val)); }

private:
    /* Private variables. */
    SPxAtomic<uintptr_t> m_val;

    SPxAtomic(const SPxAtomic<T*>& other) SPX_FUNCTION_DELETE;
    SPxAtomic<T*>& operator=(const SPxAtomic<T*>& other) SPX_FUNCTION_DELETE;
};

#endif /* _SPX_ATOMIC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
