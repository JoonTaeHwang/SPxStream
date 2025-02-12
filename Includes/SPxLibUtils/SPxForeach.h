/*********************************************************************
*
* (c) Copyright 2014 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxForeach.h,v $
* ID: $Id: SPxForeach.h,v 1.8 2016/05/23 14:18:29 rew Exp $
*
* Purpose:
*   SPX_FOREACH() macro allows simple iteration over STL containers
*   and arrays without the boilerplate.
*
*	std::vector<SPxObj*> vec;
*	SPX_FOREACH(SPxObj *obj, vec)
*	{
*	    obj->SetName("Test");
*	}
*
*	const std::set<int> vals;
*	SPX_FOREACH(const int& v, vals)
*	{
*	    int a = v;
*	}
*
*	unsigned long data[] = { 1, 2, 3, 7, 10 };
*	SPX_FOREACH(unsigned long c, data)
*	{
*	    std::printf("%u\n", c);
*	}
*
*   SPX_FOREACH_REV() macro allows iteration in reverse for STL
*   containers only.
*
*   Invocations of SPX_FOREACH/SPX_FOREACH_REV() may be nested.
*
*   The types inside SPX_FOREACH(here!) may NOT contain commas
*   (e.g. std::pair<int, int>) - use a typedef if this is required.
*
* Revision Control:
*   23/05/16 v1.8    AGC	Used native range based for on VS2012+.
*
* Previous Changes:
*   11/05/15 1.7    AGC	Improve check for standard range based for detection.
*   27/01/15 1.6    AGC	Take iterator by value in Any() constructor.
*   26/01/15 1.5    AGC	Use traits to get iterator reference.
*   13/01/15 1.4    AGC	Fix end() for mutable arrays.
*   10/11/14 1.3    AGC	Fix another icc warning.
*   07/11/14 1.2    AGC	Fix icc warnings.
*   07/11/14 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_FOREACH_H
#define _SPX_FOREACH_H

#ifndef SPX_NATIVE_FOREACH
#if defined(_WIN32)
#if (_MSC_VER >= 1700)
#define SPX_NATIVE_FOREACH
#endif /* _MSC_VER */
#else
#if __cplusplus >= 201103L
#if defined(__INTEL_COMPILER)
#if (__GNUC__ >= 13)
#define SPX_NATIVE_FOREACH
#endif
#elif defined(__clang__)
#if (__clang_major__ >= 3)
#define SPX_NATIVE_FOREACH
#endif
#elif defined(__GNUC__)
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#define SPX_NATIVE_FOREACH
#endif
#endif /* Linux variants */
#endif /* C++ version check */
#endif /* Windows/Linux */
#endif /* SPX_NATIVE_FOREACH */

#ifdef SPX_NATIVE_FOREACH

#include "SPxLibUtils/SPxTuple.h"

namespace SPxForeachNS
{
    template<typename C>
    class Range
    {
    public:
	Range(C &c) : m_begin(c.rbegin()), m_end(c.rend()) {}

	typename C::reverse_iterator begin() { return m_begin; }
	typename C::reverse_iterator end() { return m_end; }

    private:
	typename C::reverse_iterator m_begin;
	typename C::reverse_iterator m_end;
    };

    template<typename C>
    class ConstRange
    {
    public:
	ConstRange(const C &c) : m_begin(c.rbegin()), m_end(c.rend()) {}

	typename C::const_reverse_iterator begin() { return m_begin; }
	typename C::const_reverse_iterator end() { return m_end; }

    private:
	typename C::const_reverse_iterator m_begin;
	typename C::const_reverse_iterator m_end;
    };

    template<typename C>
    Range<C> Reverse(C &c)
    {
	return Range<C>(c);
    }

    template<typename C>
    ConstRange<C> Reverse(const C &c)
    {
	return ConstRange<C>(c);
    }
}

#define SPX_FOREACH(var, cont) for(var: cont)

#define SPX_FOREACH_REV(var, cont) for(var: SPxForeachNS::Reverse(cont))

#else

/* Namespace for structures and functions used within the foreach implementation. */
namespace SPxForeachNS
{
    /* Structures for type matching on true/false. */
    struct TrueType { static const bool value = true; };
    struct FalseType { static const bool value = false; };

    /* Structure that can hold any type with common base. */
    template<typename T>
    struct AnyType : public T {};

    struct AnyBase
    {
	AnyBase() {}
	virtual ~AnyBase() {}
	operator bool() const { return false; }
    };
    template<typename T>
    struct Any : public AnyBase
    {
	Any(T t) : m_t(t) {}

	mutable T m_t;
    };

    template<typename T>
    struct IterTraits
    {
	typedef typename T::reference reference;
    };

    template<typename T>
    struct IterTraits<T*>
    {
	typedef T& reference;
    };

    template<typename T>
    struct IterTraits<const T*>
    {
	typedef const T& reference;
    };

    /* Various types of ranges. */
    template<typename T>
    struct RangeIter
    {
	typedef typename T::iterator type;
	typedef typename IterTraits<typename T::iterator>::reference ref;
    };

    template<typename T, unsigned int Len>
    struct RangeIter<T[Len]>
    {
	typedef T* type;
	typedef T& ref;
    };

    template<typename T>
    struct ConstRangeIter
    {
	typedef typename T::const_iterator type;
	typedef typename IterTraits<typename T::const_iterator>::reference ref;
    };

    template<typename T, unsigned int Len>
    struct ConstRangeIter<T[Len]>
    {
	typedef const T* type;
	typedef const T& ref;
    };

    template<typename T>
    struct RevRangeIter
    {
	typedef typename T::reverse_iterator type;
	typedef typename IterTraits<typename T::reverse_iterator>::reference ref;
    };

    template<typename T>
    struct ConstRevRangeIter
    {
	typedef typename T::const_reverse_iterator type;
	typedef typename IterTraits<typename T::const_reverse_iterator>::reference ref;
    };

    /* Specialised Iter/IterRev to pick correct iterators based on constness. */
    template<typename T, typename C> struct Iter;

    template<typename T>
    struct Iter<T, TrueType> : public ConstRangeIter<T> {};

    template<typename T>
    struct Iter<T, FalseType> : public RangeIter<T> {};

    template<typename T, typename C> struct IterRev;

    template<typename T>
    struct IterRev<T, TrueType> : public ConstRevRangeIter<T> {};
 
    template<typename T>
    struct IterRev<T, FalseType> : public RevRangeIter<T> {};

    /* Detect constness of container or array. */
    template<typename T>
    struct IsConst : public FalseType {};

    template<typename T>
    struct IsConst<T[]> : public FalseType {};

    template<typename T, unsigned int N>
    struct IsConst<T[N]> : public FalseType {};

    template<typename T>
    struct IsConst<const T> : public TrueType {};

    template<typename T>
    struct IsConst<const T[]> : public TrueType {};

    template<typename T, unsigned int N>
    struct IsConst<const T[N]> : public TrueType {};

    /* Function for const detection (required for template type deduction). */
    template<typename T>
    inline IsConst<T>* IsConstFunc(T&) { return 0; }

    /* Structure used to pass type + constness. */
    template<typename T, typename C>
    struct Type { };

    template<typename T>
    inline Type<T, FalseType>* GetType(T&, FalseType*) { return 0; }
    
    template<typename T>
    inline Type<T, TrueType>* GetType(const T&, TrueType*) { return 0; }

    /* Overloads to get begin/end iterator/pointer wrapped as Any. */
    template<typename T>
    inline Any<typename T::iterator> begin(T& t) { return t.begin(); }

    template<typename T, unsigned int Len>
    inline Any<T*> begin(T (&t)[Len]) { return t; }

    template<typename T>
    inline Any<typename T::iterator> end(T& t) { return t.end(); }

    template<typename T, unsigned int Len>
    inline Any<T*> end(T (&t)[Len]) { return t + Len; }

    template<typename T>
    inline Any<typename T::const_iterator> begin(const T& t) { return t.begin(); }

    template<typename T, unsigned int Len>
    inline Any<const T*> begin(const T (&t)[Len]) { return t; }

    template<typename T>
    inline Any<typename T::const_iterator> end(const T& t) { return t.end(); }

    template<typename T, unsigned int Len>
    inline Any<const T*> end(const T (&t)[Len]) { return t + Len; }

    template<typename T>
    inline Any<typename T::reverse_iterator> rbegin(T& t) { return t.rbegin(); }

    template<typename T>
    inline Any<typename T::reverse_iterator> rend(T& t) { return t.rend(); }

    template<typename T>
    inline Any<typename T::const_reverse_iterator> rbegin(const T& t) { return t.rbegin(); }

    template<typename T>
    inline Any<typename T::const_reverse_iterator> rend(const T& t) { return t.rend(); }

    /* Functions to test if wrapped iterator has reached end of container/array. */
    template<typename T, typename C>
    inline bool done(const AnyBase& it, const AnyBase& end, Type<T, C>*)
    {
	typedef typename Iter<T, C>::type IterType;
	return static_cast<const Any<IterType>&>(it).m_t != static_cast<const Any<IterType>&>(end).m_t;
    }

    template<typename T, typename C>
    inline bool rdone(const AnyBase& it, const AnyBase& end, Type<T, C>*)
    {
	typedef typename IterRev<T, C>::type IterType;
	return static_cast<const Any<IterType>&>(it).m_t != static_cast<const Any<IterType>&>(end).m_t;
    }

    /* Functions to increment wrapped iterator. */
    template<typename T, typename C>
    inline void next(const AnyBase& it, Type<T, C>*)
    {
	typedef typename Iter<T, C>::type IterType;
	++(static_cast<const Any<IterType>&>(it).m_t);
    }

    template<typename T, typename C>
    inline void rnext(const AnyBase& it, Type<T, C>*)
    {
	typedef typename IterRev<T, C>::type IterType;
	++(static_cast<const Any<IterType>&>(it).m_t);
    }

    /* Functions to deference wrapped iterator. */
    template<typename T, typename C>
    inline typename Iter<T, C>::ref deref(const AnyBase& it, Type<T, C>*)
    {
	typedef typename Iter<T, C>::type IterType;
	return *(static_cast<const Any<IterType>&>(it).m_t);
    }

    template<typename T, typename C>
    inline typename IterRev<T, C>::ref rderef(const AnyBase& it, Type<T, C>*)
    {
	typedef typename IterRev<T, C>::type IterType;
	return *(static_cast<const Any<IterType>&>(it).m_t);
    }
}

/* Macros so that each variable named used can be given a unique name based on the line number. */
#define SPX_FOREACH_CAT2(a, b) a ## b
#define SPX_FOREACH_CAT(a, b) SPX_FOREACH_CAT2(a, b)
#define SPX_FOREACH_NAME(name) SPX_FOREACH_CAT(name, __LINE__)

/* Utility macro to get a null object with the type + constness of the container/array. */
#define SPX_FOREACH_GET_TYPE(cont)							\
    (true ? 0 : SPxForeachNS::GetType(cont, SPxForeachNS::IsConstFunc(cont)))

/* Forward foreach (works with STL containers and arrays). */
#define SPX_FOREACH(var, cont)												\
    if(const SPxForeachNS::AnyBase& SPX_FOREACH_NAME(__it) = SPxForeachNS::begin(cont)) {} else				\
    if(const SPxForeachNS::AnyBase& SPX_FOREACH_NAME(__end) = SPxForeachNS::end(cont)) {} else				\
    for(bool SPX_FOREACH_NAME(__continue) = true;									\
	(SPX_FOREACH_NAME(__continue) ? (SPX_FOREACH_NAME(__continue) = false,						\
	    SPxForeachNS::done(SPX_FOREACH_NAME(__it), SPX_FOREACH_NAME(__end), SPX_FOREACH_GET_TYPE(cont))) : 0);	\
	SPxForeachNS::next(SPX_FOREACH_NAME(__it), SPX_FOREACH_GET_TYPE(cont)))						\
	for(var = SPxForeachNS::deref(SPX_FOREACH_NAME(__it), SPX_FOREACH_GET_TYPE(cont));				\
	    !SPX_FOREACH_NAME(__continue); SPX_FOREACH_NAME(__continue) = true)

/* Reverse foreach (works with STL containers ONLY). */
#define SPX_FOREACH_REV(var, cont)											\
    if(const SPxForeachNS::AnyBase& SPX_FOREACH_NAME(__it) = SPxForeachNS::rbegin(cont)) {} else			\
    if(const SPxForeachNS::AnyBase& SPX_FOREACH_NAME(__end) = SPxForeachNS::rend(cont)) {} else				\
    for(bool SPX_FOREACH_NAME(__continue) = true;									\
	(SPX_FOREACH_NAME(__continue) ? (SPX_FOREACH_NAME(__continue) = false,						\
	    SPxForeachNS::rdone(SPX_FOREACH_NAME(__it), SPX_FOREACH_NAME(__end), SPX_FOREACH_GET_TYPE(cont))) : 0);	\
	SPxForeachNS::rnext(SPX_FOREACH_NAME(__it), SPX_FOREACH_GET_TYPE(cont)))					\
	for(var = SPxForeachNS::rderef(SPX_FOREACH_NAME(__it), SPX_FOREACH_GET_TYPE(cont));				\
	    !SPX_FOREACH_NAME(__continue); SPX_FOREACH_NAME(__continue) = true)

#endif /* SPX_NATIVE_FOREACH */

#endif /* _SPX_FOREACH_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
