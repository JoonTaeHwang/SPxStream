/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRange.h,v $
* ID: $Id: SPxRange.h,v 1.2 2015/04/14 14:57:44 rew Exp $
*
* Purpose:
*   SPxRange can be combined with SPX_FOREACH for simple
*   iteration over a range of values.
*
* Revision Control:
*   14/04/15 v1.2    AGC	Fix icc warnings.
*
* Previous Changes:
*   26/03/15 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_RANGE_H
#define _SPX_RANGE_H

#include <cstddef>
#include <iterator>

/* Range iterator. */
template <class T>
class SPxRange_iterator
{
public:
    typedef SPxRange_iterator<T> self_type;
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef T& reference;

    SPxRange_iterator(T val, T step, T stop)
	: m_val(val), m_step(step), m_stop(stop) {}
    ~SPxRange_iterator() {}

    value_type &operator*() { return m_val; }
    value_type *operator->() { return &(operator*()); }
    self_type &operator++() { m_val += m_step; return *this; }
    self_type operator++(int) { self_type tmp(*this); ++(*this); return tmp; }
    bool operator==(const self_type &other) const
    {
	return ((m_step > T()) && (m_val >= m_stop) && (other.m_val >= other.m_stop))
	    || ((m_step < T()) && (m_val <= m_stop) && (other.m_val <= other.m_stop))
	    || (m_val == other.m_val);
    }
    bool operator!=(const self_type &other) const { return !operator==(other); }

private:
    typename SPxRemoveConst<T>::type m_val;
    T m_step;
    T m_stop;
};

/* Range object. */
template<typename T>
class SPxRangeImpl
{
public:
    typedef SPxRangeImpl<T> self_type;
    typedef T value_type;
    typedef T& reference;
    typedef SPxRange_iterator<value_type> iterator;
    typedef SPxRange_iterator<const value_type> const_iterator;

    SPxRangeImpl(T start, T stop, T step)
	: m_start(start), m_stop(stop), m_step(step) {}
    ~SPxRangeImpl() {}

    iterator begin() { return iterator(m_start, m_step, m_stop); }
    iterator end() { return iterator(m_stop, m_step, m_stop); }
    const_iterator begin() const { return const_iterator(m_start, m_step, m_stop); }
    const_iterator end() const { return const_iterator(m_stop, m_step, m_stop); }

private:
    const T m_start;
    const T m_stop;
    const T m_step;
};

/* Create a range object. */
template<typename T>
inline const SPxRangeImpl<T> SPxRange(T stop)
{
    return SPxRangeImpl<T>(0, stop, 1);
}

template<typename T>
inline const SPxRangeImpl<T> SPxRange(T start, T stop)
{
    return SPxRangeImpl<T>(start, stop, 1);
}

template<typename T>
inline const SPxRangeImpl<T> SPxRange(T start, T stop, T step)
{
    return SPxRangeImpl<T>(start, stop, step);
}

#endif /* _SPX_RANGE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
