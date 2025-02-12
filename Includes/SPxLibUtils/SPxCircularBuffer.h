/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxCircularBuffer class - a templated header only
*   class for managing a circular buffer.
*
*
* Revision Control:
*   05/09/22 v1.22   AGC	Fix cppcheck suppression.
*
* Previous Changes:
*   26/08/22 1.21   AGC	Suppress spurious cppcheck warning.
*   24/05/21 1.20   AGC	Add push_back_no_assign/push_front_no_assign().
*   03/11/20 1.19   AGC	Fix shadow warnings.
*   02/11/20 1.18   AGC	Avoid cppcheck syntax error warnings.
*   30/10/20 1.17   AGC	Silence cppcheck syntax error warnings.
*   14/02/19 1.16   AGC	Use feature test macro to avoid VS2019 allocator warnings.
*   24/07/18 1.15   AGC	Fix VS2017.8 compiler issue.
*   15/12/17 1.14   AGC	Fix VS2017.5 compiler issue.
*   02/12/16 1.13   AGC	Avoid clang on Windows bug.
*   11/01/16 1.12   AGC	Avoid using MIN() macro.
*   10/12/15 1.11   AGC	Add last required operator to iterator.
*   09/11/15 1.10   AGC	Make circular buffer iterator properly random access.
*   14/04/15 1.9    AGC	Add copy constructor and assignment operator.
*   04/07/14 1.8    AGC	Add required iterator header.
*   09/05/14 1.7    AGC	Use canonical style for erase().
*   02/05/14 1.6    AGC	Fix various warnings.
*   01/05/14 1.5    AGC	Add insert/erase functions.
*			Fix pop_front().
*			Fix const_iterator.
*			Use correct iterator tag type.
*   16/01/14 1.4    AGC	Do not derive from std::iterator.
*   15/01/14 1.3    AGC	Remove extra semi-colons.
*			Fix reverse iterators.
*   08/11/13 1.2    AGC	Fix shadow warnings.
*			Use provided allocator.
*   04/11/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CIRCULAR_BUFFER_H
#define _SPX_CIRCULAR_BUFFER_H

/* Standard headers. */
#include <cstddef>
#include <iterator>
#include <memory>

namespace SPxCircularBufferImpl
{
    template <class C, class elem_type=typename C::value_type>
    class iterator;

    template <class C, class elem_type>
    bool operator==(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs);
    template <class C, class elem_type>
    bool operator!=(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs);
    template <class C, class elem_type>
    bool operator<(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs);
    template <class C, class elem_type>
    bool operator>(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs);
    template <class C, class elem_type>
    bool operator<=(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs);
    template <class C, class elem_type>
    bool operator>=(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs);

    template <class C, class elem_type>
    class iterator
    {
    public:
	typedef iterator<C, elem_type> self_type;
	typedef C buffer_type;
	typedef typename C::size_type size_type;
	typedef std::random_access_iterator_tag iterator_category;
	typedef elem_type value_type;
	typedef elem_type* pointer;
	typedef elem_type& reference;
	typedef typename C::difference_type difference_type;

	iterator(buffer_type *buffer=NULL, size_type pos_=0)
	    : m_buf(buffer), m_pos(pos_) {}
	~iterator() {}

	elem_type &operator*() const { return (*m_buf)[m_pos]; }
	elem_type *operator->() const { return &(operator*()); }
	self_type &operator++() { ++m_pos; return *this; }
	self_type operator++(int) { self_type tmp(*this); ++(*this); return tmp; }
	self_type operator+(difference_type n) { self_type tmp(*this); tmp.m_pos += n; return tmp; }
	difference_type operator+(const self_type& other) const { return m_pos + other.m_pos; }
	self_type &operator+=(difference_type n) { m_pos += n; return *this; }
	self_type &operator--() { --m_pos; return *this; }
	self_type operator--(int) { self_type tmp(*this); --(*this); return tmp; }
	self_type operator-(difference_type n) { self_type tmp(*this); tmp.m_pos -= n; return tmp; }
	difference_type operator-(const self_type& other) const { return m_pos - other.m_pos; }
	self_type &operator-=(difference_type n) { m_pos -= n; return *this; }
	size_type pos() const { return m_pos; }

    private:
	buffer_type *m_buf;
	size_type m_pos;
    };

    template <class C, class elem_type>
    bool operator==(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs)
    { return (lhs.pos() == rhs.pos()); }
    template <class C, class elem_type>
    bool operator!=(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs)
#ifdef _WIN32
#ifdef __clang__
	/* Clang on Windows currently crashes if optimisation is enabled for this function. */
	__attribute__((optnone))
#endif
#endif
    { return (lhs.pos() != rhs.pos()); }
    template <class C, class elem_type>
    bool operator<(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs)
    { return (lhs.pos() < rhs.pos()); }
    template <class C, class elem_type>
    bool operator>(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs)
    { return (lhs.pos() > rhs.pos()); }
    template <class C, class elem_type>
    bool operator<=(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs)
    { return (lhs.pos() <= rhs.pos()); }
    template <class C, class elem_type>
    bool operator>=(const iterator<C, elem_type> &lhs, const iterator<C, elem_type> &rhs)
    { return (lhs.pos() >= rhs.pos()); }
}

template <class T, class A=std::allocator<T> >
class SPxCircularBuffer
{
public:
    typedef SPxCircularBuffer<T, A> self_type;
    /* VS2019 with latest C++ version requires using allocator_traits.
     * There is no feature test for allocator_traits itself, so
     * detect support using a feature test that implies allocator_traits.
     */
#ifdef __cpp_lib_allocator_traits_is_always_equal
    typedef typename std::allocator_traits<A>::value_type value_type;
    typedef typename std::allocator_traits<A>::pointer pointer;
    typedef typename std::allocator_traits<A>::const_pointer const_pointer;
    typedef typename std::allocator_traits<A>::difference_type difference_type;
    typedef typename std::allocator_traits<A>::size_type size_type;
#else
    typedef typename A::value_type value_type;
    typedef typename A::pointer pointer;
    typedef typename A::const_pointer const_pointer;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;
#endif
    typedef T& reference;
    typedef const T& const_reference;
    typedef SPxCircularBufferImpl::iterator<self_type> iterator;
    typedef SPxCircularBufferImpl::iterator<const self_type, const value_type> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef A allocator_type;

    explicit SPxCircularBuffer(const allocator_type& new_allocator=allocator_type())
	: m_buffer(NULL), m_capacity(0), m_start(0), m_size(0), m_allocator(new_allocator) {}
    explicit SPxCircularBuffer(size_type new_capacity, const allocator_type& new_allocator=allocator_type())
	: m_buffer(NULL), m_capacity(0), m_start(0), m_size(0), m_allocator(new_allocator)
    {
	m_buffer = alloc(new_capacity);
	m_capacity = new_capacity;
    }
    ~SPxCircularBuffer(void)
    {
	dealloc(m_buffer, m_capacity);
	m_buffer = NULL;
	m_capacity = 0;
    }
    SPxCircularBuffer(const self_type& other)
	: m_buffer(NULL), m_capacity(other.m_capacity), m_start(other.m_start),
	m_size(other.m_size), m_allocator(other.m_allocator)
    {
	m_buffer = alloc(m_capacity);
	for(size_type i = 0; i < m_size; ++i)
	{
	    (*this)[i] = other[i];
	}
    }

    self_type& operator=(const self_type& other)
    {
	dealloc(m_buffer, m_capacity);
	m_buffer = NULL;
	m_capacity = other.m_capacity;
	m_start = other.m_start;
	m_size = other.m_size;
	m_allocator = other.m_allocator;
	m_buffer = alloc(m_capacity);
	for(size_type i = 0; i < m_size; ++i)
	{
	    (*this)[i] = other[i];
	}
	return *this;
    }

    allocator_type get_allocator() const { return m_allocator; }
    allocator_type& get_allocator() { return m_allocator; }
    iterator begin() { return iterator(this, 0U); }
    iterator end() { return iterator(this, size()); }
    const_iterator begin() const { return const_iterator(this, 0U); }
    const_iterator end() const { return const_iterator(this, size()); }
    reverse_iterator rbegin() { return reverse_iterator(iterator(this, size())); }
    reverse_iterator rend() { return reverse_iterator(iterator(this, 0U)); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(const_iterator(this, size())); }
    const_reverse_iterator rend() const { return const_reverse_iterator(const_iterator(this, 0U)); }
    reference operator[](size_type index) { return m_buffer[getIndex(index)]; }
    const_reference operator[](size_type index) const { return m_buffer[getIndex(index)]; }
    reference front() { return m_buffer[m_start]; }
    reference back() { return m_buffer[getIndex(m_size - 1)]; }
    const_reference front() const { return m_buffer[m_start]; }
    const_reference back() const { return m_buffer[getIndex(m_size - 1)]; }
    size_type size() const { return m_size; }
    bool empty() const { return (m_size == 0); }
    bool full() const { return (m_size == m_capacity); }
    size_type capacity() const { return m_capacity; }
    void set_capacity(size_type new_capacity)
    {
	if( new_capacity == m_capacity )
	{
	    return;
	}

	T *ptr = alloc(new_capacity);
	if( m_capacity > 0 )
	{
	    size_type numToCopy = (m_capacity < new_capacity) ? m_capacity : new_capacity;
	    iterator it = begin();
	    for(size_type i = 0; i < numToCopy; ++i, ++it)
	    {
		*(ptr + i) = *it;
	    }
	    m_start = 0;
	    if( new_capacity < m_size )
	    {
		m_size = new_capacity;
	    }
	}
	dealloc(m_buffer, m_capacity);
	m_buffer = ptr;
	m_capacity = new_capacity;
    }
    void push_back(const_reference item=value_type())
    {
	if( capacity() == 0 )
	{
	    return;
	}
	if( full() )
	{
	    m_start = (m_start + 1) % m_capacity;
	    m_size -= 1;
	}
	m_buffer[getIndex(m_size)] = item;
	m_size += 1;
    }
    void push_back_no_assign()
    {
	if(capacity() == 0)
	{
	    return;
	}
	if(full())
	{
	    m_start = (m_start + 1) % m_capacity;
	    m_size -= 1;
	}
	m_size += 1;
    }
    void push_front(const_reference item=value_type())
    {
	if( capacity() == 0 )
	{
	    return;
	}
	if( full() )
	{
	    m_size -= 1;
	}
	m_buffer[getIndex(-1)] = item;
	m_start = (m_start + m_capacity - 1) % m_capacity;
	m_size += 1;
    }
    void push_front_no_assign()
    {
	if(capacity() == 0)
	{
	    return;
	}
	if(full())
	{
	    m_size -= 1;
	}
	m_start = (m_start + m_capacity - 1) % m_capacity;
	m_size += 1;
    }
    void pop_back()
    {
	if( !empty() )
	{
	    m_size -= 1;
	}
    }
    void pop_front()
    {
	if( !empty() )
	{
	    m_start = getIndex(1);
	    m_size -= 1;
	}
    }
    void insert(const_reference item)
    {
	for(iterator it = begin(); it != end(); ++it)
	{
	    if( *it == item )
	    {
		return;
	    }
	}
	push_back(item);
    }
    void clear() { m_size = 0; m_start = 0; }
    iterator erase(iterator eraseIt)
    {
	for(iterator it = eraseIt; it != end(); ++it)
	{
	    iterator nIt = it;
	    ++nIt;
	    if( nIt != end() )
	    {
		*it = *nIt;
	    }
	}
	pop_back();
	return eraseIt;
    }
    void erase(const_reference item)
    {
	for(iterator it = begin(); it != end(); )
	{
	    if( *it == item )
	    {
		erase(it);
	    }
	    else
	    {
		++it;
	    }
	}
    }

private:
    T *m_buffer;
    size_type m_capacity;
    size_type m_start;
    size_type m_size;
    A m_allocator;

    T *alloc(size_type new_capacity)
    {
	/* VS2019 with latest C++ version requires using allocator_traits.
	 * There is no feature test for allocator_traits itself, so
	 * detect support using a feature test that implies allocator_traits.
	 */
#ifdef __cpp_lib_allocator_traits_is_always_equal
	T *ptr = std::allocator_traits<A>::allocate(m_allocator, new_capacity);
#else
	T *ptr = m_allocator.allocate(new_capacity);
#endif
	for(size_type i = 0; i < new_capacity; ++i)
	{
	    // cppcheck-suppress legacyUninitvar
	    T t = T();
#ifdef __cpp_lib_allocator_traits_is_always_equal
	    std::allocator_traits<A>::construct(m_allocator, ptr + i, t);
#else
	    m_allocator.construct(ptr + i, t);
#endif
	}
	return ptr;
    }
    void dealloc(T *ptr, size_type curr_capacity)
    {
	for(size_type i = 0; i < curr_capacity; ++i)
	{
#ifdef __cpp_lib_allocator_traits_is_always_equal
	    std::allocator_traits<A>::destroy(m_allocator, ptr + i);
#else
	    m_allocator.destroy(ptr + i);
#endif
	}
#ifdef __cpp_lib_allocator_traits_is_always_equal
	std::allocator_traits<A>::deallocate(m_allocator, ptr, curr_capacity);
#else
	m_allocator.deallocate(ptr, curr_capacity);
#endif
    }
    size_type getIndex(difference_type index) const { return (m_start + m_capacity + index) % m_capacity; }
};

#endif /* _SPX_CIRCULAR_BUFFER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
