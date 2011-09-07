/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_VECTOR_INL_
#define BE_MINITL_CONTAINER_INL_VECTOR_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>

namespace minitl
{

template< typename T >
template< typename POLICY >
class vector<T>::base_iterator
    :   public random_access_iterator<T, typename vector<T>::difference_type>
{
    friend class vector<T>;
public:
    typedef          random_access_iterator_tag         iterator_category;
    typedef typename POLICY::value_type                 value_type;
    typedef typename POLICY::pointer                    pointer;
    typedef typename POLICY::reference                  reference;
    typedef typename POLICY::size_type                  size_type;
    typedef typename POLICY::difference_type            difference_type;
public:
    const vector<T>*            m_owner;
    typename POLICY::pointer    m_iterator;
private:
    base_iterator(const vector<T>* owner, typename POLICY::pointer it);
public:
    base_iterator();
    template< typename OTHERPOLICY >
    base_iterator(const base_iterator<OTHERPOLICY>& other);
    ~base_iterator();
public:
    bool operator==(const base_iterator<POLICY>& other);
    bool operator!=(const base_iterator<POLICY>& other);

    base_iterator<POLICY>& operator=(const base_iterator<POLICY>& other)
    {
        m_owner = other.m_owner;
        m_iterator = other.m_iterator;
        return *this;
    }
    base_iterator<POLICY> operator+(typename POLICY::difference_type offset) const
    {
        return base_iterator<POLICY>(m_owner, POLICY::advance(m_iterator, offset));
    }
    base_iterator<POLICY> operator-(typename POLICY::difference_type offset) const
    {
        return base_iterator<POLICY>(m_owner, POLICY::advance(m_iterator, -offset));
    }
    typename POLICY::difference_type operator-(const base_iterator<POLICY>& other) const
    {
        be_assert_recover(m_owner == other.m_owner, "can't differ between unrelated iterators", return 0);
        return distance(other.m_iterator, m_iterator);
    }

    base_iterator<POLICY>& operator++()
    {
        m_iterator = POLICY::advance(m_iterator, 1);
        return *this;
    }
    base_iterator<POLICY>  operator++(int)
    {
        base_iterator<POLICY> p = *this;
        m_iterator = POLICY::advance(m_iterator, 1);
        return p;
    }
    base_iterator<POLICY>& operator+=(typename POLICY::difference_type size)
    {
        m_iterator = POLICY::advance(m_iterator, size);
        return *this;
    }
    base_iterator<POLICY>& operator--()
    {
        m_iterator = POLICY::advance(m_iterator, -1);
        return *this;
    }
    base_iterator<POLICY>  operator--(int)
    {
        base_iterator<POLICY> p = *this;
        m_iterator = POLICY::advance(m_iterator, -1);
        return p;
    }
    base_iterator<POLICY>& operator-=(typename POLICY::difference_type size)
    {
        m_iterator = POLICY::advance(m_iterator, -size);
        return *this;
    }
    typename POLICY::pointer    operator->() const;
    typename POLICY::reference  operator*() const;
};

template< typename T >
template< typename POLICY >
vector<T>::base_iterator<POLICY>::base_iterator()
:   m_owner(0)
,   m_iterator(0)
{
}

template< typename T >
template< typename POLICY >
vector<T>::base_iterator<POLICY>::base_iterator(const vector<T>* owner, typename POLICY::pointer it)
:   m_owner(owner)
,   m_iterator(it)
{
}

template< typename T >
template< typename POLICY >
template< typename OTHERPOLICY >
vector<T>::base_iterator<POLICY>::base_iterator(const base_iterator<OTHERPOLICY>& other)
:   m_owner(other.m_owner)
,   m_iterator(other.m_iterator)
{
}

template< typename T >
template< typename POLICY >
vector<T>::base_iterator<POLICY>::~base_iterator()
{
}

template< typename T >
template< typename POLICY >
bool vector<T>::base_iterator<POLICY>::operator==(const base_iterator<POLICY>& other)
{
    return m_iterator == other.m_iterator;
}

template< typename T >
template< typename POLICY >
bool vector<T>::base_iterator<POLICY>::operator!=(const base_iterator<POLICY>& other)
{
    return m_iterator != other.m_iterator;
}

template< typename T >
template< typename POLICY >
typename POLICY::pointer vector<T>::base_iterator<POLICY>::operator->() const
{
    return m_iterator;
}

template< typename T >
template< typename POLICY >
typename POLICY::reference vector<T>::base_iterator<POLICY>::operator*() const
{
    return *m_iterator;
}


template< typename T >
struct vector<T>::iterator_policy
{
    typedef typename vector<T>::value_type       value_type;
    typedef typename vector<T>::pointer          pointer;
    typedef typename vector<T>::reference        reference;
    typedef typename vector<T>::size_type        size_type;
    typedef typename vector<T>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
};

template< typename T >
struct vector<T>::const_iterator_policy
{
    typedef typename vector<T>::value_type const value_type;
    typedef typename vector<T>::const_pointer    pointer;
    typedef typename vector<T>::const_reference  reference;
    typedef typename vector<T>::size_type        size_type;
    typedef typename vector<T>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
};

template< typename T >
struct vector<T>::reverse_iterator_policy
{
    typedef typename vector<T>::value_type       value_type;
    typedef typename vector<T>::pointer          pointer;
    typedef typename vector<T>::reference        reference;
    typedef typename vector<T>::size_type        size_type;
    typedef typename vector<T>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
};

template< typename T >
struct vector<T>::const_reverse_iterator_policy
{
    typedef typename vector<T>::value_type const value_type;
    typedef typename vector<T>::const_pointer    pointer;
    typedef typename vector<T>::const_reference  reference;
    typedef typename vector<T>::size_type        size_type;
    typedef typename vector<T>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
};


template< typename T >
vector<T>::vector(BugEngine::Allocator& allocator, size_type count)
:   m_memory(allocator, count)
,   m_end(m_memory)
,   m_capacity(m_memory)
{
}

template< typename T >
vector<T>::vector(const vector& other)
    :   m_memory(other.m_memory.arena(), other.size())
    ,   m_end(m_memory)
    ,   m_capacity(m_memory)
{
    push_back(other.begin(), other.end());
}

template< typename T >
template< typename ITERATOR >
vector<T>::vector(BugEngine::Allocator& allocator, ITERATOR first, ITERATOR last)
    :   m_memory(allocator, minitl::distance(first, last))
    ,   m_end(m_memory)
    ,   m_capacity(m_memory)
{
    push_back(first, last);

}

template< typename T >
vector<T>& vector<T>::operator=(const vector<T>& other)
{
    clear();
    push_back(other.begin(), other.end());
    return *this;
}

template< typename T >
vector<T>::~vector()
{
    for (const_pointer t = m_memory; t != m_end; t = advance(t, 1))
    {
        t->~T();
    }
}

template< typename T >
typename vector<T>::iterator                 vector<T>::begin()
{
    return iterator(this, m_memory);
}

template< typename T >
typename vector<T>::iterator                 vector<T>::end()
{
    return iterator(this, m_end);
}

template< typename T >
typename vector<T>::const_iterator           vector<T>::begin() const
{
    return const_iterator(this, m_memory);
}

template< typename T >
typename vector<T>::const_iterator           vector<T>::end() const
{
    return const_iterator(this, m_end);
}

template< typename T >
typename vector<T>::reverse_iterator         vector<T>::rbegin()
{
    return reverse_iterator(this, advance(m_end, -1));
}

template< typename T >
typename vector<T>::reverse_iterator         vector<T>::rend()
{
    return reverse_iterator(this, advance(m_memory.data(), -1));
}

template< typename T >
typename vector<T>::const_reverse_iterator   vector<T>::rbegin() const
{
    return const_reverse_iterator(this, advance(m_end, -1));
}

template< typename T >
typename vector<T>::const_reverse_iterator   vector<T>::rend() const
{
    return const_reverse_iterator(this, advance(m_memory.data(), -1));
}


template< typename T >
typename vector<T>::size_type                vector<T>::size() const
{
    return distance(m_memory.data(), m_end);
}

template< typename T >
bool                                                vector<T>::empty() const
{
    return m_end == m_memory;
}


template< typename T >
typename vector<T>::reference                vector<T>::operator[](size_type i)
{
    return *advance(m_memory.data(), i);
}

template< typename T >
typename vector<T>::const_reference          vector<T>::operator[](size_type i) const
{
    return *advance(m_memory.data(), i);
}


template< typename T >
void                                                vector<T>::push_back(const_reference r)
{
    reserve(size() + 1);
    new((void*)m_end) T(r);
    m_end = advance_ptr(m_end, 1);
}

template< typename T >
template< typename ITERATOR >
void                                                vector<T>::push_back(ITERATOR first, ITERATOR last)
{
    size_type count = minitl::distance(first, last);
    reserve(size() + count);
    while (first != last)
    {
        new((void*)m_end) T(*first);
        m_end = advance_ptr(m_end, 1);
        ++first;
    }
}

template< typename T >
typename vector<T>::iterator                 vector<T>::erase(iterator it)
{
    return erase(it, it+1);
}

template< typename T >
typename vector<T>::iterator                 vector<T>::erase(iterator first, iterator last)
{
    be_assert_recover(first.m_owner == this, "can't erase iterator that is not pointing on current vector", return first);
    be_assert_recover(last.m_owner == this, "can't erase iterator that is not pointing on current vector", return first);
    be_assert_recover(m_memory <= first.m_iterator && m_end > first.m_iterator, "first %p is not in the range of the vector [%p,%p)" | first.m_iterator | m_memory.data() | m_end, return first);
    be_assert_recover(m_memory <= last.m_iterator && m_end >= last.m_iterator, "last %p is not in the range of the vector [%p,%p)" | last.m_iterator | m_memory.data() | m_end, return first);
    be_assert_recover(first.m_iterator <= last.m_iterator,"first %p is not before last %p" | first.m_iterator | last.m_iterator, return first);

    for (pointer i = first.m_iterator; i != last.m_iterator; i = advance_ptr(i, 1))
    {
        i->~T();
    }
    pointer t = first.m_iterator;
    pointer t2 = last.m_iterator;
    for ( ; t2 != m_end; t = advance(t, 1), t2 = advance_ptr(t2, 1))
    {
        new((void*)t) T(*t2);
        t2->~T();
    }
    m_end = t;
    return first;
}


template< typename T >
typename vector<T>::reference                vector<T>::front()
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}

template< typename T >
typename vector<T>::reference                vector<T>::back()
{
    be_assert(!empty(), "getting back of empty vector");
    return *advance_ptr(m_end, -1);
}

template< typename T >
typename vector<T>::const_reference          vector<T>::front() const
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}

template< typename T >
typename vector<T>::const_reference          vector<T>::back() const
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}


template< typename T >
void                                                vector<T>::resize(size_type size)
{
    size_type s = distance(m_memory.data(), m_end);
    if (size > s)
    {
        reserve(size);
        pointer newend = advance_ptr(m_memory.data(), size);
        for (pointer t = m_end; t != newend; ++t)
            new((void*)t) T;
        m_end = newend;
    }
    else
    {
        pointer newend = advance_ptr(m_memory.data(), size);
        for (pointer t = newend; t != m_end; ++t)
            t->~T();
        m_end = newend;
    }
}

template< typename T >
void                                                vector<T>::clear()
{
    for (pointer t = m_memory.data(); t != m_end; ++t)
        t->~T();
    m_end = m_memory.data();
}

template< typename T >
void                                                vector<T>::reserve(size_type size)
{
    size_type capacity = distance(m_memory.data(), m_capacity);
    if (size > capacity)
    {
        size = size >> 1  | size;
        size = size >> 2  | size;
        size = size >> 4  | size;
        size = size >> 8  | size;
        size = size >> 16 | size;
        size = size >> (sizeof(size_type) == 64 ? 32 : 0) | size;
        size++;
        BugEngine::Allocator::Block<T> block(m_memory.arena(), size);
        pointer t = block;
        for (pointer t2 = m_memory; t2 != m_end; t = advance_ptr(t, 1), t2 = advance_ptr(t2, 1))
        {
            new((void*)t) T(*t2);
            t2->~T();
        }
        m_memory.swap(block);
        m_end = t;
        m_capacity = advance_ptr(m_memory.data(), size);
    }
}

}

/*****************************************************************************/
#endif
