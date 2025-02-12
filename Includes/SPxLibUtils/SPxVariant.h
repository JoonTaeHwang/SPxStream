/*********************************************************************
*
* (c) Copyright 2017, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for type-safe union.
*
* Revision Control:
*   22/06/18 v1.6    AGC	Suppress more cppcheck warnings.
*
* Previous Changes:
*   20/06/18 1.5    AGC	Suppress spurious cppcheck warnings.
*   17/11/17 1.4    AGC	Fix build with newer gcc.
*   15/03/17 1.3    AGC	Fix warnings for copy constructors.
*   05/01/17 1.2    AGC	Suppress various erroneous cppcheck warnings.
*   04/01/17 1.1    AGC	Initial Version.
*
**********************************************************************/
#ifndef SPX_VARIANT_H_
#define SPX_VARIANT_H_

#ifndef	_WIN32
#include <stdint.h>
#endif

#include "SPxLibUtils/SPxTypes.h"

template<typename T0,
    typename T1 = void,
    typename T2 = void,
    typename T3 = void,
    typename T4 = void>
class SPxVariant;

template<typename T0>
class SPxVariant<T0, void, void, void, void>
{
public:
    template<typename T>
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const T& t)
    {
	static const int isValidType = !SPxIsSame<T, void>::value &&
	    SPxIsSame<T, T0>::value;
	SPX_STATIC_ASSERT(isValidType, Type_not_valid_for_variant);

	alloc(t);
    }
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const SPxVariant<T0>& other)
    {
	alloc(other);
    }
    ~SPxVariant()
    {
	destroy();
    }

    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0>& operator=(const SPxVariant<T0>& other)
    {
	destroy();
	alloc(other);
	return *this;
    }
    template<typename T>
    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0>& operator=(const T& other)
    {
	destroy();
	alloc(other);
	return *this;
    }

    template<typename R, typename V>
    R visit(const V& v)
    {
	return v(*get<T0>());
    }

private:
    unsigned char m_val[sizeof(T0) + SPxAlignof(T0) - 1];
    unsigned char *m_ptr;
    
    template<typename T>
    void alloc(const T& other)
    {
	/* Enforce correct alignment for memory. */
	m_ptr = reinterpret_cast<unsigned char*>(((reinterpret_cast<uintptr_t>(m_val) + SPxAlignof(T) - 1) & ~(SPxAlignof(T) - 1)));

	new (m_ptr) T0(other);
    }
    void alloc(const SPxVariant<T0>& other)
    {
	alloc(*other.get<T0>());
    }
    void destroy()
    {
	get<T0>()->~T0();
    }

    template<typename T>
    T* get()
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<T*>(m_ptr);
    }
    template<typename T>
    const T* get() const
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<const T*>(m_ptr);
    }
};

template<typename T0, typename T1>
class SPxVariant<T0, T1, void, void, void>
{
public:
    template<typename T>
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const T& t)
    {
	static const int isValidType = !SPxIsSame<T, void>::value &&
	    (SPxIsSame<T, T0>::value || SPxIsSame<T, T1>::value);
	SPX_STATIC_ASSERT(isValidType, Type_not_valid_for_variant);

	setType<T>();

	alloc(t);
    }
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const SPxVariant<T0, T1>& other)
    {
	m_typeIndex = other.m_typeIndex;
	alloc(other);
    }
    ~SPxVariant()
    {
	destroy();
    }

    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0, T1>& operator=(const SPxVariant<T0, T1>& other)
    {
	destroy();
	m_typeIndex = other.m_typeIndex;
	alloc(other);
	return *this;
    }
    template<typename T>
    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0, T1>& operator=(const T& other)
    {
	destroy();
	setType<T>();
	alloc(other);
	return *this;
    }

    template<typename R, typename V>
    R visit(const V& v)
    {
	switch(m_typeIndex)
	{
	case 0:
	    return v(*get<T0>());
	case 1:
	    return v(*get<T1>());
	}
	return R();
    }

private:
    unsigned char m_val[MAX(sizeof(T0) + SPxAlignof(T0) - 1,
			    sizeof(T1) + SPxAlignof(T1) - 1)];
    unsigned char *m_ptr;
    unsigned int m_typeIndex;

    template<typename T>
    void setType()
    {
	if(SPxIsSame<T, T0>::value)
	{
	    m_typeIndex = 0;
	}
	else if(SPxIsSame<T, T1>::value)
	{
	    m_typeIndex = 1;
	}
    }

    template<typename T>
    void alloc(const T& other)
    {
	/* Enforce correct alignment for memory. */
	m_ptr = reinterpret_cast<unsigned char*>(((reinterpret_cast<uintptr_t>(m_val) + SPxAlignof(T) - 1) & ~(SPxAlignof(T) - 1)));

	new (m_ptr) T(other);
    }
    void alloc(const SPxVariant<T0, T1>& other)
    {
	switch(m_typeIndex)
	{
	case 0:
	    alloc(*other.get<T0>());
	    break;
	case 1:
	    alloc(*other.get<T1>());
	    break;
	}
    }
    void destroy()
    {
	switch(m_typeIndex)
	{
	case 0:
	    get<T0>()->~T0();
	    break;
	case 1:
	    get<T1>()->~T1();
	    break;
	}
    }

    template<typename T>
    T* get()
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<T*>(m_ptr);
    }
    template<typename T>
    const T* get() const
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<const T*>(m_ptr);
    }
};

template<typename T0, typename T1, typename T2>
class SPxVariant<T0, T1, T2, void, void>
{
public:
    template<typename T>
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const T& t)
    {
	static const int isValidType = !SPxIsSame<T, void>::value &&
	    (SPxIsSame<T, T0>::value || SPxIsSame<T, T1>::value || SPxIsSame<T, T2>::value);
	SPX_STATIC_ASSERT(isValidType, Type_not_valid_for_variant);

	setType<T>();

	/* Zero first element of value to suppress cppcheck warning. */
	m_val[0] = 0;

	alloc(t);
    }
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const SPxVariant<T0, T1, T2>& other)
    {
	/* Zero first element of value to suppress cppcheck warning. */
	m_val[0] = 0;

	m_typeIndex = other.m_typeIndex;
	alloc(other);
    }
    ~SPxVariant()
    {
	destroy();
    }

    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0, T1, T2>& operator=(const SPxVariant<T0, T1, T2>& other)
    {
	destroy();
	m_typeIndex = other.m_typeIndex;
	alloc(other);
	return *this;
    }
    template<typename T>
    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0, T1, T2>& operator=(const T& other)
    {
	destroy();
	setType<T>();
	alloc(other);
	return *this;
    }

    template<typename R, typename V>
    R visit(const V& v)
    {
	switch(m_typeIndex)
	{
	case 0:
	    return v(*get<T0>());
	case 1:
	    return v(*get<T1>());
	case 2:
	    return v(*get<T2>());
	}
	return R();
    }

private:
    unsigned char m_val[MAX(MAX(sizeof(T0) + SPxAlignof(T0) - 1,
				sizeof(T1) + SPxAlignof(T1) - 1),
			    sizeof(T2) + SPxAlignof(T2) - 1)];
    unsigned char *m_ptr;
    unsigned int m_typeIndex;

    template<typename T>
    void setType()
    {
	if(SPxIsSame<T, T0>::value)
	{
	    m_typeIndex = 0;
	}
	else if(SPxIsSame<T, T1>::value)
	{
	    m_typeIndex = 1;
	}
	else if(SPxIsSame<T, T2>::value)
	{
	    m_typeIndex = 2;
	}
    }

    template<typename T>
    void alloc(const T& other)
    {
	/* Enforce correct alignment for memory. */
	m_ptr = reinterpret_cast<unsigned char*>(((reinterpret_cast<uintptr_t>(m_val) + SPxAlignof(T) - 1) & ~(SPxAlignof(T) - 1)));

	new (m_ptr) T(other);
    }
    void alloc(const SPxVariant<T0, T1, T2>& other)
    {
	switch(m_typeIndex)
	{
	case 0:
	    alloc(*other.get<T0>());
	    break;
	case 1:
	    alloc(*other.get<T1>());
	    break;
	case 2:
	    alloc(*other.get<T2>());
	    break;
	}
    }
    void destroy()
    {
	switch(m_typeIndex)
	{
	case 0:
	    get<T0>()->~T0();
	    break;
	case 1:
	    get<T1>()->~T1();
	    break;
	case 2:
	    get<T2>()->~T2();
	    break;
	}
    }

    template<typename T>
    T* get()
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<T*>(m_ptr);
    }
    template<typename T>
    const T* get() const
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<const T*>(m_ptr);
    }
};

template<typename T0, typename T1, typename T2, typename T3>
class SPxVariant<T0, T1, T2, T3, void>
{
public:
    template<typename T>
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const T& t)
    {
	static const int isValidType = !SPxIsSame<T, void>::value &&
	    (SPxIsSame<T, T0>::value || SPxIsSame<T, T1>::value || SPxIsSame<T, T2>::value ||
	     SPxIsSame<T, T3>::value);
	SPX_STATIC_ASSERT(isValidType, Type_not_valid_for_variant);

	setType<T>();

	/* Zero first element of value to suppress cppcheck warning. */
	m_val[0] = 0;

	alloc(t);
    }
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const SPxVariant<T0, T1, T2, T3>& other)
    {
	/* Zero first element of value to suppress cppcheck warning. */
	m_val[0] = 0;

	m_typeIndex = other.m_typeIndex;
	alloc(other);
    }
    ~SPxVariant()
    {
	destroy();
    }

    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0, T1, T2, T3>& operator=(const SPxVariant<T0, T1, T2, T3>& other)
    {
	destroy();
	m_typeIndex = other.m_typeIndex;
	alloc(other);
	return *this;
    }
    template<typename T>
    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0, T1, T2, T3>& operator=(const T& other)
    {
	destroy();
	setType<T>();
	alloc(other);
	return *this;
    }

    template<typename R, typename V>
    R visit(const V& v)
    {
	switch(m_typeIndex)
	{
	case 0:
	    return v(*get<T0>());
	case 1:
	    return v(*get<T1>());
	case 2:
	    return v(*get<T2>());
	case 3:
	    return v(*get<T3>());
	}
	return R();
    }

private:
    unsigned char m_val[MAX(MAX(sizeof(T0) + SPxAlignof(T0) - 1,
				sizeof(T1) + SPxAlignof(T1) - 1),
			    MAX(sizeof(T2) + SPxAlignof(T2) - 1,
				sizeof(T3) + SPxAlignof(T3) - 1))];
    unsigned char *m_ptr;
    unsigned int m_typeIndex;

    template<typename T>
    void setType()
    {
	if(SPxIsSame<T, T0>::value)
	{
	    m_typeIndex = 0;
	}
	else if(SPxIsSame<T, T1>::value)
	{
	    m_typeIndex = 1;
	}
	else if(SPxIsSame<T, T2>::value)
	{
	    m_typeIndex = 2;
	}
	else if(SPxIsSame<T, T3>::value)
	{
	    m_typeIndex = 3;
	}
    }

    template<typename T>
    void alloc(const T& other)
    {
	/* Enforce correct alignment for memory. */
	m_ptr = reinterpret_cast<unsigned char*>(((reinterpret_cast<uintptr_t>(m_val) + SPxAlignof(T) - 1) & ~(SPxAlignof(T) - 1)));

	new (m_ptr) T(other);
    }
    void alloc(const SPxVariant<T0, T1, T2, T3>& other)
    {
	switch(m_typeIndex)
	{
	case 0:
	    alloc(*other.get<T0>());
	    break;
	case 1:
	    alloc(*other.get<T1>());
	    break;
	case 2:
	    alloc(*other.get<T2>());
	    break;
	case 3:
	    alloc(*other.get<T3>());
	    break;
	}
    }
    void destroy()
    {
	switch(m_typeIndex)
	{
	case 0:
	    get<T0>()->~T0();
	    break;
	case 1:
	    get<T1>()->~T1();
	    break;
	case 2:
	    get<T2>()->~T2();
	    break;
	case 3:
	    get<T3>()->~T3();
	    break;
	}
    }

    template<typename T>
    T* get()
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<T*>(m_ptr);
    }
    template<typename T>
    const T* get() const
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<const T*>(m_ptr);
    }
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
class SPxVariant
{
public:
    template<typename T>
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const T& t)
    {
	static const int isValidType = !SPxIsSame<T, void>::value &&
	    (SPxIsSame<T, T0>::value || SPxIsSame<T, T1>::value || SPxIsSame<T, T2>::value ||
	     SPxIsSame<T, T3>::value || SPxIsSame<T, T4>::value);
	SPX_STATIC_ASSERT(isValidType, Type_not_valid_for_variant);

	setType<T>();

	/* Zero first element of value to suppress cppcheck warning. */
	m_val[0] = 0;

	alloc(t);
    }
    // cppcheck-suppress uninitMemberVar
    SPxVariant(const SPxVariant<T0, T1, T2, T3>& other)
    {
	/* Zero first element of value to suppress cppcheck warning. */
	m_val[0] = 0;

	m_typeIndex = other.m_typeIndex;
	alloc(other);
    }
    ~SPxVariant()
    {
	destroy();
    }

    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0, T1, T2, T3, T4>& operator=(const SPxVariant<T0, T1, T2, T3, T4>& other)
    {
	destroy();
	m_typeIndex = other.m_typeIndex;
	alloc(other);
	return *this;
    }
    template<typename T>
    // cppcheck-suppress operatorEqVarError
    SPxVariant<T0, T1, T2, T3, T4>& operator=(const T& other)
    {
	destroy();
	setType<T>();
	alloc(other);
	return *this;
    }

    template<typename R, typename V>
    R visit(const V& v)
    {
	switch(m_typeIndex)
	{
	case 0:
	    return v(*get<T0>());
	case 1:
	    return v(*get<T1>());
	case 2:
	    return v(*get<T2>());
	case 3:
	    return v(*get<T3>());
	case 4:
	    return v(*get<T4>());
	}
	return R();
    }

private:
    unsigned char m_val[MAX(MAX(MAX(sizeof(T0) + SPxAlignof(T0) - 1,
				    sizeof(T1) + SPxAlignof(T1) - 1),
				MAX(sizeof(T2) + SPxAlignof(T2) - 1,
				    sizeof(T3) + SPxAlignof(T3) - 1)),
			    sizeof(T4) + SPxAlignof(T4) - 1)];
    unsigned char *m_ptr;
    unsigned int m_typeIndex;

    template<typename T>
    void setType()
    {
	if(SPxIsSame<T, T0>::value)
	{
	    m_typeIndex = 0;
	}
	else if(SPxIsSame<T, T1>::value)
	{
	    m_typeIndex = 1;
	}
	else if(SPxIsSame<T, T2>::value)
	{
	    m_typeIndex = 2;
	}
	else if(SPxIsSame<T, T3>::value)
	{
	    m_typeIndex = 3;
	}
	else if(SPxIsSame<T, T4>::value)
	{
	    m_typeIndex = 4;
	}
    }

    template<typename T>
    void alloc(const T& other)
    {
	/* Enforce correct alignment for memory. */
	m_ptr = reinterpret_cast<unsigned char*>(((reinterpret_cast<uintptr_t>(m_val) + SPxAlignof(T) - 1) & ~(SPxAlignof(T) - 1)));

	new (m_ptr) T(other);
    }
    void alloc(const SPxVariant<T0, T1, T2, T3, T4>& other)
    {
	switch(m_typeIndex)
	{
	case 0:
	    alloc(*other.get<T0>());
	    break;
	case 1:
	    alloc(*other.get<T1>());
	    break;
	case 2:
	    alloc(*other.get<T2>());
	    break;
	case 3:
	    alloc(*other.get<T3>());
	    break;
	case 4:
	    alloc(*other.get<T4>());
	    break;
	}
    }
    void destroy()
    {
	switch(m_typeIndex)
	{
	case 0:
	    get<T0>()->~T0();
	    break;
	case 1:
	    get<T1>()->~T1();
	    break;
	case 2:
	    get<T2>()->~T2();
	    break;
	case 3:
	    get<T3>()->~T3();
	    break;
	case 4:
	    get<T4>()->~T4();
	    break;
	}
    }

    template<typename T>
    T* get()
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<T*>(m_ptr);
    }
    template<typename T>
    const T* get() const
    {
	// cppcheck-suppress invalidPointerCast
	return reinterpret_cast<const T*>(m_ptr);
    }
};

#endif /* SPX_VARIANT_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
