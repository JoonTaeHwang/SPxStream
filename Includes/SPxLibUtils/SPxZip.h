/*********************************************************************
*
* (c) Copyright 2015, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxZip function. This function allows multiple
*   objects to be iterated at the same time. The iteration stops
*   when the end of any of the objects is reached. The objects
*   can be STL compliant containers or arrays. The returned object
*   can be used conveniently with SPX_FOREACH or with the standard
*   iterator interface.
*
*   The element type is an SPxTuple of suitable values or references.
*   If the element is taken by value then the tuple types can be
*   'like' those in the objects, whereas if it is taken by reference
*   the types must match exactly.
*
*   std::vector<int> vec(156);
*   const float arr[256] = { };
*
*   // Typedef required to avoid confusing SPX_FOREACH with the comma.
*   typedef SPxTuple<int&, float> Element;
*   // Taking elements by value as the types do not match exactly (would be int&, const float&).
*   SPX_FOREACH(Element el, SPxZip(vec, arr))
*   {
*	int &vecVal = SPxGet<0>(el);
*	float arrVal = SPxGet<1>(el);
*   }
*
*   // Taking elements by reference, types must match exactly.
*   typedef SPxTuple<int&, const float&> Element2;
*   SPX_FOREACH(const Element2& el, SPxZip(vec, arr))
*   {
*	int &vecVal = SPxGet<0>(el);
*	float arrVal = SPxGet<1>(el);
*   }
*
*
* Revision Control:
*   20/06/18 v1.6    AGC	Suppress/fix cppcheck warnings.
*
* Previous Changes:
*   26/03/15 1.5    AGC	Support auto-zip of tuple elements.
*			Support zipping up to 5 containers.
*   06/03/15 1.4    AGC	Improve support for STL containers.
*   27/01/15 1.3    AGC	Fix remaining icc warnings.
*   26/01/15 1.2    AGC	Fix various icc warnings.
*   26/01/15 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_ZIP_H
#define _SPX_ZIP_H

#include "SPxLibUtils/SPxTuple.h"
#include "SPxLibUtils/SPxTypes.h"

namespace SPxZipNS
{
    template<typename C>
    struct ContTraits
    {
	typedef typename C::iterator iterator;
	typedef typename C::const_iterator const_iterator;
    };

    template<typename C>
    struct ContTraits<const C>
    {
	typedef typename C::const_iterator iterator;
	typedef typename C::const_iterator const_iterator;
    };

    template<typename C>
    struct ContTraits<C*>
    {
	typedef C* iterator;
	typedef const C* const_iterator;
    };

    template<typename C>
    struct ContTraits<const C*>
    {
	typedef const C* iterator;
	typedef const C* const_iterator;
    };

    template<typename C, unsigned int Len>
    struct ContTraits<C[Len]>
    {
	typedef C* iterator;
	typedef const C* const_iterator;
    };

    template<typename C, unsigned int Len>
    struct ContTraits<const C[Len]>
    {
	typedef const C* iterator;
	typedef const C* const_iterator;
    };

    template<typename T>
    struct IterTraits
    {
	typedef typename T::reference reference;
	typedef typename SPxAddConst<typename T::reference>::type const_reference;
    };

    template<typename T>
    struct IterTraits<T*>
    {
	typedef T& reference;
	typedef const T& const_reference;
    };

    template<typename T>
    struct IterTraits<const T*>
    {
	typedef const T& reference;
	typedef const T& const_reference;
    };

    template<typename T, unsigned int Len>
    struct IterTraits<T[Len]>
    {
	typedef T& reference;
	typedef const T& const_reference;
    };

    template<typename T, unsigned int Len>
    struct IterTraits<const T[Len]>
    {
	typedef const T& reference;
	typedef const T& const_reference;
    };

    template<typename C>
    inline typename ContTraits<C>::iterator Begin(C& c) { return c.begin(); }

    template<typename C, unsigned int Len>
    inline typename ContTraits<C*>::iterator Begin(C (&c)[Len]) { return c; }

    template<typename C>
    inline typename ContTraits<C>::iterator End(C& c) { return c.end(); }

    template<typename C, unsigned int Len>
    inline typename ContTraits<C*>::iterator End(C (&c)[Len]) { return c + Len; }

    template<typename C>
    inline typename ContTraits<C>::const_iterator CBegin(const C& c) { return c.begin(); }

    template<typename C, unsigned int Len>
    inline typename ContTraits<C*>::const_iterator CBegin(const C (&c)[Len]) { return c; }

    template<typename C>
    inline typename ContTraits<C>::const_iterator CEnd(const C& c) { return c.end(); }

    template<typename C, unsigned int Len>
    inline typename ContTraits<C*>::const_iterator CEnd(const C (&c)[Len]) { return c + Len; }

    template<typename I1, typename I2, typename I3=void, typename I4=void, typename I5=void>
    class Iterator;

    template<typename I1, typename I2>
    class Iterator<I1, I2, void, void, void>
    {
    public:
	typedef typename IterTraits<I1>::reference reference1;
	typedef typename IterTraits<I2>::reference reference2;
	typedef typename IterTraits<I1>::const_reference const_reference1;
	typedef typename IterTraits<I2>::const_reference const_reference2;
	typedef SPxTuple<reference1, reference2> reference;
	typedef SPxTuple<const_reference1, const_reference2> const_reference;

	Iterator(I1 i1, I2 i2) : m_i1(i1), m_i2(i2) {}
	Iterator(const Iterator& other) : m_i1(other.m_i1), m_i2(other.m_i2) {}

	Iterator& operator++() { ++m_i1; ++m_i2; return *this; }
	Iterator operator++(int) {
	    Iterator<I1, I2> tmp(*this);
	    operator++();
	    return tmp;
	}
	reference operator*() { return reference(*m_i1, *m_i2); }
	const_reference operator*() const { return const_reference(*m_i1, *m_i2); }
	reference operator->() { return reference(*m_i1, *m_i2); }
	const_reference operator->() const { return const_reference(*m_i1, *m_i2); }

	friend inline bool operator==(const Iterator<I1, I2>& lhs, const Iterator<I1, I2>& rhs)
	{
	    /* This statement contains ORs rather than ANDs so that we
	     * stop iterating when ANY of the iterators gets to the end.
	     */
	    return (lhs.m_i1 == rhs.m_i1) || (lhs.m_i2 == rhs.m_i2);
	}

    private:
	I1 m_i1;
	I2 m_i2;
    };

    template<typename I1, typename I2>
    inline bool operator!=(const Iterator<I1, I2>& nlhs, const Iterator<I1, I2>& nrhs)
    {
	return !operator==(nlhs, nrhs);
    }

    template<typename I1, typename I2, typename I3>
    class Iterator<I1, I2, I3, void, void>
    {
    public:
	typedef typename IterTraits<I1>::reference reference1;
	typedef typename IterTraits<I2>::reference reference2;
	typedef typename IterTraits<I3>::reference reference3;
	typedef typename IterTraits<I1>::const_reference const_reference1;
	typedef typename IterTraits<I2>::const_reference const_reference2;
	typedef typename IterTraits<I3>::const_reference const_reference3;
	typedef SPxTuple<reference1, reference2, reference3> reference;
	typedef SPxTuple<const_reference1, const_reference2, const_reference3> const_reference;

	Iterator(I1 i1, I2 i2, I3 i3) : m_i1(i1), m_i2(i2), m_i3(i3) {}
	Iterator(const Iterator &other) : m_i1(other.m_i1), m_i2(other.m_i2), m_i3(other.m_i3) {}

	Iterator& operator++() { ++m_i1; ++m_i2; ++m_i3; return *this; }
	Iterator operator++(int) {
	    Iterator<I1, I2, I3> tmp(*this);
	    operator++();
	    return tmp;
	}
	reference operator*() { return reference(*m_i1, *m_i2, *m_i3); }
	const_reference operator*() const { return const_reference(*m_i1, *m_i2, *m_i3); }
	reference operator->() { return reference(*m_i1, *m_i2, *m_i3); }
	const_reference operator->() const { return const_reference(*m_i1, *m_i2, *m_i3); }
	
	friend inline bool operator==(const Iterator<I1, I2, I3>& lhs, const Iterator<I1, I2, I3>& rhs)
	{
	    /* This statement contains ORs rather than ANDs so that we
	     * stop iterating when ANY of the iterators gets to the end.
	     */
	    return (lhs.m_i1 == rhs.m_i1) || (lhs.m_i2 == rhs.m_i2) || (lhs.m_i3 == rhs.m_i3);
	}

    private:
	I1 m_i1;
	I2 m_i2;
	I3 m_i3;
    };

    template<typename I1, typename I2, typename I3>
    inline bool operator!=(const Iterator<I1, I2, I3>& nlhs, const Iterator<I1, I2, I3>& nrhs)
    {
	return !operator==(nlhs, nrhs);
    }

    template<typename I1, typename I2, typename I3, typename I4>
    class Iterator<I1, I2, I3, I4, void>
    {
    public:
	typedef typename IterTraits<I1>::reference reference1;
	typedef typename IterTraits<I2>::reference reference2;
	typedef typename IterTraits<I3>::reference reference3;
	typedef typename IterTraits<I4>::reference reference4;
	typedef typename IterTraits<I1>::const_reference const_reference1;
	typedef typename IterTraits<I2>::const_reference const_reference2;
	typedef typename IterTraits<I3>::const_reference const_reference3;
	typedef typename IterTraits<I4>::const_reference const_reference4;
	typedef SPxTuple<reference1, reference2, reference3, reference4> reference;
	typedef SPxTuple<const_reference1, const_reference2, const_reference3, const_reference4> const_reference;

	Iterator(I1 i1, I2 i2, I3 i3, I4 i4) : m_i1(i1), m_i2(i2), m_i3(i3), m_i4(i4) {}
	Iterator(const Iterator &other) : m_i1(other.m_i1), m_i2(other.m_i2), m_i3(other.m_i3), m_i4(other.m_i4) {}

	Iterator& operator++() { ++m_i1; ++m_i2; ++m_i3; ++m_i4; return *this; }
	Iterator operator++(int) {
	    Iterator<I1, I2, I3, I4> tmp(*this);
	    operator++();
	    return tmp;
	}
	reference operator*() { return reference(*m_i1, *m_i2, *m_i3, *m_i4); }
	const_reference operator*() const { return const_reference(*m_i1, *m_i2, *m_i3, *m_i4); }
	reference operator->() { return reference(*m_i1, *m_i2, *m_i3, *m_i4); }
	const_reference operator->() const { return const_reference(*m_i1, *m_i2, *m_i3, *m_i4); }
	
	friend inline bool operator==(const Iterator<I1, I2, I3, I4>& lhs, const Iterator<I1, I2, I3, I4>& rhs)
	{
	    /* This statement contains ORs rather than ANDs so that we
	     * stop iterating when ANY of the iterators gets to the end.
	     */
	    return (lhs.m_i1 == rhs.m_i1) || (lhs.m_i2 == rhs.m_i2) || (lhs.m_i3 == rhs.m_i3) || (lhs.m_i4 == rhs.m_i4);
	}

    private:
	I1 m_i1;
	I2 m_i2;
	I3 m_i3;
	I4 m_i4;
    };

    template<typename I1, typename I2, typename I3, typename I4>
    inline bool operator!=(const Iterator<I1, I2, I3, I4>& nlhs, const Iterator<I1, I2, I3, I4>& nrhs)
    {
	return !operator==(nlhs, nrhs);
    }

    template<typename I1, typename I2, typename I3, typename I4, typename I5>
    class Iterator
    {
    public:
	typedef typename IterTraits<I1>::reference reference1;
	typedef typename IterTraits<I2>::reference reference2;
	typedef typename IterTraits<I3>::reference reference3;
	typedef typename IterTraits<I4>::reference reference4;
	typedef typename IterTraits<I5>::reference reference5;
	typedef typename IterTraits<I1>::const_reference const_reference1;
	typedef typename IterTraits<I2>::const_reference const_reference2;
	typedef typename IterTraits<I3>::const_reference const_reference3;
	typedef typename IterTraits<I4>::const_reference const_reference4;
	typedef typename IterTraits<I5>::const_reference const_reference5;
	typedef SPxTuple<reference1, reference2, reference3, reference4, reference5> reference;
	typedef SPxTuple<const_reference1, const_reference2, const_reference3, const_reference4, const_reference5> const_reference;

	Iterator(I1 i1, I2 i2, I3 i3, I4 i4, I5 i5) : m_i1(i1), m_i2(i2), m_i3(i3), m_i4(i4), m_i5(i5) {}
	Iterator(const Iterator &other) : m_i1(other.m_i1), m_i2(other.m_i2), m_i3(other.m_i3), m_i4(other.m_i4), m_i5(other.m_i5) {}

	Iterator& operator=(const Iterator &other)
	{
	    m_i1 = other.m_i1;
	    m_i2 = other.m_i2;
	    m_i3 = other.m_i3;
	    m_i4 = other.m_i4;
	    m_i5 = other.m_i5;
	    return *this;
	}

	Iterator& operator++() { ++m_i1; ++m_i2; ++m_i3; ++m_i4; ++m_i5; return *this; }
	Iterator operator++(int) {
	    Iterator<I1, I2, I3, I4, I5> tmp(*this);
	    operator++();
	    return tmp;
	}
	reference operator*() { return reference(*m_i1, *m_i2, *m_i3, *m_i4, *m_i5); }
	const_reference operator*() const { return const_reference(*m_i1, *m_i2, *m_i3, *m_i4, *m_i5); }
	reference operator->() { return reference(*m_i1, *m_i2, *m_i3, *m_i4, *m_i5); }
	const_reference operator->() const { return const_reference(*m_i1, *m_i2, *m_i3, *m_i4, *m_i5); }
	
	friend inline bool operator==(const Iterator<I1, I2, I3, I4, I5>& lhs, const Iterator<I1, I2, I3, I4, I5>& rhs)
	{
	    /* This statement contains ORs rather than ANDs so that we
	     * stop iterating when ANY of the iterators gets to the end.
	     */
	    return (lhs.m_i1 == rhs.m_i1) || (lhs.m_i2 == rhs.m_i2) || (lhs.m_i3 == rhs.m_i3) || (lhs.m_i4 == rhs.m_i4) || (lhs.m_i5 == rhs.m_i5);
	}

    private:
	I1 m_i1;
	I2 m_i2;
	I3 m_i3;
	I4 m_i4;
	I5 m_i5;
    };

    template<typename I1, typename I2, typename I3, typename I4, typename I5>
    inline bool operator!=(const Iterator<I1, I2, I3, I4, I5>& nlhs, const Iterator<I1, I2, I3, I4, I5>& nrhs)
    {
	return !operator==(nlhs, nrhs);
    }

    template<typename C1, typename C2=void, typename C3=void, typename C4=void, typename C5=void>
    class Range;

    template<typename C>
    class Range<C, void, void, void, void>
    {
    public:
	typedef typename ContTraits<C>::iterator iterator;
	typedef typename ContTraits<C>::iterator const_iterator;

	Range(C &c) : m_begin(Begin(c)), m_end(End(c)) {}
	//cppcheck-suppress uninitMemberVar
	Range(const Range& other) : m_begin(other.m_begin), m_end(other.m_end) {}

	//cppcheck-suppress operatorEqVarError
	Range& operator=(const Range& other)
	{
	    m_begin = other.m_begin;
	    m_end = other.m_end;
	    return *this;
	}

	iterator begin() const { return m_begin; }
	iterator end() const { return m_end; }

    private:
	iterator m_begin;
	iterator m_end;
    };

    template<typename C1, typename C2>
    class Range<C1, C2, void, void, void>
    {
    public:
	typedef Iterator<typename ContTraits<C1>::iterator, typename ContTraits<C2>::iterator> iterator;
	typedef Iterator<typename ContTraits<C1>::iterator, typename ContTraits<C2>::iterator> const_iterator;

	Range(C1 &c1, C2 &c2)
	    : m_begin(Begin(c1), Begin(c2)), m_end(End(c1), End(c2)) {}
	//cppcheck-suppress uninitMemberVar
	Range(const Range& other) : m_begin(other.m_begin), m_end(other.m_end) {}

	//cppcheck-suppress operatorEqVarError
	Range& operator=(const Range& other)
	{
	    m_begin = other.m_begin;
	    m_end = other.m_end;
	    return *this;
	}

	iterator begin() const { return m_begin; }
	iterator end() const { return m_end; }

    private:
	iterator m_begin;
	iterator m_end;
    };

    template<typename C1, typename C2, typename C3>
    class Range<C1, C2, C3, void, void>
    {
    public:
	typedef Iterator<typename ContTraits<C1>::iterator, typename ContTraits<C2>::iterator, typename ContTraits<C3>::iterator> iterator;
	typedef Iterator<typename ContTraits<C1>::iterator, typename ContTraits<C2>::iterator, typename ContTraits<C3>::iterator> const_iterator;

	Range(C1 &c1, C2 &c2, C3 &c3)
	    : m_begin(Begin(c1), Begin(c2), Begin(c3)), m_end(End(c1), End(c2), End(c3)) {}
	//cppcheck-suppress uninitMemberVar
	Range(const Range& other) : m_begin(other.m_begin), m_end(other.m_end) {}

	//cppcheck-suppress operatorEqVarError
	Range& operator=(const Range& other)
	{
	    m_begin = other.m_begin;
	    m_end = other.m_end;
	    return *this;
	}

	iterator begin() const { return m_begin; }
	iterator end() const { return m_end; }

    private:
	iterator m_begin;
	iterator m_end;
    };

    template<typename C1, typename C2, typename C3, typename C4>
    class Range<C1, C2, C3, C4, void>
    {
    public:
	typedef Iterator<typename ContTraits<C1>::iterator, typename ContTraits<C2>::iterator, typename ContTraits<C3>::iterator, typename ContTraits<C4>::iterator> iterator;
	typedef Iterator<typename ContTraits<C1>::iterator, typename ContTraits<C2>::iterator, typename ContTraits<C3>::iterator, typename ContTraits<C4>::iterator> const_iterator;

	Range(C1 &c1, C2 &c2, C3 &c3, C4 &c4)
	    : m_begin(Begin(c1), Begin(c2), Begin(c3), Begin(c4)), m_end(End(c1), End(c2), End(c3), End(c4)) {}
	//cppcheck-suppress uninitMemberVar
	Range(const Range& other) : m_begin(other.m_begin), m_end(other.m_end) {}

	//cppcheck-suppress operatorEqVarError
	Range& operator=(const Range& other)
	{
	    m_begin = other.m_begin;
	    m_end = other.m_end;
	    return *this;
	}

	iterator begin() const { return m_begin; }
	iterator end() const { return m_end; }

    private:
	iterator m_begin;
	iterator m_end;
    };

    template<typename C1, typename C2, typename C3, typename C4, typename C5>
    class Range
    {
    public:
	typedef Iterator<typename ContTraits<C1>::iterator, typename ContTraits<C2>::iterator, typename ContTraits<C3>::iterator, typename ContTraits<C4>::iterator, typename ContTraits<C5>::iterator> iterator;
	typedef Iterator<typename ContTraits<C1>::iterator, typename ContTraits<C2>::iterator, typename ContTraits<C3>::iterator, typename ContTraits<C4>::iterator, typename ContTraits<C5>::iterator> const_iterator;

	Range(C1 &c1, C2 &c2, C3 &c3, C4 &c4, C5 &c5)
	    : m_begin(Begin(c1), Begin(c2), Begin(c3), Begin(c4), Begin(c5)), m_end(End(c1), End(c2), End(c3), End(c4), End(c5)) {}
	//cppcheck-suppress uninitMemberVar
	Range(const Range& other) : m_begin(other.m_begin), m_end(other.m_end) {}

	//cppcheck-suppress operatorEqVarError
	Range& operator=(const Range& other)
	{
	    m_begin = other.m_begin;
	    m_end = other.m_end;
	    return *this;
	}

	iterator begin() const { return m_begin; }
	iterator end() const { return m_end; }

    private:
	iterator m_begin;
	iterator m_end;
    };
}

template<typename C1>
inline const SPxZipNS::Range<C1> SPxZip(C1 &c1)
{
    return SPxZipNS::Range<C1>(c1);
}

template<typename C1, typename C2>
inline const SPxZipNS::Range<C1, C2> SPxZip(C1 &c1, C2 &c2)
{
    return SPxZipNS::Range<C1, C2>(c1, c2);
}

template<typename C1, typename C2, typename C3>
inline const SPxZipNS::Range<C1, C2, C3> SPxZip(C1 &c1, C2 &c2, C3 &c3)
{
    return SPxZipNS::Range<C1, C2, C3>(c1, c2, c3);
}

template<typename C1, typename C2, typename C3, typename C4>
inline const SPxZipNS::Range<C1, C2, C3, C4> SPxZip(C1 &c1, C2 &c2, C3 &c3, C4 &c4)
{
    return SPxZipNS::Range<C1, C2, C3, C4>(c1, c2, c3, c4);
}

template<typename C1, typename C2, typename C3, typename C4, typename C5>
inline const SPxZipNS::Range<C1, C2, C3, C4, C5> SPxZip(C1 &c1, C2 &c2, C3 &c3, C4 &c4, C5 &c5)
{
    return SPxZipNS::Range<C1, C2, C3, C4, C5>(c1, c2, c3, c4, c5);
}

template<typename C1>
inline const SPxZipNS::Range<typename SPxRemoveRef<C1>::type> SPxZip(SPxTuple<C1>& t)
{
    return SPxZip(SPxGet<0>(t));
}

template<typename C1, typename C2>
inline const SPxZipNS::Range<typename SPxRemoveRef<C1>::type, typename SPxRemoveRef<C2>::type> SPxZip(SPxTuple<C1, C2>& t)
{
    return SPxZip(SPxGet<0>(t), SPxGet<1>(t));
}

template<typename C1, typename C2, typename C3>
inline const SPxZipNS::Range<typename SPxRemoveRef<C1>::type, typename SPxRemoveRef<C2>::type, typename SPxRemoveRef<C3>::type> SPxZip(SPxTuple<C1, C2, C3>& t)
{
    return SPxZip(SPxGet<0>(t), SPxGet<1>(t), SPxGet<2>(t));
}

template<typename C1, typename C2, typename C3, typename C4>
inline const SPxZipNS::Range<typename SPxRemoveRef<C1>::type, typename SPxRemoveRef<C2>::type, typename SPxRemoveRef<C3>::type, typename SPxRemoveRef<C4>::type> SPxZip(SPxTuple<C1, C2, C3, C4>& t)
{
    return SPxZip(SPxGet<0>(t), SPxGet<1>(t), SPxGet<2>(t), SPxGet<3>(t));
}

template<typename C1, typename C2, typename C3, typename C4, typename C5>
inline const SPxZipNS::Range<typename SPxRemoveRef<C1>::type, typename SPxRemoveRef<C2>::type, typename SPxRemoveRef<C3>::type, typename SPxRemoveRef<C4>::type, typename SPxRemoveRef<C5>::type> SPxZip(SPxTuple<C1, C2, C3, C4, C5>& t)
{
    return SPxZip(SPxGet<0>(t), SPxGet<1>(t), SPxGet<2>(t), SPxGet<3>(t), SPxGet<4>(t));
}



#endif /* _SPX_ZIP_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
