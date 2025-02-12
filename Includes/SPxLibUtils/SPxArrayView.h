/*********************************************************************
*
* (c) Copyright 2015, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxArrayView class - a templated header only
*   class encapsulating a view of some portion of an array.
*
*   The portion of the array can either be a 1D or 2D area.
*   This portion of the array can then be iterated over using
*   standard the iterator interface.
*
*   Additionally, values within this portion can be indexed using
*   a 2D indexing operation:
*
*	SPxArray<int, 64> arr;
*	SPxArrayView<int> arrView(arr, SPxAreaI(2, 2, 4, 4), 8);
*
*	// Get the element at pos (2, 2) in arrView, which is at
*	// position 36 in the underlying array.
*	int val = arrView[SPxPosI(2, 2)];
*
*   Modifications can be made safely through the array view.
*
*
* Revision Control:
*   20/06/18 v1.6    AGC	Suppress spurious cppcheck warnings.
*
* Previous Changes:
*   04/12/15 1.5    AGC	Add all required operators for iterator.
*   14/04/15 1.4    AGC	Add support for rotations.
*   27/01/15 1.3    AGC	Fix various shadow warnings.
*   26/01/15 1.2    AGC	Fix various icc warnings.
*   26/01/15 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_ARRAY_VIEW_H
#define _SPX_ARRAY_VIEW_H

/* Standard headers. */
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstddef>
#include <iterator>
#include "SPxLibUtils/SPxArea.h"
#include "SPxLibUtils/SPxArray.h"
#include "SPxLibUtils/SPxMaths.h"

template <class C, class elem_type=typename C::value_type>
class SPxArrayView_iterator
{
public:
    typedef SPxArrayView_iterator<C, elem_type> self_type;
    typedef C buffer_type;
    typedef typename C::size_type size_type;
    typedef std::random_access_iterator_tag iterator_category;
    typedef elem_type value_type;
    typedef elem_type* pointer;
    typedef const elem_type* const_pointer;
    typedef elem_type& reference;
    typedef const elem_type& const_reference;
    typedef typename C::difference_type difference_type;

    SPxArrayView_iterator(buffer_type *buffer=NULL, size_type pos=0U)
	: m_buf(buffer), m_pos(pos) {}
    ~SPxArrayView_iterator() {}

    elem_type &operator*() { return (*m_buf)[m_pos]; }
    elem_type *operator->() { return &(operator*()); }
    self_type &operator++() { ++m_pos; return *this; }
    self_type operator++(int) { self_type tmp(*this); ++(*this); return tmp; }
    self_type operator+(difference_type n) { self_type tmp(*this); tmp.m_pos += n; return tmp; }
    self_type &operator+=(difference_type n) { m_pos += n; return *this; }
    self_type &operator--() { --m_pos; return *this; }
    self_type operator--(int) { self_type tmp(*this); --(*this); return tmp; }
    self_type operator-(difference_type n) { self_type tmp(*this); tmp.m_pos -= n; return tmp; }
    self_type &operator-=(difference_type n) { m_pos -= n; return *this; }
    difference_type operator-(const self_type& other) const { return m_pos - other.m_pos; }
    bool operator==(const self_type &other) const { return (m_buf == other.m_buf) && (m_pos == other.m_pos); }
    bool operator!=(const self_type &other) const { return (m_buf != other.m_buf) || (m_pos != other.m_pos);
 }
    bool operator<(const self_type& other) const { return (other - *this) > difference_type(); }
    bool operator>(const self_type& other) const { return other < *this; }
    bool operator<=(const self_type& other) const { return !(*this < other); }
    bool operator>=(const self_type& other) const { return !(*this > other); }

private:
    buffer_type *m_buf;
    size_type m_pos;
};

template <class T>
//cppcheck-suppress copyCtorAndEqOperator
class SPxArrayView
{
public:
    typedef SPxArrayView<T> self_type;
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;
    typedef SPxArrayView_iterator<self_type> iterator;
    typedef SPxArrayView_iterator<const self_type, const value_type> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    template<size_type N>
    explicit SPxArrayView(SPxArray<T, N>& arr)
	: m_len(arr.size()), m_area(0, 0, m_len, 1), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(), m_pitch(m_len), m_data(arr.data()), m_dummy() {  }
    template<size_type N>
    explicit SPxArrayView(T (&arr)[N])
	: m_len(N), m_area(0, 0, m_len, 1), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(), m_pitch(m_len), m_data(arr), m_dummy() {}
    explicit SPxArrayView(pointer pBegin, pointer pEnd)
	: m_len(pEnd - pBegin), m_area(0, 0, m_len, 1), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(), m_pitch(m_len), m_data(pBegin), m_dummy() {}
    explicit SPxArrayView(pointer p, size_type len)
	: m_len(len), m_area(0, 0, m_len, 1), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(), m_pitch(m_len), m_data(p), m_dummy() {}
    template<size_type N, typename S1>
    explicit SPxArrayView(SPxArray<T, N>& arr, const SPxArea<S1> &area_, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(N), m_area(area_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(area_.W), m_data(arr.data()), m_dummy() { configRot(rotDegs); }
    template<size_type N, typename S1>
    explicit SPxArrayView(SPxArray<T, N>& arr, const SPxArea<S1> &area_, size_type pitch, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(N), m_area(area_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(pitch), m_data(arr.data()), m_dummy() { configRot(rotDegs); }
    template<size_type N>
    explicit SPxArrayView(T (&arr)[N], const SPxAreaU &area_, size_type pitch, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(N), m_area(area_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(pitch), m_data(arr), m_dummy() { configRot(rotDegs); }
    template<size_type N>
    explicit SPxArrayView(T (&arr)[N], const SPxAreaU &area_, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(N), m_area(area_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(area_.W), m_data(arr), m_dummy() { configRot(rotDegs); }
    template<typename S1>
    explicit SPxArrayView(pointer pBegin, pointer pEnd, const SPxArea<S1>& area_, size_type pitch, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(pEnd - pBegin), m_area(area_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(pitch), m_data(pBegin), m_dummy() { configRot(rotDegs); }
    template<typename S1>
    explicit SPxArrayView(pointer pBegin, pointer pEnd, const SPxArea<S1>& area_, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(pEnd - pBegin), m_area(area_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(area_.W), m_data(pBegin), m_dummy() { configRot(rotDegs); }
    template<typename S1>
    explicit SPxArrayView(pointer p, size_type len, const SPxArea<S1> &area_, size_type pitch, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(len), m_area(area_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(pitch), m_data(p), m_dummy() { configRot(rotDegs); }
    template<typename S1>
    explicit SPxArrayView(pointer p, size_type len, const SPxArea<S1> &area_, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(len), m_area(area_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(area_.W), m_data(p), m_dummy() { configRot(rotDegs); }
    template<typename S1>
    explicit SPxArrayView(pointer p, const SPxSize<S1> &size_, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(size_.area()), m_area(SPxPos<size_type>(), size_), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(size_.W), m_data(p), m_dummy() { configRot(rotDegs); }
    SPxArrayView(const self_type &other)
	: m_len(other.m_len), m_area(other.m_area), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(), m_pitch(other.m_pitch), m_data(other.m_data), m_dummy() {}
    template<typename S1>
    SPxArrayView(const self_type &other, const SPxArea<S1> &area_, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
	: m_len(other.m_len), m_area(other.m_area.Pos + area_.Pos, area_.Size), m_rot(false), m_sinRot(0.0), m_cosRot(0.0), m_rotCentre(rotCentre), m_pitch(other.m_pitch), m_data(other.m_data), m_dummy() { configRot(rotDegs); }
    ~SPxArrayView() {}
    iterator begin() { return iterator(this, 0U); }
    iterator end() { return iterator(this, size()); }
    const_iterator begin() const { return const_iterator(this, 0U); }
    const_iterator end() const { return const_iterator(this, size()); }
    reverse_iterator rbegin() { return reverse_iterator(iterator(this, size())); }
    reverse_iterator rend() { return reverse_iterator(iterator(this, 0U)); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(const_iterator(this, size())); }
    const_reverse_iterator rend() const { return const_reverse_iterator(const_iterator(this, 0U)); }
    template<typename U>
    reference operator[](const U& pos)
    {
	SPxPos<difference_type> pos2(pos2D(pos));
	if( !within(pos2) )
	{
	    m_dummy = T();
	    return m_dummy;
	}
	return m_data[pos2.Y * m_pitch + pos2.X];
    }
    template<typename U>
    const_reference operator[](const U& pos) const
    {
	SPxPos<difference_type> pos2(pos2D(pos));
	if( !within(pos2) )
	{
	    m_dummy = T();
	    return m_dummy;
	}
	return m_data[pos2.Y * m_pitch + pos2.X];
    }
    reference front() { return operator[](0U); }
    reference back() { return operator[](size() - 1); }
    const_reference front() const { return operator[](0U); }
    const_reference back() const { return operator[](size() - 1); }
    pointer data() { return &m_data[0]; }
    const_pointer data() const { return &m_data[0]; }
    size_type size() const { return m_area.area(); }
    SPxArea<difference_type> area() const { return m_area; }
    difference_type x() const { return m_area.X; }
    difference_type y() const { return m_area.Y; }
    difference_type width() const { return m_area.W; }
    difference_type height() const { return m_area.H; }
    bool empty() const { return (m_len == 0) && (m_area.area() == 0); }
    bool within(const SPxPos<difference_type>& pos) const
    {
	return (pos.X >= 0) && (static_cast<size_type>(pos.X) < m_pitch)
	       && (pos.Y >= 0) && (static_cast<size_type>(pos.Y * m_pitch + pos.X) < m_len);
    }
    SPxPos<difference_type> pos2D(size_type pos) const
    {
	return SPxPos<difference_type>(m_area.X + pos % m_area.W, m_area.Y + pos / m_area.W);
    }
    template<typename U>
    SPxPos<difference_type> pos2D(const SPxPos<U>& pos) const
    {
	if( !m_rot )
	{
	    return SPxPos<difference_type>(m_area.X + pos.X, m_area.Y + pos.Y);
	}

	const SPxPosD posD(pos);
	const SPxPosD subPosD(SPxPosD(m_area.tl()) + posD - m_rotCentre);
	const SPxPosD rotPosD(subPosD.X * m_cosRot - subPosD.Y * m_sinRot,
	    subPosD.X * m_sinRot + subPosD.Y * m_cosRot);
	const SPxPosD addPosD(rotPosD + m_rotCentre);
	return SPxPos<difference_type>(
	    static_cast<difference_type>(SPxMathsRound(addPosD.X)),
	    static_cast<difference_type>(SPxMathsRound(addPosD.Y)));
    }

private:
    SPxArrayView() {}
    void configRot(double rotDegs)
    {
	if( rotDegs != 0.0 )
	{
	    m_rot = true;
	    const double rotRads = rotDegs * M_PI / 180.0;
	    m_sinRot = std::sin(rotRads);
	    m_cosRot = std::cos(rotRads);
	}
    }

    const size_type m_len;
    const SPxArea<difference_type> m_area;
    bool m_rot;
    double m_sinRot;
    double m_cosRot;
    const SPxPosD m_rotCentre;
    const size_type m_pitch;
    T *m_data;
    mutable typename SPxRemoveConst<T>::type m_dummy;
};

template<typename T, std::size_t N>
inline SPxArrayView<T> SPxMakeArrayView(SPxArray<T, N>& arr)
{
    return SPxArrayView<T>(arr);
}

template<typename T>
inline SPxArrayView<T> SPxMakeArrayView(T *pBegin, T *pEnd)
{
    return SPxArrayView<T>(pBegin, pEnd);
}

template<typename T, std::size_t N>
inline SPxArrayView<T> SPxMakeArrayView(T (&arr)[N])
{
    return SPxArrayView<T>(arr);
}

template<typename T>
inline SPxArrayView<T> SPxMakeArrayView(T *p, std::size_t len)
{
    return SPxArrayView<T>(p, len);
}

template<typename T, std::size_t N, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(SPxArray<T, N>& arr, const SPxArea<S1> &area, std::size_t pitch, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(arr, area, pitch, rotDegs, rotCentre);
}

template<typename T, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(T *pBegin, T *pEnd, const SPxArea<S1> &area, std::size_t pitch, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(pBegin, pEnd, area, pitch, rotDegs, rotCentre);
}

template<typename T, std::size_t N, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(T (&arr)[N], const SPxArea<S1> &area, std::size_t pitch, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(arr, area, pitch, rotDegs, rotCentre);
}

template<typename T, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(T *p, std::size_t len, const SPxArea<S1> &area, std::size_t pitch, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(p, len, area, pitch, rotDegs, rotCentre);
}

template<typename T, std::size_t N, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(SPxArray<T, N>& arr, const SPxArea<S1> &area, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(arr, area, rotDegs, rotCentre);
}

template<typename T, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(T *pBegin, T *pEnd, const SPxArea<S1> &area, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(pBegin, pEnd, area, rotDegs, rotCentre);
}

template<typename T, std::size_t N, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(T (&arr)[N], const SPxArea<S1> &area, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(arr, area, rotDegs, rotCentre);
}

template<typename T, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(T *p, std::size_t len, const SPxArea<S1> &area, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(p, len, area, rotDegs, rotCentre);
}

template<typename T, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(T *p, const SPxSize<S1> &size, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(p, size, rotDegs, rotCentre);
}

template<typename T, typename S1>
inline SPxArrayView<T> SPxMakeArrayView(const SPxArrayView<T> &other, const SPxArea<S1> &area, double rotDegs=0.0, const SPxPosD& rotCentre=SPxPosD())
{
    return SPxArrayView<T>(other, area, rotDegs, rotCentre);
}


#endif /* _SPX_ARRAY_VIEW_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
