/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPos.h,v $
* ID: $Id: SPxPos.h,v 1.11 2017/02/21 15:20:31 rew Exp $
*
* Purpose:
*   Generic SPxPos class.
*
* Revision Control:
*   21/02/17 v1.11   AGC	Fix gcc warnings.
*
* Previous Changes:
*   01/10/15 1.10   AGC	Add operators for adding single value.
*   11/05/15 1.9    AGC	Add non-explicit default constructor.
*   07/01/15 1.8    AGC	Remove implicit conversion to string.
*   22/10/14 1.7    AGC	Arg type for unary minus should be const.
*   09/09/14 1.6    AGC	Fix unary minus operator.
*   07/07/14 1.5    AGC	Fix cppcheck warnings.
*   04/07/14 1.4    AGC	Move some code to source file.
*   26/03/14 1.3    AGC	Add const char* operators.
*   15/01/14 1.2    AGC	Add multiplication of T by SPxPos.
*			Add unary minus operator.
*   22/11/13 1.1    AGC	Initial version - moved from AV library.
**********************************************************************/

#ifndef _SPX_POS_H
#define _SPX_POS_H

#include <iosfwd> /* For std::istream/ostream. */
#include "SPxLibUtils/SPxTypes.h" /* For UINT16. */

/* Forward declarations. */
template <typename T, typename T2> class SPxSize;

template <typename T>
class SPxPos
{
public:
    typedef T value_type;
    typedef SPxPos<T> self_type;

    SPxPos(void)
	: X(), Y()
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    explicit SPxPos(T x, T y=T())
	: X(x), Y(y)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    template<typename U>
    explicit SPxPos(const SPxPos<U>& other)
	: X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y))
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    template<typename U, typename U2>
    explicit SPxPos(const SPxSize<U, U2>& size)
	: X(static_cast<T>(size.Width)), Y(static_cast<T>(size.Height))
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    explicit SPxPos(const char *text)
	    : X(T()), Y(T())
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
	str(text);
    }
    template<typename U>
    self_type& operator=(const SPxPos<U>& rhs)
    {
	X = static_cast<T>(rhs.X);
	Y = static_cast<T>(rhs.Y);
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
	return *this;
    }
    self_type& operator=(const char *text)
    {
	str(text);
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
	return *this;
    }
    const char *str(void) const;
    void str(const char *text);
    template<typename U>
    self_type& operator-=(const SPxPos<U>& rhs)
    {
	X -= static_cast<T>(rhs.X);
	Y -= static_cast<T>(rhs.Y);
	return *this;
    }
    template<typename U>
    self_type& operator+=(const SPxPos<U>& rhs)
    {
	X += static_cast<T>(rhs.X);
	Y += static_cast<T>(rhs.Y);
	return *this;
    }
    self_type& operator-=(const T& val)
    {
	X -= val;
	Y -= val;
	return *this;
    }
    self_type& operator+=(const T& val)
    {
	X += val;
	Y += val;
	return *this;
    }
    template<typename U, typename U2>
    self_type& operator*=(const SPxSize<U, U2>& rhs)
    {
	X *= rhs.Width;
	Y *= rhs.Height;
	return *this;
    }
    template<typename U, typename U2>
    self_type& operator/=(const SPxSize<U, U2>& rhs)
    {
	X /= rhs.Width;
	Y /= rhs.Height;
	return *this;
    }
    template<typename U>
    self_type& operator*=(U div)
    {
	X = static_cast<T>(X * div);
	Y = static_cast<T>(Y * div);
	return *this;
    }
    template<typename U>
    self_type& operator/=(U div)
    {
	X = static_cast<T>(X / div);
	Y = static_cast<T>(Y / div);
	return *this;
    }
    bool operator==(const self_type& rhs) const
    {
	return (X == rhs.X) && (Y == rhs.Y);
    }
    bool operator!=(const self_type& rhs) const
    {
	return !operator==(rhs);
    }
    double len(void) const;

    T X;
    T Y;

private:
    mutable char m_cache[32];
};

template<typename T>
inline SPxPos<T> operator-(const SPxPos<T>& lhs)
{
    return SPxPos<T>(-lhs.X, -lhs.Y);
}

template<typename T, typename U>
inline SPxPos<T> operator-(SPxPos<T> lhs, const SPxPos<U>& rhs)
{
    lhs -= rhs;
    return lhs;
}
template<typename T, typename U>
inline SPxPos<T> operator+(SPxPos<T> lhs, const SPxPos<U>& rhs)
{
    lhs += rhs;
    return lhs;
}
template<typename T>
inline SPxPos<T> operator-(SPxPos<T> lhs, const T& val)
{
    lhs -= val;
    return lhs;
}
template<typename T>
inline SPxPos<T> operator+(SPxPos<T> lhs, const T& val)
{
    lhs += val;
    return lhs;
}
template<typename T>
inline SPxPos<T> operator*(T lhs, SPxPos<T> rhs)
{
    rhs *= lhs;
    return rhs;
}
template<typename T, typename U, typename U2>
inline SPxPos<T> operator*(SPxPos<T> lhs, const SPxSize<U, U2>& rhs)
{
    lhs *= rhs;
    return lhs;
}
template<typename T, typename U, typename U2>
inline SPxPos<T> operator/(SPxPos<T> lhs, const SPxSize<U, U2>& rhs)
{
    lhs /= rhs;
    return lhs;
}
template<typename T, typename U>
inline SPxPos<T> operator*(SPxPos<T> lhs, U arg)
{
    lhs *= arg;
    return lhs;
}
template<typename T, typename U>
inline SPxPos<T> operator/(SPxPos<T> lhs, U arg)
{
    lhs /= arg;
    return lhs;
}
template<typename T>
std::ostream& operator<<(std::ostream& os, const SPxPos<T>& obj);
template<typename T>
std::istream& operator>>(std::istream& is, SPxPos<T>& obj);

/* Typedefs of SPxPos. */
typedef SPxPos<unsigned int> SPxPosU;
typedef SPxPos<UINT16> SPxPosU16;
typedef SPxPos<int> SPxPosI;
typedef SPxPos<float> SPxPosF;
typedef SPxPos<double> SPxPosD;

#endif /* _SPX_POS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
