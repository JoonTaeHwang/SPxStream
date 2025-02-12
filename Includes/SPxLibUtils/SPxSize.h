/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSize.h,v $
* ID: $Id: SPxSize.h,v 1.11 2017/02/21 15:20:31 rew Exp $
*
* Purpose:
*   Generic SPxSize class.
*
* Revision Control:
*   21/02/17 v1.11   AGC	Fix gcc warnings.
*
* Previous Changes:
*   11/05/15 1.10   AGC	Add non-explicit default constructor.
*   07/01/15 1.9    AGC	Remove implicit conversion to string.
*   07/11/14 1.8    AGC	Add empty() function.
*   24/09/14 1.7    AGC	Suppress spurious cppcheck warnings.
*   15/09/14 1.6    AGC	Add W/H convenience references.
*   09/09/14 1.5    AGC	Add unary minus operator.
*   07/07/14 1.4    AGC	Fix cppcheck warnings.
*   04/07/14 1.3    AGC	Move some code to source file.
*   26/03/14 1.2    AGC	Add const char* operators.
*   22/11/13 1.1    AGC	Initial version - moved from AV library.
**********************************************************************/

#ifndef _SPX_SIZE_H
#define _SPX_SIZE_H

#include <iosfwd> /* For std::istream/ostream. */
#include "SPxLibUtils/SPxTypes.h" /* For UINT16. */

template <typename T, typename T2=T>
class SPxSize
{
public:
    typedef T value_type;
    typedef SPxSize<T, T2> self_type;

    SPxSize(void) : Width(), Height(), W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    explicit SPxSize(T width, T height=T())
	: Width(width), Height(height),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    SPxSize(const self_type& other)
	: Width(static_cast<T>(other.Width)), Height(static_cast<T>(other.Height)),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    template<typename U, typename U2>
    explicit SPxSize(const SPxSize<U, U2>& other)
	: Width(static_cast<T>(other.Width)), Height(static_cast<T>(other.Height)),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    explicit SPxSize(const char *text)
	: Width(T()), Height(T()),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
	str(text);
    }
    // cppcheck-suppress operatorEqVarError
    self_type& operator=(const self_type& rhs)
    {
	Width = rhs.Width;
	Height = rhs.Height;
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
	return *this;
    }
    // cppcheck-suppress operatorEqVarError
    template<typename U, typename U2>
    self_type& operator=(const SPxSize<U, U2>& rhs)
    {
	Width = static_cast<T>(rhs.Width);
	Height = static_cast<T>(rhs.Height);
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
	return *this;
    }
    // cppcheck-suppress operatorEqVarError
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
    template<typename U, typename U2>
    self_type& operator-=(const SPxSize<U, U2>& rhs)
    {
	Width -= static_cast<T>(rhs.Width);
	Height -= static_cast<T>(rhs.Height);
	return *this;
    }
    template<typename U, typename U2>
    self_type& operator+=(const SPxSize<U, U2>& rhs)
    {
	Width += static_cast<T>(rhs.Width);
	Height += static_cast<T>(rhs.Height);
	return *this;
    }
    template <typename U>
    self_type& operator/=(U arg)
    {
	Width = static_cast<T>(Width / arg);
	Height = static_cast<T>(Height / arg);
	return *this;
    }
    template <typename U>
    self_type& operator*=(U arg)
    {
	Width = static_cast<T>(Width * arg);
	Height = static_cast<T>(Height * arg);
	return *this;
    }
    template<typename T2b>
    bool operator==(const SPxSize<T, T2b>& rhs) const
    {
	return (Width == rhs.Width) && (Height == rhs.Height);
    }
    template<typename T2b>
    bool operator!=(const SPxSize<T, T2b>& rhs) const
    {
	return !operator==(rhs);
    }
    T2 area(void) const { return Width * Height; }
    bool empty(void) const { return area() == static_cast<T2>(0); }

    T Width;
    T Height;
    T& W;
    T& H;

private:
    mutable char m_cache[32];
};

template<typename T, typename T2>
inline SPxSize<T, T2> operator-(SPxSize<T, T2>& lhs)
{
    return SPxSize<T, T2>(-lhs.Width, -lhs.Height);
}
template<typename T, typename T2, typename U, typename U2>
inline SPxSize<T, T2> operator-(SPxSize<T, T2> lhs, const SPxSize<U, U2>& rhs)
{
    lhs -= rhs;
    return lhs;
}
template<typename T, typename T2, typename U, typename U2>
inline SPxSize<T, T2> operator+(SPxSize<T, T2> lhs, const SPxSize<U, U2>& rhs)
{
    lhs += rhs;
    return lhs;
}
template<typename T, typename T2, typename U>
inline SPxSize<T, T2> operator/(SPxSize<T, T2> lhs, U arg)
{
    lhs /= arg;
    return lhs;
}
template<typename T, typename T2, typename U>
inline SPxSize<T, T2> operator*(SPxSize<T, T2> lhs, U arg)
{
    lhs *= arg;
    return lhs;
}
template<typename T, typename T2>
std::ostream& operator<<(std::ostream& os, const SPxSize<T, T2>& obj);
template<typename T, typename T2>
std::istream& operator>>(std::istream& is, SPxSize<T, T2>& obj);

/* Common implementations of SPxSize. */
typedef SPxSize<unsigned int> SPxSizeU;
typedef SPxSize<UINT16, unsigned int> SPxSizeU16;
typedef SPxSize<int> SPxSizeI;
typedef SPxSize<float> SPxSizeF;
typedef SPxSize<double> SPxSizeD;

#endif /* _SPX_SIZE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
