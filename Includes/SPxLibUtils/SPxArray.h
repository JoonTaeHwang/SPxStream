/*********************************************************************
*
* (c) Copyright 2015, 2016, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxArray class - a templated header only
*   class encapsulating a fixed size array.
*
*   The behaviour is almost identical to a standard array, but with
*   the addition of an STL compliant iterator interface, and access
*   to the size of the array at run-time.
*
*   A specialisation with zero size exists and behaves appropriately,
*   but does not contain any data.
*
*
* Revision Control:
*   11/02/21 v1.4    AGC	Avoid cppcheck warning.
*
* Previous Changes:
*   18/05/16 1.3    AGC	Add SPxMakeArray().
*   05/02/15 1.2    AGC	Make size() and empty() functions static.
*   26/01/15 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_ARRAY_H
#define _SPX_ARRAY_H

/* Standard headers. */
#include <cstddef>
#include <iterator>

#include "SPxLibUtils/SPxTypes.h"

template <class T, std::size_t N>
class SPxArray
{
public:
    typedef SPxArray<T, N> self_type;
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin() { return data(); }
    iterator end() { return data() + N; }
    const_iterator begin() const { return data(); }
    const_iterator end() const { return &m_data[0] + N; }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    reference operator[](size_type pos) { return m_data[pos]; }
    const_reference operator[](size_type pos) const { return m_data[pos]; }
    reference front() { return m_data[0]; }
    reference back() { return m_data[N-1]; }
    const_reference front() const { return m_data[0]; }
    const_reference back() const { return m_data[N-1]; }
    pointer data() { return &m_data[0]; }
    const_pointer data() const { return &m_data[0]; }
    static size_type size() { return N; }
    static bool empty() { return false; }
    void fill(const_reference value)
    {
	for(size_type i = 0; i < N; ++i)
	{
	    m_data[i] = value;
	}
    }

    T m_data[N];
};

template <class T>
class SPxArray<T, 0>
{
public:
    typedef SPxArray<T, 0> self_type;
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin() { return data(); }
    iterator end() { return data(); }
    const_iterator begin() const { return data(); }
    const_iterator end() const { return data(); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    pointer data() { return NULL; }
    const_pointer data() const { return NULL; }
    static size_type size() { return 0; }
    static bool empty() { return true; }
};

/* Non-member functions. */
template<typename T>
inline SPxArray<T, 0> SPxMakeArray()
{
    SPxArray<T, 0> arr;
    return arr;
}

template<typename T>
inline SPxArray<T, 1> SPxMakeArray(typename SPxAddCrefIfNotRef<T>::type v1)
{
    SPxArray<T, 1> arr = { { v1 } };
    return arr;
}

template<typename T>
inline SPxArray<T, 2> SPxMakeArray(typename SPxAddCrefIfNotRef<T>::type v1,
				   typename SPxAddCrefIfNotRef<T>::type v2)
{
    SPxArray<T, 2> arr = { { v1, v2 } };
    return arr;
}

template<typename T>
inline SPxArray<T, 3> SPxMakeArray(typename SPxAddCrefIfNotRef<T>::type v1,
				   typename SPxAddCrefIfNotRef<T>::type v2,
				   typename SPxAddCrefIfNotRef<T>::type v3)
{
    SPxArray<T, 3> arr = { { v1, v2, v3 } };
    return arr;
}

template<typename T>
inline SPxArray<T, 4> SPxMakeArray(typename SPxAddCrefIfNotRef<T>::type v1,
				   typename SPxAddCrefIfNotRef<T>::type v2,
				   typename SPxAddCrefIfNotRef<T>::type v3,
				   typename SPxAddCrefIfNotRef<T>::type v4)
{
    SPxArray<T, 4> arr = { { v1, v2, v3, v4 } };
    return arr;
}

template<typename T>
inline SPxArray<T, 5> SPxMakeArray(typename SPxAddCrefIfNotRef<T>::type v1,
				   typename SPxAddCrefIfNotRef<T>::type v2,
				   typename SPxAddCrefIfNotRef<T>::type v3,
				   typename SPxAddCrefIfNotRef<T>::type v4,
				   typename SPxAddCrefIfNotRef<T>::type v5)
{
    SPxArray<T, 5> arr = { { v1, v2, v3, v4, v5 } };
    return arr;
}

#endif /* _SPX_ARRAY_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
