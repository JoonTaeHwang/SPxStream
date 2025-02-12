/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRefWrapper.h,v $
* ID: $Id: SPxRefWrapper.h,v 1.1 2014/11/07 14:42:47 rew Exp $
*
* Purpose:
*   Header for SPxRefWrapper object that wraps a reference so it
*   can be used as though it has value semantics.
*
* Revision Control:
*   07/11/14 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_REF_WRAPPER_H
#define _SPX_REF_WRAPPER_H

template<typename T>
class SPxRefWrapper
{
public:
    SPxRefWrapper() : m_val(0) {}
    SPxRefWrapper(T& val) : m_val(reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char &>(val)))) {}
    SPxRefWrapper(const SPxRefWrapper<T>& other) : m_val(other.m_val) {}

    SPxRefWrapper& operator=(const SPxRefWrapper<T>& other)
    {
	m_val = other.m_val;
	return *this;
    }

    operator T&() const { return *m_val; }
    T& get() const { return *m_val; }

private:
    T* m_val;
};

/* Functions to turn a reference into a reference wrapped object. */
template<typename T>
inline SPxRefWrapper<T> SPxRef(T &val) { return SPxRefWrapper<T>(val); }

template<typename T>
inline SPxRefWrapper<const T> SPxCref(const T &val) { return SPxRefWrapper<const T>(val); }

#endif /* _SPX_REF_WRAPPER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
