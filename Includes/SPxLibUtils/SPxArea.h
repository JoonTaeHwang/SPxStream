/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxArea.h,v $
* ID: $Id: SPxArea.h,v 1.15 2017/02/21 15:20:31 rew Exp $
*
* Purpose:
*   Generic SPxArea class (combination of SPxPos and SPxSize).
*
* Revision Control:
*   21/02/17 v1.15   AGC	Fix gcc warnings.
*
* Previous Changes:
*   07/11/16 1.14   AGC	Improve union operator with empty areas.
*   11/05/15 1.13   AGC	Add non-explicit default constructor.
*   07/01/15 1.12   AGC	Remove implicit conversion to string.
*   07/11/14 1.11   AGC	Add empty() function.
*   24/09/14 1.10   AGC	Suppress spurious cppcheck warnings.
*   15/09/14 1.9    AGC	Add X/Y/Width/Height member references.
*   01/09/14 1.8    AGC	Add shift() function taking SPxPos.
*   27/08/14 1.7    AGC	Add tl/tr/bl/br/shift functions.
*   07/07/14 1.6    AGC	Fix cppcheck warnings.
*   04/07/14 1.5    AGC	Move some code to source file.
*   26/06/14 1.4    AGC	Add constructor taking two SPxPos objects.
*			Add overloads for setCentre() and setSize().
*   20/06/14 1.3    AGC	Fix setSize().
*   26/03/14 1.2    AGC	Add const char* operators.
*   22/11/13 1.1    AGC	Initial version - moved from AV library.
**********************************************************************/

#ifndef _SPX_AREA_H
#define _SPX_AREA_H

#include <iosfwd>
#include "SPxLibUtils/SPxPos.h"
#include "SPxLibUtils/SPxSize.h"
#include "SPxLibUtils/SPxTypes.h"

template <typename T, typename T2=T>
class SPxArea
{
public:
    typedef T value_type;
    typedef SPxArea<T, T2> self_type;

    SPxArea(void) : Pos(), Size(),
	X(Pos.X), Y(Pos.Y), Width(Size.Width), Height(Size.Height),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    explicit SPxArea(T x, T y=T(),
	    T width=T(), T height=T())
	: Pos(x, y), Size(width, height),
	X(Pos.X), Y(Pos.Y), Width(Size.Width), Height(Size.Height),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    SPxArea(const self_type& other)
	: Pos(other.Pos), Size(other.Size),
	X(Pos.X), Y(Pos.Y), Width(Size.Width), Height(Size.Height),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    template<typename U, typename U2>
    explicit SPxArea(const SPxArea<U, U2>& other)
	: Pos(other.Pos), Size(other.Size),
	X(Pos.X), Y(Pos.Y), Width(Size.Width), Height(Size.Height),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    template<typename U, typename V, typename V2>
    explicit SPxArea(const SPxPos<U>& pos,
	    const SPxSize<V, V2>& size=(SPxSize<V, V2>()))
	: Pos(pos), Size(size),
	X(Pos.X), Y(Pos.Y), Width(Size.Width), Height(Size.Height),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    template<typename U, typename V>
    explicit SPxArea(const SPxPos<U>& pos1,
	    const SPxPos<V>& pos2)
	    : Pos(pos1), Size((pos2 - pos1).X, (pos2 - pos1).Y),
	    X(Pos.X), Y(Pos.Y), Width(Size.Width), Height(Size.Height),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
    }
    explicit SPxArea(const char *text)
	: Pos(), Size(), X(Pos.X), Y(Pos.Y), Width(Size.Width), Height(Size.Height),
	W(Width), H(Height)
    {
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
	str(text);
    }
    // cppcheck-suppress operatorEqVarError
    self_type& operator=(const self_type &other)
    {
	Pos = other.Pos;
	Size = other.Size;
	for(unsigned int i = 0; i < SPxArrayLen(m_cache); ++i)
	{
	    m_cache[i] = '\0';
	}
	return *this;
    }
    // cppcheck-suppress operatorEqVarError
    template<typename U, typename U2>
    self_type& operator=(const SPxArea<U, U2>& rhs)
    {
	Pos = rhs.Pos;
	Size = rhs.Size;
	m_cache[0] = '\0';
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
    self_type& operator&=(const SPxArea<U, U2>& rhs)
    {
	T r = MIN(right(), static_cast<T>(rhs.right()));
	T b = MIN(bottom(), static_cast<T>(rhs.bottom()));
	Pos.X = MAX(Pos.X, static_cast<T>(rhs.Pos.X));
	Pos.Y = MAX(Pos.Y, static_cast<T>(rhs.Pos.Y));
	Size.Width = ( Pos.X < r ) ? r - Pos.X : 0;
	Size.Height = ( Pos.Y < b ) ? b - Pos.Y : 0;
	return *this;
    }
    template<typename U, typename U2>
    self_type& operator|=(const SPxArea<U, U2>& rhs)
    {
	if(rhs.empty())
	{
	    /* Do nothing. */
	}
	else if(empty())
	{
	    /* Replace this with supplied area. */
	    *this = rhs;
	}
	else
	{
	    T minX = MIN(Pos.X, static_cast<T>(rhs.Pos.X));
	    T minY = MIN(Pos.Y, static_cast<T>(rhs.Pos.Y));
	    Size.Width = MAX(right(), static_cast<T>(rhs.right())) - minX;
	    Size.Height = MAX(bottom(), static_cast<T>(rhs.bottom())) - minY;
	    Pos.X = minX;
	    Pos.Y = minY;
	}
	return *this;
    }
    template<typename U, typename U2>
    self_type& operator*=(const SPxSize<U, U2>& rhs)
    {
	Pos.X *= rhs.Width;
	Pos.Y *= rhs.Height;
	Size.Width *= rhs.Width;
	Size.Height *= rhs.Height;
	return *this;
    }
    template<typename U, typename U2>
    self_type& operator/=(const SPxSize<U, U2>& rhs)
    {
	Pos.X /= rhs.Width;
	Pos.Y /= rhs.Height;
	Size.Width /= rhs.Width;
	Size.Height /= rhs.Height;
	return *this;
    }
    template<typename T2b>
    bool operator==(const SPxArea<T, T2b>& rhs) const
    {
	return (Pos == rhs.Pos) && (Size == rhs.Size);
    }
    template<typename T2b>
    bool operator!=(const SPxArea<T, T2b>& rhs) const
    {
	return !operator==(rhs);
    }
    T2 area(void) const { return Size.area(); }
    bool empty(void) const { return Size.empty(); }
    T left(void) const { return Pos.X; }
    T right(void) const { return Pos.X + Size.Width; }
    T top(void) const { return Pos.Y; }
    T bottom(void) const { return Pos.Y + Size.Height; }
    SPxPos<T> tl(void) const { return SPxPos<T>(left(), top()); }
    SPxPos<T> tr(void) const { return SPxPos<T>(right(), top()); }
    SPxPos<T> bl(void) const { return SPxPos<T>(left(), bottom()); }
    SPxPos<T> br(void) const { return SPxPos<T>(right(), bottom()); }
    SPxPos<T> centre(void) const { return Pos + SPxPos<T>(Size.Width, Size.Height) / 2; }
    self_type& setCentre(const SPxPos<T>& c)
    {
	Pos = c - SPxPos<T>(Size.Width, Size.Height) / 2;
	return *this;
    }
    self_type& setCentre(T x, T y)
    {
	return setCentre(SPxPos<T>(x, y));
    }
    template<typename T2b>
    self_type& setSize(const SPxSize<T, T2b>& newSize)
    {
	Pos += SPxPos<T>(Size.Width, Size.Height) / 2;
	Pos -= SPxPos<T>(newSize.Width, newSize.Height) / 2;
	Size = newSize;
	return *this;
    }
    self_type& setSize(T width, T height)
    {
	return setSize(SPxSize<T, T2>(width, height));
    }
    self_type& shift(const SPxPos<T>& pos)
    {
	Pos += pos;
	return *this;
    }
    self_type& shift(T x, T y)
    {
	Pos += SPxPos<T>(x, y);
	return *this;
    }
    template<typename U>
    bool within(const SPxPos<U>& p) const
    {
	return ((p.X > left()) && (p.Y > top()) && (p.X < right()) && (p.Y < bottom()));
    }
    self_type& inflate(T width, T height)
    {
	Pos.X -= width;
	Pos.Y -= height;
	Size.Width += width * 2;
	Size.Height += height * 2;
	return *this;
    }
    self_type& inflate(T size) { return inflate(size, size); }
    template<typename T2b>
    self_type& inflate(const SPxSize<T, T2b>& size)
    { return inflate(size.Width, size.Height); }
    self_type& deflate(T width, T height)
    {
	Pos.X += width;
	Pos.Y += height;
	Size.Width -= width * 2;
	Size.Height -= height * 2;
	return *this;
    }
    self_type& deflate(T size) { return deflate(size, size); }
    template<typename T2b>
    self_type& deflate(const SPxSize<T, T2b>& size)
    { return deflate(size.Width, size.Height); }

    SPxPos<T> Pos;
    SPxSize<T, T2> Size;
    T& X;
    T& Y;
    T& Width;
    T& Height;
    T& W;
    T& H;

private:
    mutable char m_cache[64];
};
template <typename T, typename T2, typename U, typename U2>
inline SPxArea<T, T2> operator&(SPxArea<T, T2> lhs, const SPxArea<U, U2>& rhs)
{
    lhs &= rhs;
    return lhs;
}
template <typename T, typename T2, typename U, typename U2>
inline SPxArea<T, T2> operator|(SPxArea<T, T2> lhs, const SPxArea<U, U2>& rhs)
{
    lhs |= rhs;
    return lhs;
}
template <typename T, typename T2, typename U, typename U2>
inline SPxArea<T, T2> operator*(SPxArea<T, T2> lhs, const SPxSize<U, U2>& rhs)
{
    lhs *= rhs;
    return lhs;
}
template <typename T, typename T2, typename U, typename U2>
inline SPxArea<T, T2> operator/(SPxArea<T, T2> lhs, const SPxSize<U, U2>& rhs)
{
    lhs /= rhs;
    return lhs;
}
template <typename T, typename T2>
std::ostream& operator<<(std::ostream& os, const SPxArea<T, T2>& obj);
template <typename T, typename T2>
std::istream& operator>>(std::istream& is, SPxArea<T, T2>& obj);

/* Common implementations of SPxArea. */
typedef SPxArea<unsigned int> SPxAreaU;
typedef SPxArea<UINT16, unsigned int> SPxAreaU16;
typedef SPxArea<int> SPxAreaI;
typedef SPxArea<float> SPxAreaF;
typedef SPxArea<double> SPxAreaD;

#endif /* _SPX_AREA_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
