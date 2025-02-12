/*********************************************************************
*
* (c) Copyright 2014 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxTuple.h,v $
* ID: $Id: SPxTuple.h,v 1.8 2017/05/09 13:23:46 rew Exp $
*
* Purpose:
*   Header for SPxTuple class that acts just like std::pair, but for
*   up to five elements.
*
*   The elements are not directly accessible, but are instead accessed
*   using the SPxGet() function:
*
*	SPxTuple<int, float> t(4, 2.5F);
*	int el1 = SPxGet<0>(t);
*	float el2 = SPxGet<1>(t);
*
*   The SPxMakeTuple() functions construct a tuple using template
*   type deduction so the types do not need repeating when calling
*   a function that takes an SPxTuple argument:
*
*	void func(const SPxTuple<int, int>& t);
*
*	int i = 4;
*	func(SPxMakeTuple(i, 2));
*
*   A function returning an SPxTuple can apply the elements directly
*   to existing variables using the SPxTie function:
*
*	SPxTuple<int, float> func(void) { return SPxMakeTuple(3, 4.5); }
*
*	int el1 = 0;
*	float el2 = 0.0F;
*	SPxTie(el1, el2) = func(); // el1 now equals 3, el2 now equals 4.5
*
*   Where not all the elements of the SPxTuple are required, the
*   SPxTupleIgnore item may be used to ignore certain elements:
*
*	SPxTuple<int, float> func(void) { return SPxMakeTuple(3, 4.5); }
*
*	float el2 = 0.0F;
*	SPxTie(SPxTupleIgnore, el2) = func(); // el2 now equals 4.5
*
*   The SPxTupleElement structure allows the type of each element of
*   a tuple to be retrieved at compile time:
*
*	SPxTuple<std::vector<int>, float> t;
*
*	// var has type std::vector<int>.
*	SPxTupleElement<0, t>::type var;
*
* Revision Control:
*   06/09/22 v1.9    AGC	Suppress spurious cppcheck ODR warnings.
*
* Previous Changes:
*   09/05/17 1.8    AGC	Fix cppcheck warnings.
*   07/06/16 1.7    AGC	Add operator less than.
*   04/09/15 1.6    AGC	Add non-type changing assignment operator for Leaf class.
*   26/03/15 1.5    AGC	Add SPxTupleSize.
*   26/01/15 1.4    AGC	Fix issues with tuples containing references.
*   07/11/14 1.3    AGC	Fix another icc warning.
*   07/11/14 1.2    AGC	Fix icc warnings.
*   07/11/14 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_TUPLE_H
#define _SPX_TUPLE_H

/* Other headers required. */
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxRefWrapper.h"

namespace SPxTupleNS
{
    template<unsigned int I, typename T>
    struct Leaf
    {
	typedef T type;

	Leaf() : val() {}
	Leaf(typename SPxAddCrefIfNotRef<T>::type val_) : val(val_) {}

	template<typename U>
	Leaf(const Leaf<I, U>& other) : val(other.val) {}

	Leaf(const Leaf<I, T>& other) : val(other.val) {}

	template<typename U>
	Leaf<I, T>& operator=(const Leaf<I, U>& other)
	{
	    val = other.val;
	    return *this;
	}

	Leaf<I, T>& operator=(const Leaf<I, T>& other)
	{
	    val = other.val;
	    return *this;
	}

	T val;
    };

    template<unsigned int I, typename T0, typename T1=void, typename T2=void, typename T3=void, typename T4=void>
    struct Element;

    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    struct Element<0, T0, T1, T2, T3, T4>
    {
	typedef T0 type;
    };
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    struct Element<1, T0, T1, T2, T3, T4>
    {
	typedef T1 type;
    };
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    struct Element<2, T0, T1, T2, T3, T4>
    {
	typedef T2 type;
    };
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    struct Element<3, T0, T1, T2, T3, T4>
    {
	typedef T3 type;
    };
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    struct Element<4, T0, T1, T2, T3, T4>
    {
	typedef T4 type;
    };

    template<typename T>
    struct Decay
    {
	typedef typename SPxRemoveRef<typename SPxRemoveCV<T>::type>::type type;
    };

    template<typename T>
    struct Decay<SPxRefWrapper<T> >
    {
	typedef T& type;
    };

    struct Ignore
    {
	Ignore() {}

	template<typename T>
	void operator=(const T&) const {}
    };
}

template<typename T0, typename T1=void, typename T2=void, typename T3=void, typename T4=void>
class SPxTuple;

template<unsigned int I, typename T0>
inline typename SPxAddRef<T0>::type SPxGet(SPxTuple<T0> &t)
{
    return static_cast<typename SPxAddRef<typename SPxTupleNS::Leaf<I, T0> >::type>(t).val;
}
template<unsigned int I, typename T0>
inline typename SPxAddCref<T0>::type SPxGet(const SPxTuple<T0> &t)
{
    return static_cast<typename SPxAddCref<typename SPxTupleNS::Leaf<I, T0> >::type>(t).val;
}
template<unsigned int I, typename T0, typename T1>
inline typename SPxAddRef<typename SPxTupleNS::Element<I, T0, T1>::type>::type SPxGet(SPxTuple<T0, T1> &t)
{
    return static_cast<typename SPxAddRef<typename SPxTupleNS::Leaf<I, typename SPxTupleNS::Element<I, T0, T1>::type> >::type>(t).val;
}
template<unsigned int I, typename T0, typename T1>
inline typename SPxAddCref<typename SPxTupleNS::Element<I, T0, T1>::type>::type SPxGet(const SPxTuple<T0, T1> &t)
{
    return static_cast<typename SPxAddCref<typename SPxTupleNS::Leaf<I, typename SPxTupleNS::Element<I, T0, T1>::type> >::type>(t).val;
}
template<unsigned int I, typename T0, typename T1, typename T2>
inline typename SPxAddRef<typename SPxTupleNS::Element<I, T0, T1, T2>::type>::type SPxGet(SPxTuple<T0, T1, T2> &t)
{
    return static_cast<typename SPxAddRef<typename SPxTupleNS::Leaf<I, typename SPxTupleNS::Element<I, T0, T1, T2>::type> >::type>(t).val;
}
template<unsigned int I, typename T0, typename T1, typename T2>
inline typename SPxAddCref<typename SPxTupleNS::Element<I, T0, T1, T2>::type>::type SPxGet(const SPxTuple<T0, T1, T2> &t)
{
    return static_cast<typename SPxAddCref<typename SPxTupleNS::Leaf<I, typename SPxTupleNS::Element<I, T0, T1, T2>::type> >::type>(t).val;
}
template<unsigned int I, typename T0, typename T1, typename T2, typename T3>
inline typename SPxAddRef<typename SPxTupleNS::Element<I, T0, T1, T2, T3>::type>::type SPxGet(SPxTuple<T0, T1, T2, T3> &t)
{
    return static_cast<typename SPxAddRef<typename SPxTupleNS::Leaf<I, typename SPxTupleNS::Element<I, T0, T1, T2, T3>::type> >::type>(t).val;
}
template<unsigned int I, typename T0, typename T1, typename T2, typename T3>
inline typename SPxAddCref<typename SPxTupleNS::Element<I, T0, T1, T2, T3>::type>::type SPxGet(const SPxTuple<T0, T1, T2, T3> &t)
{
    return static_cast<typename SPxAddCref<typename SPxTupleNS::Leaf<I, typename SPxTupleNS::Element<I, T0, T1, T2, T3>::type> >::type>(t).val;
}
template<unsigned int I, typename T0, typename T1, typename T2, typename T3, typename T4>
inline typename SPxAddRef<typename SPxTupleNS::Element<I, T0, T1, T2, T3, T4>::type>::type SPxGet(SPxTuple<T0, T1, T2, T3, T4> &t)
{
    return static_cast<typename SPxAddRef<typename SPxTupleNS::Leaf<I, typename SPxTupleNS::Element<I, T0, T1, T2, T3, T4>::type> >::type>(t).val;
}
template<unsigned int I, typename T0, typename T1, typename T2, typename T3, typename T4>
inline typename SPxAddCref<typename SPxTupleNS::Element<I, T0, T1, T2, T3, T4>::type>::type SPxGet(const SPxTuple<T0, T1, T2, T3, T4> &t)
{
    return static_cast<typename SPxAddCref<typename SPxTupleNS::Leaf<I, typename SPxTupleNS::Element<I, T0, T1, T2, T3, T4>::type> >::type>(t).val;
}

template<typename T0>
// cppcheck-suppress ctuOneDefinitionRuleViolation
class SPxTuple<T0, void, void, void, void> : public SPxTupleNS::Leaf<0, T0>
{
public:
    SPxTuple() {}
    explicit SPxTuple(typename SPxAddCrefIfNotRef<T0>::type v0) : SPxTupleNS::Leaf<0, T0>(v0) {}
    template<typename U0>
    SPxTuple(const SPxTuple<U0>& other)
	: SPxTupleNS::Leaf<0, T0>(static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val) {}
    template<typename U0>
    SPxTuple<T0>& operator=(const SPxTuple<U0>& other)
    {
	static_cast<typename SPxTupleNS::Leaf<0, T0>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val;
	return *this;
    }
    static const unsigned int size = 1;
};

template<typename T0>
inline bool operator==(const SPxTuple<T0> &lhs, const SPxTuple<T0> &rhs)
{
    return SPxGet<0>(lhs) == SPxGet<0>(rhs);
}
template<typename T0>
inline bool operator!=(const SPxTuple<T0> &lhs, const SPxTuple<T0> &rhs)
{
    return !operator==(lhs, rhs);
}
template<typename T0>
inline bool operator<(const SPxTuple<T0> &lhs, const SPxTuple<T0> &rhs)
{
    return SPxGet<0>(lhs) < SPxGet<0>(rhs);
}
template<typename T0>
inline SPxTuple<T0> SPxMakeTuple(const T0& v0)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type>(v0);
}
template<typename T0>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type> SPxMakeTuple(T0& v0)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type>(v0);
}

template<typename T0, typename T1>
// cppcheck-suppress ctuOneDefinitionRuleViolation
class SPxTuple<T0, T1, void, void, void>
    : public SPxTupleNS::Leaf<0, T0>, public SPxTupleNS::Leaf<1, T1>
{
public:
    SPxTuple() {}
    explicit SPxTuple(typename SPxAddCrefIfNotRef<T0>::type v0,
		      typename SPxAddCrefIfNotRef<T1>::type v1)
	: SPxTupleNS::Leaf<0, T0>(v0), SPxTupleNS::Leaf<1, T1>(v1) {}
    template<typename U0, typename U1>
    SPxTuple(const SPxTuple<U0, U1>& other)
	: SPxTupleNS::Leaf<0, T0>(static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val),
	  SPxTupleNS::Leaf<1, T1>(static_cast<typename SPxTupleNS::Leaf<1, U1> >(other).val) {}
    template<typename U0, typename U1>
    SPxTuple<T0, T1>& operator=(const SPxTuple<U0, U1>& other)
    {
	static_cast<typename SPxTupleNS::Leaf<0, T0>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<1, T1>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<1, U1> >(other).val;
	return *this;
    }
    static const unsigned int size = 2;
};

template<typename T0, typename T1>
inline bool operator==(const SPxTuple<T0, T1> &lhs, const SPxTuple<T0, T1> &rhs)
{
    return (SPxGet<0>(lhs) == SPxGet<0>(rhs)) && (SPxGet<1>(lhs) == SPxGet<1>(rhs));
}
template<typename T0, typename T1>
inline bool operator!=(const SPxTuple<T0, T1> &lhs, const SPxTuple<T0, T1> &rhs)
{
    return !operator==(lhs, rhs);
}
template<typename T0, typename T1>
inline bool operator<(const SPxTuple<T0, T1> &lhs, const SPxTuple<T0, T1> &rhs)
{
    if(SPxGet<0>(lhs) < SPxGet<0>(rhs)) return true;
    if(SPxGet<0>(rhs) < SPxGet<0>(lhs)) return false;
    if(SPxGet<1>(lhs) < SPxGet<1>(rhs)) return true;
    if(SPxGet<1>(rhs) < SPxGet<1>(lhs)) return false;
    return false;
}
template<typename T0, typename T1>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type> SPxMakeTuple(const T0& v0, const T1& v1)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type>(v0, v1);
}
template<typename T0, typename T1>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type> SPxMakeTuple(T0& v0, T1& v1)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type>(v0, v1);
}

template<typename T0, typename T1, typename T2>
// cppcheck-suppress ctuOneDefinitionRuleViolation
class SPxTuple<T0, T1, T2, void, void>
    : public SPxTupleNS::Leaf<0, T0>, public SPxTupleNS::Leaf<1, T1>,
      public SPxTupleNS::Leaf<2, T2>
{
public:
    SPxTuple() {}
    explicit SPxTuple(typename SPxAddCrefIfNotRef<T0>::type v0, typename SPxAddCrefIfNotRef<T1>::type v1, typename SPxAddCrefIfNotRef<T2>::type v2)
	: SPxTupleNS::Leaf<0, T0>(v0), SPxTupleNS::Leaf<1, T1>(v1),
	  SPxTupleNS::Leaf<2, T2>(v2) {}
    template<typename U0, typename U1, typename U2>
    SPxTuple(const SPxTuple<U0, U1, U2>& other)
	: SPxTupleNS::Leaf<0, T0>(static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val),
	  SPxTupleNS::Leaf<1, T1>(static_cast<typename SPxTupleNS::Leaf<1, U1> >(other).val),
	  SPxTupleNS::Leaf<2, T2>(static_cast<typename SPxTupleNS::Leaf<2, U2> >(other).val) {}
    template<typename U0, typename U1, typename U2>
    SPxTuple<T0, T1, T2>& operator=(const SPxTuple<U0, U1, U2>& other)
    {
	static_cast<typename SPxTupleNS::Leaf<0, T0>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<1, T1>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<1, U1> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<2, T2>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<2, U2> >(other).val;
	return *this;
    }
    static const unsigned int size = 3;
};

template<typename T0, typename T1, typename T2>
inline bool operator==(const SPxTuple<T0, T1, T2> &lhs, const SPxTuple<T0, T1, T2> &rhs)
{
    return (SPxGet<0>(lhs) == SPxGet<0>(rhs)) && (SPxGet<1>(lhs) == SPxGet<1>(rhs))
	&& (SPxGet<2>(lhs) == SPxGet<2>(rhs));
}
template<typename T0, typename T1, typename T2>
inline bool operator!=(const SPxTuple<T0, T1, T2> &lhs, const SPxTuple<T0, T1, T2> &rhs)
{
    return !operator==(lhs, rhs);
}
template<typename T0, typename T1, typename T2>
inline bool operator<(const SPxTuple<T0, T1, T2> &lhs, const SPxTuple<T0, T1, T2> &rhs)
{
    if(SPxGet<0>(lhs) < SPxGet<0>(rhs)) return true;
    if(SPxGet<0>(rhs) < SPxGet<0>(lhs)) return false;
    if(SPxGet<1>(lhs) < SPxGet<1>(rhs)) return true;
    if(SPxGet<1>(rhs) < SPxGet<1>(lhs)) return false;
    if(SPxGet<2>(lhs) < SPxGet<2>(rhs)) return true;
    if(SPxGet<2>(rhs) < SPxGet<2>(lhs)) return false;
    return false;
}
template<typename T0, typename T1, typename T2>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type, typename SPxTupleNS::Decay<T2>::type> SPxMakeTuple(const T0& v0, const T1& v1, const T2& v2)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type, typename SPxTupleNS::Decay<T2>::type>(v0, v1, v2);
}
template<typename T0, typename T1, typename T2>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type, typename SPxTupleNS::Decay<T2>::type> SPxMakeTuple(T0& v0, T1& v1, T2& v2)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type, typename SPxTupleNS::Decay<T2>::type>(v0, v1, v2);
}

template<typename T0, typename T1, typename T2, typename T3>
// cppcheck-suppress ctuOneDefinitionRuleViolation
class SPxTuple<T0, T1, T2, T3, void>
    : public SPxTupleNS::Leaf<0, T0>, public SPxTupleNS::Leaf<1, T1>,
      public SPxTupleNS::Leaf<2, T2>, public SPxTupleNS::Leaf<3, T3>
{
public:
    SPxTuple() {}
    explicit SPxTuple(typename SPxAddCrefIfNotRef<T0>::type v0, typename SPxAddCrefIfNotRef<T1>::type v1,
		      typename SPxAddCrefIfNotRef<T2>::type v2, typename SPxAddCrefIfNotRef<T3>::type v3)
	: SPxTupleNS::Leaf<0, T0>(v0), SPxTupleNS::Leaf<1, T1>(v1),
	  SPxTupleNS::Leaf<2, T2>(v2), SPxTupleNS::Leaf<3, T3>(v3) {}
    template<typename U0, typename U1, typename U2, typename U3>
    SPxTuple(const SPxTuple<U0, U1, U2, U3>& other)
	: SPxTupleNS::Leaf<0, T0>(static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val),
	  SPxTupleNS::Leaf<1, T1>(static_cast<typename SPxTupleNS::Leaf<1, U1> >(other).val),
	  SPxTupleNS::Leaf<2, T2>(static_cast<typename SPxTupleNS::Leaf<2, U2> >(other).val),
	  SPxTupleNS::Leaf<3, T3>(static_cast<typename SPxTupleNS::Leaf<3, U3> >(other).val) {}
    template<typename U0, typename U1, typename U2, typename U3>
    SPxTuple<T0, T1, T2, T3>& operator=(const SPxTuple<U0, U1, U2, U3>& other)
    {
	static_cast<typename SPxTupleNS::Leaf<0, T0>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<1, T1>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<1, U1> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<2, T2>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<2, U2> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<3, T3>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<3, U3> >(other).val;
	return *this;
    }
    static const unsigned int size = 4;
};

template<typename T0, typename T1, typename T2, typename T3>
inline bool operator==(const SPxTuple<T0, T1, T2, T3> &lhs, const SPxTuple<T0, T1, T2, T3> &rhs)
{
    return (SPxGet<0>(lhs) == SPxGet<0>(rhs)) && (SPxGet<1>(lhs) == SPxGet<1>(rhs))
	&& (SPxGet<2>(lhs) == SPxGet<2>(rhs)) && (SPxGet<3>(lhs) == SPxGet<3>(rhs));
}
template<typename T0, typename T1, typename T2, typename T3>
inline bool operator!=(const SPxTuple<T0, T1, T2, T3> &lhs, const SPxTuple<T0, T1, T2, T3> &rhs)
{
    return !operator==(lhs, rhs);
}
template<typename T0, typename T1, typename T2, typename T3>
inline bool operator<(const SPxTuple<T0, T1, T2, T3> &lhs, const SPxTuple<T0, T1, T2, T3> &rhs)
{
    if(SPxGet<0>(lhs) < SPxGet<0>(rhs)) return true;
    if(SPxGet<0>(rhs) < SPxGet<0>(lhs)) return false;
    if(SPxGet<1>(lhs) < SPxGet<1>(rhs)) return true;
    if(SPxGet<1>(rhs) < SPxGet<1>(lhs)) return false;
    if(SPxGet<2>(lhs) < SPxGet<2>(rhs)) return true;
    if(SPxGet<2>(rhs) < SPxGet<2>(lhs)) return false;
    if(SPxGet<3>(lhs) < SPxGet<3>(rhs)) return true;
    if(SPxGet<3>(rhs) < SPxGet<3>(lhs)) return false;
    return false;
}
template<typename T0, typename T1, typename T2, typename T3>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type, typename SPxTupleNS::Decay<T2>::type, typename SPxTupleNS::Decay<T3>::type> SPxMakeTuple(const T0& v0, const T1& v1, const T2& v2, const T3& v3)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type,
	typename SPxTupleNS::Decay<T2>::type, typename SPxTupleNS::Decay<T3>::type>(v0, v1, v2, v3);
}
template<typename T0, typename T1, typename T2, typename T3>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type, typename SPxTupleNS::Decay<T2>::type, typename SPxTupleNS::Decay<T3>::type> SPxMakeTuple(T0& v0, T1& v1, T2& v2, T3& v3)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type,
	typename SPxTupleNS::Decay<T2>::type, typename SPxTupleNS::Decay<T3>::type>(v0, v1, v2, v3);
}

template<typename T0, typename T1, typename T2, typename T3, typename T4>
// cppcheck-suppress ctuOneDefinitionRuleViolation
class SPxTuple
    : public SPxTupleNS::Leaf<0, T0>, public SPxTupleNS::Leaf<1, T1>,
      public SPxTupleNS::Leaf<2, T2>, public SPxTupleNS::Leaf<3, T3>,
      public SPxTupleNS::Leaf<4, T4>
{
public:
    SPxTuple() {}
    explicit SPxTuple(typename SPxAddCrefIfNotRef<T0>::type v0, typename SPxAddCrefIfNotRef<T1>::type v1,
		      typename SPxAddCrefIfNotRef<T2>::type v2, typename SPxAddCrefIfNotRef<T3>::type v3,
		      typename SPxAddCrefIfNotRef<T4>::type v4)
	: SPxTupleNS::Leaf<0, T0>(v0), SPxTupleNS::Leaf<1, T1>(v1),
	  SPxTupleNS::Leaf<2, T2>(v2), SPxTupleNS::Leaf<3, T3>(v3),
	  SPxTupleNS::Leaf<4, T4>(v4) {}
    template<typename U0, typename U1, typename U2, typename U3, typename U4>
    SPxTuple(const SPxTuple<U0, U1, U2, U3, U4>& other)
	: SPxTupleNS::Leaf<0, T0>(static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val),
	  SPxTupleNS::Leaf<1, T1>(static_cast<typename SPxTupleNS::Leaf<1, U1> >(other).val),
	  SPxTupleNS::Leaf<2, T2>(static_cast<typename SPxTupleNS::Leaf<2, U2> >(other).val),
	  SPxTupleNS::Leaf<3, T3>(static_cast<typename SPxTupleNS::Leaf<3, U3> >(other).val),
	  SPxTupleNS::Leaf<4, T4>(static_cast<typename SPxTupleNS::Leaf<4, U4> >(other).val) {}
    template<typename U0, typename U1, typename U2, typename U3, typename U4>
    SPxTuple<T0, T1, T2, T3, T4>& operator=(const SPxTuple<U0, U1, U2, U3, U4>& other)
    {
	static_cast<typename SPxTupleNS::Leaf<0, T0>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<0, U0> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<1, T1>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<1, U1> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<2, T2>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<2, U2> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<3, T3>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<3, U3> >(other).val;
	static_cast<typename SPxTupleNS::Leaf<4, T4>&>(*this).val
	    = static_cast<typename SPxTupleNS::Leaf<4, U4> >(other).val;
	return *this;
    }
    static const unsigned int size = 5;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
inline bool operator==(const SPxTuple<T0, T1, T2, T3, T4> &lhs, const SPxTuple<T0, T1, T2, T3, T4> &rhs)
{
    return (SPxGet<0>(lhs) == SPxGet<0>(rhs)) && (SPxGet<1>(lhs) == SPxGet<1>(rhs))
	&& (SPxGet<2>(lhs) == SPxGet<2>(rhs)) && (SPxGet<3>(lhs) == SPxGet<3>(rhs))
	&& (SPxGet<4>(lhs) == SPxGet<4>(rhs));
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
inline bool operator!=(const SPxTuple<T0, T1, T2, T3, T4> &lhs, const SPxTuple<T0, T1, T2, T3, T4> &rhs)
{
    return !operator==(lhs, rhs);
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
inline bool operator<(const SPxTuple<T0, T1, T2, T3, T4> &lhs, const SPxTuple<T0, T1, T2, T3, T4> &rhs)
{
    if(SPxGet<0>(lhs) < SPxGet<0>(rhs)) return true;
    if(SPxGet<0>(rhs) < SPxGet<0>(lhs)) return false;
    if(SPxGet<1>(lhs) < SPxGet<1>(rhs)) return true;
    if(SPxGet<1>(rhs) < SPxGet<1>(lhs)) return false;
    if(SPxGet<2>(lhs) < SPxGet<2>(rhs)) return true;
    if(SPxGet<2>(rhs) < SPxGet<2>(lhs)) return false;
    if(SPxGet<3>(lhs) < SPxGet<3>(rhs)) return true;
    if(SPxGet<3>(rhs) < SPxGet<3>(lhs)) return false;
    if(SPxGet<4>(lhs) < SPxGet<4>(rhs)) return true;
    if(SPxGet<4>(rhs) < SPxGet<4>(lhs)) return false;
    return false;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type, typename SPxTupleNS::Decay<T2>::type, typename SPxTupleNS::Decay<T3>::type, typename SPxTupleNS::Decay<T4>::type> SPxMakeTuple(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type,
	typename SPxTupleNS::Decay<T2>::type, typename SPxTupleNS::Decay<T3>::type,
	typename SPxTupleNS::Decay<T4>::type>(v0, v1, v2, v3, v4);
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
inline SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type, typename SPxTupleNS::Decay<T2>::type, typename SPxTupleNS::Decay<T3>::type, typename SPxTupleNS::Decay<T4>::type> SPxMakeTuple(T0& v0, T1& v1, T2& v2, T3& v3, T4& v4)
{
    return SPxTuple<typename SPxTupleNS::Decay<T0>::type, typename SPxTupleNS::Decay<T1>::type,
	typename SPxTupleNS::Decay<T2>::type, typename SPxTupleNS::Decay<T3>::type,
	typename SPxTupleNS::Decay<T4>::type>(v0, v1, v2, v3, v4);
}

template<unsigned int I, typename T0, typename T1=void, typename T2=void, typename T3=void, typename T4=void>
struct SPxTupleElement;
template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct SPxTupleElement<0, SPxTuple<T0, T1, T2, T3, T4> >
{
    typedef T0 type;
};
template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct SPxTupleElement<1, SPxTuple<T0, T1, T2, T3, T4> >
{
    typedef T1 type;
};
template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct SPxTupleElement<2, SPxTuple<T0, T1, T2, T3, T4> >
{
    typedef T2 type;
};
template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct SPxTupleElement<3, SPxTuple<T0, T1, T2, T3, T4> >
{
    typedef T3 type;
};
template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct SPxTupleElement<4, SPxTuple<T0, T1, T2, T3, T4> >
{
    typedef T4 type;
};

template<typename T>
struct SPxTupleSize
{
    static const unsigned int value = T::size;
};

const SPxTupleNS::Ignore SPxTupleIgnore = SPxTupleNS::Ignore();

template<typename T0>
inline SPxTuple<T0&> SPxTie(T0& v0)
{
    return SPxMakeTuple(SPxRefWrapper<T0>(v0));
}
template<typename T0, typename T1>
inline SPxTuple<T0&, T1&> SPxTie(T0& v0, T1 &v1)
{
    return SPxMakeTuple(SPxRefWrapper<T0>(v0), SPxRefWrapper<T1>(v1));
}
template<typename T0, typename T1, typename T2>
inline SPxTuple<T0&, T1&, T2&> SPxTie(T0& v0, T1 &v1, T2 &v2)
{
    return SPxMakeTuple(SPxRefWrapper<T0>(v0), SPxRefWrapper<T1>(v1),
			SPxRefWrapper<T2>(v2));
}
template<typename T0, typename T1, typename T2, typename T3>
inline SPxTuple<T0&, T1&, T2&, T3&> SPxTie(T0& v0, T1 &v1, T2 &v2, T3 &v3)
{
    return SPxMakeTuple(SPxRefWrapper<T0>(v0), SPxRefWrapper<T1>(v1),
			SPxRefWrapper<T2>(v2), SPxRefWrapper<T3>(v3));
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
inline SPxTuple<T0&, T1&, T2&, T3&, T4&> SPxTie(T0& v0, T1 &v1, T2 &v2, T3 &v3, T4 &v4)
{
    return SPxMakeTuple(SPxRefWrapper<T0>(v0), SPxRefWrapper<T1>(v1),
			SPxRefWrapper<T2>(v2), SPxRefWrapper<T3>(v3),
			SPxRefWrapper<T4>(v4));
}

#endif /* _SPX_TUPLE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
