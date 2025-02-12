/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Matrix (and vector) class.
*
* Revision Control:
*   25/03/22 v1.13   AGC	Support cross product.
*				Support quaternion multiplication.
*
* Previous Changes:
*   25/09/19 1.12   AGC	Add required include for std::size_t type.
*   20/03/19 1.11   AGC	Fix gcc 9 warning.
*   20/06/18 1.10   AGC	Suppress spurious cppcheck warnings.
*   14/04/15 1.9    AGC	Fix matrix inversion when leading diagonal has zeros.
*			Fix matrix inversion when rows need reordering.
*			Make dot() function const.
*			Add comparison functions.
*   15/01/14 1.8    AGC	Add set() function for SPxVector taking array.
*   18/11/13 1.7    AGC	Fix error in SPxVector::get().
*   08/11/13 1.6    AGC	Fix link errors.
*   08/11/13 1.5    AGC	Include matrix size in type.
*   15/07/13 1.4    AGC	Initialise memory from default constructor.
*   04/03/13 1.3    AGC	Stop using vector.
*   04/04/12 1.2    AGC	Check args in const operator() function.
*   29/03/12 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_MATRIX_H
#define _SPX_MATRIX_H

/*********************************************************************
*
*   Headers 
*
**********************************************************************/

/* For std::size_t. */
#include <cstddef>

/* Need SPX_STATIC_ASSERT. */
#include "SPxLibUtils/SPxTypes.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Define our class. */
template<typename T, std::size_t R, std::size_t C=R>
class SPxMatrix
{
public:
    typedef SPxMatrix<T, R, C> self_type;
    typedef std::size_t size_type;

    /* Constructors. */
    SPxMatrix(void) { setAll(T()); }
    SPxMatrix(const T (&arr)[R * C]) { set(arr); }
    SPxMatrix(const T (&arr)[R][C]) { set(arr); }

    /* Single argument constructor sets leading diagonal. */
    SPxMatrix(T val)
    {
	/* This special constructor initialises the leading diagonal. */
	SPX_STATIC_ASSERT(R == C, Matrix_must_be_square);
	setAll(T());
	setDiagonal(val);
    }

    /* Provide constructors for directly constructing matrices with
     * up to 9 elements.
     */
    SPxMatrix(T v0, T v1)
    {
	T arr[] = { v0, v1 };
	set(arr);
    }
    SPxMatrix(T v0, T v1, T v2)
    {
	T arr[] = { v0, v1, v2 };
	set(arr);
    }
    SPxMatrix(T v0, T v1, T v2, T v3)
    {
	T arr[] = { v0, v1, v2, v3 };
	set(arr);
    }
    SPxMatrix(T v0, T v1, T v2, T v3, T v4)
    {
	T arr[] = { v0, v1, v2, v3, v4 };
	set(arr);
    }
    SPxMatrix(T v0, T v1, T v2, T v3, T v4, T v5)
    {
	T arr[] = { v0, v1, v2, v3, v4, v5 };
	set(arr);
    }
    SPxMatrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6)
    {
	T arr[] = { v0, v1, v2, v3, v4, v5, v6 };
	set(arr);
    }
    SPxMatrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
    {
	T arr[] = { v0, v1, v2, v3, v4, v5, v6, v7};
	set(arr);
    }
    SPxMatrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8)
    {
	T arr[] = { v0, v1, v2, v3, v4, v5, v6, v7, v8 };
	set(arr);
    }

    /* Copy constructor. */
    SPxMatrix(const SPxMatrix<T, R, C> &other)
    {
	*this = other;
    }

    virtual ~SPxMatrix(void) {}

    /* Assignment operator. */
    self_type &operator=(const self_type &other)
    {
	set(other.data());
	return *this;
    }

    /* Functions to retrieve matrix dimensions. */
    size_type rows(void) const { return R; }
    size_type cols(void) const { return C; }
    size_type size(void) const { return R * C; }

    /* Functions to retrieve reference to element in matrix. */
    template<size_type R1, size_type C1>
    T& get(void)
    {
	SPX_STATIC_ASSERT(R1 < R, Invalid_row_index);
	SPX_STATIC_ASSERT(C1 < C, Invalid_column_index);
	return m_data[R1][C1];
    }
    template<size_type R1, size_type C1>
    const T& get(void) const
    {
	SPX_STATIC_ASSERT(R1 < R, Invalid_row_index);
	SPX_STATIC_ASSERT(C1 < C, Invalid_column_index);
	return m_data[R1][C1];
    }
    template<size_type R1, size_type C1>
    void set(T val)
    {
	SPX_STATIC_ASSERT(R1 < R, Invalid_row_index);
	SPX_STATIC_ASSERT(C1 < C, Invalid_column_index);
	m_data[R1][C1] = val;
    }
    void set(const T (&vals)[R * C])
    {
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		set(i, j, vals[j + i * C]);
	    }
	}
    }
    void set(const T (&vals)[R][C])
    {
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		set(i, j, vals[i][j]);
	    }
	}
    }

    /* Set all elements to the specified value. */
    void setAll(const T val)
    {
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		set(i, j, val);
	    }
	}
    }

    /* Set the leading diagonal to the specified value. */
    void setDiagonal(const T val)
    {
	size_type els = MIN(R, C);
	for(size_type i = 0; i < els; ++i)
	{
	    set(i, i, val);
	}
    }

    /* Matrix can be implicitly converted to a pointer to the
     * elements ordered by row then column.
     */
    operator const T*(void) const
    {
	return &m_data[0][0];
    }

    /* Retrieve data as reference to two-dimensional array. */
    const T (&data(void) const)[R][C] { return m_data; }

    /* Transpose of matrix. */
    SPxMatrix<T, C, R> t(void) const
    {
	/* Swap rows/columns. */
	SPxMatrix<T, C, R> tmp;
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		tmp.set(j, i, get(i, j));
	    }
	}

	/* Return the transposed matrix. */
	return tmp;
    }

    /* Inverse of matrix. */
    self_type inverse(void) const
    {
	/* Copy matrix to tmp and add identity matrix to right. */
	SPxMatrix<T, R, C * 2> tmp;
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		tmp.set(i, j, get(i, j));
	    }
	}
	for(size_type i = 0; i < C; ++i)
	{
	    T ident = static_cast<T>(1);
	    tmp.set(i, i + C, ident);
	}

	/* Make sure all values on leading diagonal are non-zero. */
	for(size_type i = 0; i < tmp.rows(); ++i)
	{
	    if( tmp.get(i, i) == T() )
	    {
		for(size_type j = 0; j < tmp.rows(); ++j)
		{
		    if( tmp.get(j, i) != T() )
		    {
			for(size_type k = 0; k < tmp.cols(); ++k)
			{
			    tmp.get(i, k) += tmp.get(j, k);
			}
			break;
		    }
		}
	    }
	}

	/* Perform Gaussian elimination until left hand section
	 * is identity.
	 */
	for(size_type i = 0; i < tmp.rows(); ++i)
	{
	    /* Subtract other rows so that all cells BEFORE
	     * cell on leading diagonal are zero.
	     */
	    for(size_type k = 0; k < i; ++k)
	    {
		if( tmp.get(k, k) != T() )
		{
		    T factor = tmp.get(i, k) / tmp.get(k, k);
		    for(size_type j = 0; j < tmp.cols(); ++j)
		    {
			tmp.get(i, j) -= tmp.get(k, j) * factor;
		    }
		}
	    }

	    /* Divide row so that cell on leading diagonal is 1. */
	    T head = tmp.get(i, i);
	    if( head != T() )
	    {
		for(size_type j = 0; j < tmp.cols(); ++j)
		{
		    tmp.get(i, j) /= head;
		}
	    }
	}

	/* Make sure rows are ordered correctly. */
	for(size_type i = 0; i < tmp.rows(); ++i)
	{
	    if( tmp.get(i, i) != T(1) )
	    {
		for(size_type j = 0; j < tmp.rows(); ++j)
		{
		    int rowOk = TRUE;
		    for(size_type k = 0; k < i; ++k)
		    {
			if( tmp.get(j, k) != T() )
			{
			    rowOk = FALSE;
			    break;
			}
		    }

		    if( rowOk && (tmp.get(j, i) != T()) )
		    {
			/* Swap rows i and j. */
			for(size_type k = 0; k < tmp.cols(); ++k)
			{
			    T el = tmp.get(i, k);
			    tmp.get(i, k) = tmp.get(j, k);
			    tmp.get(j, k) = el;
			}
			break;
		    }
		}
	    }
	}

	/* All cells to left of leading diagonal are now zero. */
	for(size_type i = 0; i < tmp.rows(); ++i)
	{
	    /* Subtract other rows so that all cells AFTER
	     * cell on leading diagonal are zero.
	     */
	    for(size_type k = 0; k < (tmp.rows() - i - 1); ++k)
	    {
		if( tmp.get(i, k + i + 1) != T() )
		{
		    T factor = tmp.get(i, k + i + 1);
		    for(size_type j = 0; j < tmp.cols(); ++j)
		    {
			tmp.get(i, j) -= tmp.get(i + k + 1, j) * factor;
		    }
		}
	    }
	}

	/* Right hand side is now inverse. */
	self_type inv;
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		inv.set(i, j, tmp.get(i, j + C));
	    }
	}
	return inv;
    }

    /* Dot product. */
    T dot(const self_type &other) const
    {
	SPX_STATIC_ASSERT(C == 1, Dot_product_only_valid_for_vectors);
	T val = T();
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		val += get(i, j) * other.get(i, j);
	    }
	}
	return val;
    }

    static SPxMatrix<T, R, R> identity(void)
    {
	T ident = static_cast<T>(1);
	return SPxMatrix<T, R, R>(ident);
    }

    /* Arithmetic operators. */
    self_type& operator+=(const self_type& rhs)
    {
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		get(i, j) += rhs.get(i, j);
	    }
	}
	return *this;
    }
    self_type& operator-=(const self_type& rhs)
    {
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		get(i, j) -= rhs.get(i, j);
	    }
	}
	return *this;
    }
    self_type& operator*=(T val)
    {
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		get(i, j) *= val;
	    }
	}
	return *this;
    }

    /* Make friends with non-member matrix multiply function so it
     * can call our private non-templated get function.
     */
    template<std::size_t S>
    friend inline SPxMatrix<T, R, S> operator*(const SPxMatrix<T, R, C>& lhs, const SPxMatrix<T, C, S>& rhs)
    {
	/* Matrix multiplication - columns in left-hand matrix must equal
	 * rows in right-hand matrix.
	 */
	SPxMatrix<T, R, S> res;
	for(std::size_t i = 0; i < R; ++i)
	{
	    for(std::size_t j = 0; j < S; ++j)
	    {
		T val = T();
		for(std::size_t e = 0; e < C; ++e)
		{
		    val += lhs.get(i, e) * lhs.get(rhs, e, j);
		}
		lhs.set(res, i, j, val);
	    }
	}
	return res;
    }
    self_type& operator/=(T val)
    {
	for(size_type i = 0; i < R; ++i)
	{
	    for(size_type j = 0; j < C; ++j)
	    {
		get(i, j) /= val;
	    }
	}
	return *this;
    }

    friend inline self_type operator*(const self_type& m, T val)
    {
	self_type res = m;
	res *= val;
	return res;
    }
    friend inline self_type operator/(const self_type& m, T val)
    {
	self_type res = m;
	res /= val;
	return res;
    }

    bool operator==(const SPxMatrix<T, R, C>& rhs) const
    {
	for(std::size_t i = 0; i < R; ++i)
	{
	    for(std::size_t j = 0; j < C; ++j)
	    {
		if( get(i, j) != rhs.get(i, j) )
		{
		    return false;
		}
	    }
	}
	return true;
    }
    bool operator!=(const SPxMatrix<T, R, C>& rhs) const
    {
	return !operator==(rhs);
    }

private:
    T m_data[R][C]; /* Actual data. */

    /* Private non-template functions for get/set values. */
    T& get(size_type r, size_type c)
    {
	return m_data[r][c];
    }
    const T& get(size_type r, size_type c) const
    {
	return m_data[r][c];
    }
    void set(size_type r, size_type c, T val)
    {
	// cppcheck-suppress arrayIndexOutOfBounds
	m_data[r][c] = val;
    }

    /* Private non-template functiosn for get/set values in another matrix. */
    template<size_type R2, size_type C2>
    T& get(const SPxMatrix<T, R2, C2> &other, size_type r, size_type c)
    {
	return other.get(r, c);
    }
    template<size_type R2, size_type C2>
    const T& get(const SPxMatrix<T, R2, C2> &other, size_type r, size_type c) const
    {
	return other.get(r, c);
    }
    template<size_type R2, size_type C2>
    void set(SPxMatrix<T, R2, C2> &other, size_type r, size_type c, T val) const
    {
	return other.set(r, c, val);
    }

    /* Make friends with other matrix classes so they can call the
     * private non-templated get function.
     */
    template<typename U, size_type R2, size_type C2>
    friend class SPxMatrix;

}; /* SPxMatrix. */

/* Vector class is simply a one-dimensional Matrix. */
template<typename T, std::size_t R>
class SPxVector : public SPxMatrix<T, R, 1>
{
public:
    typedef SPxMatrix<T, R, 1> base_type;
    typedef SPxVector<T, R> self_type;
    typedef typename base_type::size_type size_type;

    SPxVector(void) : base_type() {}
    SPxVector(const T (&arr)[R]) : base_type(arr) {}
    SPxVector(T v0) : base_type(v0) {}
    SPxVector(T v0, T v1) : base_type(v0, v1) {}
    SPxVector(T v0, T v1, T v2)
	: base_type(v0, v1, v2) {}
    SPxVector(T v0, T v1, T v2, T v3)
	: base_type(v0, v1, v2, v3) {}
    SPxVector(T v0, T v1, T v2, T v3,T v4)
	: base_type(v0, v1, v2, v3, v4) {}
    SPxVector(T v0, T v1, T v2, T v3, T v4, T v5)
	: base_type(v0, v1, v2, v3, v4, v5) {}
    SPxVector(T v0, T v1, T v2, T v3, T v4, T v5, T v6)
	: base_type(v0, v1, v2, v3, v4, v5, v6) {}
    SPxVector(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
	: base_type(v0, v1, v2, v3, v4, v5, v6, v7) {}
    SPxVector(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8)
	: base_type(v0, v1, v2, v3, v4, v5, v6, v7, v8) {}
    SPxVector(const self_type &other) : base_type(other) {}
    SPxVector(const base_type &other) : base_type(other) {}
    virtual ~SPxVector(void) {}

    self_type &operator=(const self_type &other)
    {
	base_type::set(other.data());
	return *this;
    }
    self_type &operator=(const base_type &other)
    {
	base_type::set(other.data());
	return *this;
    }

    /* Functions to retrieve reference to element in matrix. */
    template<size_type R1> T& get(void) { return base_type::template get<R1, 0>(); }
    template<size_type R1> const T& get(void) const { return base_type::template get<R1, 0>(); }
    template<size_type R1> void set(T val) { base_type::template set<R1, 0>(val); }
    void set(const T (&vals)[R]) { base_type::set(vals); }

    self_type cross(const self_type& other)
    {
	SPX_STATIC_ASSERT(R == 3, Cross_product_requires_3_element_vectors);
	return self_type(get<1>() * other.get<2>() - get<2>() * other.get<1>(),
			 get<2>() * other.get<0>() - get<0>() * other.get<2>(),
			 get<0>() * other.get<1>() - get<1>() * other.get<0>());
    }

    self_type quatMult(const self_type& other)
    {
	SPX_STATIC_ASSERT(R == 4, Quaternion_muliplication_requires_4_element_vectors);

	T s1 = get<0>();
	T s2 = other.get<0>();
	SPxVector<T, 3> v1(get<1>(), get<2>(), get<3>());
	SPxVector<T, 3> v2(other.get<1>(), other.get<2>(), other.get<3>());
	SPxVector<T, 3> vret = v2 * s1 + v1 * s2 + v1.cross(v2);
	return self_type(s1 * s2 - v1.dot(v2),
			 vret.template get<0>(),
			 vret.template get<1>(),
			 vret.template get<2>());
    }

}; /* SPxVector. */

template<typename T, std::size_t R, std::size_t C>
inline SPxMatrix<T, R, C> operator+(SPxMatrix<T, R, C> lhs, const SPxMatrix<T, R, C>& rhs)
{
    lhs += rhs;
    return lhs;
}
template<typename T, std::size_t R, std::size_t C>
inline SPxMatrix<T, R, C> operator-(SPxMatrix<T, R, C> lhs, const SPxMatrix<T, R, C>& rhs)
{
    lhs -= rhs;
    return lhs;
}
template<typename T, std::size_t R, std::size_t C>
inline SPxMatrix<T, R, C> operator*(SPxMatrix<T, R, C> lhs, T rhs)
{
    lhs *= rhs;
    return lhs;
}
template<typename T, std::size_t R, std::size_t C>
inline SPxMatrix<T, R, C> operator/(SPxMatrix<T, R, C> lhs, T rhs)
{
    lhs /= rhs;
    return lhs;
}

#endif /* _SPX_MATRIX_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
