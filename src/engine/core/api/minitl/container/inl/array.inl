/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_ARRAY_INL_
#define BE_MINITL_CONTAINER_INL_ARRAY_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>

namespace minitl
{

template< typename T, size_t SIZE >
template< typename POLICY >
class array<T,SIZE>::base_iterator
    :   public random_access_iterator<T, typename array<T,SIZE>::difference_type>
{
    friend class array<T,SIZE>;
public:
    typedef          random_access_iterator_tag         iterator_category;
    typedef typename POLICY::value_type                 value_type;
    typedef typename POLICY::pointer                    pointer;
    typedef typename POLICY::reference                  reference;
    typedef typename POLICY::size_type                  size_type;
    typedef typename POLICY::difference_type            difference_type;
private:
    const array<T,SIZE>*        m_owner;
    typename POLICY::pointer    m_iterator;
private:
    base_iterator(const array<T,SIZE>* owner, typename POLICY::pointer it);
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

template< typename T, size_t SIZE >
template< typename POLICY >
array<T,SIZE>::base_iterator<POLICY>::base_iterator()
:   m_owner(0)
,   m_iterator(0)
{
}

template< typename T, size_t SIZE >
template< typename POLICY >
array<T,SIZE>::base_iterator<POLICY>::base_iterator(const array<T,SIZE>* owner, typename POLICY::pointer it)
:   m_owner(owner)
,   m_iterator(it)
{
}

template< typename T, size_t SIZE >
template< typename POLICY >
template< typename OTHERPOLICY >
array<T,SIZE>::base_iterator<POLICY>::base_iterator(const base_iterator<OTHERPOLICY>& other)
:   m_owner(other.m_owner)
,   m_iterator(other.m_iterator)
{
}

template< typename T, size_t SIZE >
template< typename POLICY >
array<T,SIZE>::base_iterator<POLICY>::~base_iterator()
{
}

template< typename T, size_t SIZE >
template< typename POLICY >
bool array<T,SIZE>::base_iterator<POLICY>::operator==(const base_iterator<POLICY>& other)
{
    return m_iterator == other.m_iterator;
}

template< typename T, size_t SIZE >
template< typename POLICY >
bool array<T,SIZE>::base_iterator<POLICY>::operator!=(const base_iterator<POLICY>& other)
{
    return m_iterator != other.m_iterator;
}

template< typename T, size_t SIZE >
template< typename POLICY >
typename POLICY::pointer array<T,SIZE>::base_iterator<POLICY>::operator->() const
{
    return m_iterator;
}

template< typename T, size_t SIZE >
template< typename POLICY >
typename POLICY::reference array<T,SIZE>::base_iterator<POLICY>::operator*() const
{
    return *m_iterator;
}


template< typename T, size_t SIZE >
struct array<T,SIZE>::iterator_policy
{
    typedef typename array<T,SIZE>::value_type       value_type;
    typedef typename array<T,SIZE>::pointer          pointer;
    typedef typename array<T,SIZE>::reference        reference;
    typedef typename array<T,SIZE>::size_type        size_type;
    typedef typename array<T,SIZE>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
};

template< typename T, size_t SIZE >
struct array<T,SIZE>::const_iterator_policy
{
    typedef typename array<T,SIZE>::value_type const value_type;
    typedef typename array<T,SIZE>::const_pointer    pointer;
    typedef typename array<T,SIZE>::const_reference  reference;
    typedef typename array<T,SIZE>::size_type        size_type;
    typedef typename array<T,SIZE>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
};

template< typename T, size_t SIZE >
struct array<T,SIZE>::reverse_iterator_policy
{
    typedef typename array<T,SIZE>::value_type       value_type;
    typedef typename array<T,SIZE>::pointer          pointer;
    typedef typename array<T,SIZE>::reference        reference;
    typedef typename array<T,SIZE>::size_type        size_type;
    typedef typename array<T,SIZE>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
};

template< typename T, size_t SIZE >
struct array<T,SIZE>::const_reverse_iterator_policy
{
    typedef typename array<T,SIZE>::value_type const value_type;
    typedef typename array<T,SIZE>::const_pointer    pointer;
    typedef typename array<T,SIZE>::const_reference  reference;
    typedef typename array<T,SIZE>::size_type        size_type;
    typedef typename array<T,SIZE>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
};


template< typename T, size_t SIZE >
array<T,SIZE>::array(size_type count)
:   m_end(reinterpret_cast<pointer>(m_memory))
{
    for(size_type s = 0; s < count; ++s)
        push_back(T());
}

template< typename T, size_t SIZE >
array<T,SIZE>::array(const array<T, SIZE>& other)
    :   m_end(reinterpret_cast<pointer>(m_memory))
{
    push_back(other.begin(), other.end());
}

template< typename T, size_t SIZE >
template< typename T1, size_t SIZE1 >
array<T,SIZE>::array(const array<T1, SIZE1>& other)
    :   m_end(reinterpret_cast<pointer>(m_memory))
{
    push_back(other.begin(), other.end());
}

template< typename T, size_t SIZE >
template< typename ITERATOR >
array<T,SIZE>::array(ITERATOR first, ITERATOR last)
    :   m_end(reinterpret_cast<pointer>(m_memory))
{
    push_back(first, last);
}

template< typename T, size_t SIZE >
array<T,SIZE>& array<T,SIZE>::operator=(const array<T,SIZE>& other)
{
    clear();
    push_back(other.begin(), other.end());
    return *this;
}


template< typename T, size_t SIZE >
template< typename T1, size_t SIZE1 >
array<T,SIZE>& array<T,SIZE>::operator=(const array<T1,SIZE1>& other)
{
    clear();
    push_back(other.begin(), other.end());
    return *this;
}

template< typename T, size_t SIZE >
array<T,SIZE>::~array()
{
    for (const_pointer t = (const_pointer)m_memory; t != m_end; t = advance(t, 1))
    {
        t->~T();
    }
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::iterator                 array<T,SIZE>::begin()
{
    return iterator(this, (pointer)m_memory);
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::iterator                 array<T,SIZE>::end()
{
    return iterator(this, m_end);
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::const_iterator           array<T,SIZE>::begin() const
{
    return const_iterator(this, (const_pointer)m_memory);
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::const_iterator           array<T,SIZE>::end() const
{
    return const_iterator(this, m_end);
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::reverse_iterator         array<T,SIZE>::rbegin()
{
    return reverse_iterator(this, advance(m_end, -1));
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::reverse_iterator         array<T,SIZE>::rend()
{
    return reverse_iterator(this, advance((pointer)m_memory, -1));
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::const_reverse_iterator   array<T,SIZE>::rbegin() const
{
    return const_reverse_iterator(this, advance(m_end, -1));
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::const_reverse_iterator   array<T,SIZE>::rend() const
{
    return const_reverse_iterator(this, advance((const_pointer)m_memory, -1));
}


template< typename T, size_t SIZE >
typename array<T,SIZE>::size_type                array<T,SIZE>::size() const
{
    return distance((const_pointer)m_memory, m_end);
}

template< typename T, size_t SIZE >
bool                                                array<T,SIZE>::empty() const
{
    return m_end == (const_pointer)m_memory;
}


template< typename T, size_t SIZE >
typename array<T,SIZE>::reference                array<T,SIZE>::operator[](size_type i)
{
    return *advance((pointer)m_memory, i);
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::const_reference          array<T,SIZE>::operator[](size_type i) const
{
    return *advance((const_pointer)m_memory, i);
}


template< typename T, size_t SIZE >
void                                                array<T,SIZE>::push_back(const_reference r)
{
    //reserve(size() + 1);
    new((void*)m_end) T(r);
    m_end = advance(m_end, 1);
}

template< typename T, size_t SIZE >
template< typename ITERATOR >
void                                                array<T,SIZE>::push_back(ITERATOR first, ITERATOR last)
{
    //size_type count = minitl::distance(first, last);
    //reserve(size() + count);
    while (first != last)
    {
        new((void*)m_end) T(*first);
        m_end = advance(m_end, 1);
        ++first;
    }
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::iterator                 array<T,SIZE>::erase(iterator it)
{
    return erase(it, it+1);
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::iterator                 array<T,SIZE>::erase(iterator first, iterator last)
{
    be_assert_recover(first.m_owner == this, "can't erase iterator that is not pointing on current vector", return first);
    be_assert_recover(last.m_owner == this, "can't erase iterator that is not pointing on current vector", return first);
    be_assert_recover((const_pointer)m_memory <= first.m_iterator && m_end > first.m_iterator, "first %p is not in the range of the vector [%p,%p)" | first.m_iterator | (const_pointer)m_memory | m_end, return first);
    be_assert_recover((const_pointer)m_memory <= last.m_iterator && m_end >= last.m_iterator, "last %p is not in the range of the vector [%p,%p)" | last.m_iterator | (const_pointer)m_memory | m_end, return first);
    be_assert_recover(first.m_iterator <= last.m_iterator,"first %p is not before last %p" | first.m_iterator | last.m_iterator, return first);

    for (pointer t = first.m_iterator; t != last.m_iterator; t = advance(t, 1))
    {
        t->~T();
    }
    pointer t = first.m_iterator;
    pointer t2 = last.m_iterator;
    for ( ; t2 != m_end; t = advance(t, 1), t2 = advance(t2, 1))
    {
        new((void*)t) T(*t2);
        t2->~T();
    }
    m_end = t;
    return first;
}


template< typename T, size_t SIZE >
typename array<T,SIZE>::reference                array<T,SIZE>::front()
{
    be_assert(!empty(), "getting front of empty vector");
    return *(pointer)m_memory;
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::reference                array<T,SIZE>::back()
{
    be_assert(!empty(), "getting back of empty vector");
    return *advance(m_end, -1);
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::const_reference          array<T,SIZE>::front() const
{
    be_assert(!empty(), "getting front of empty vector");
    return *(const_pointer)m_memory;
}

template< typename T, size_t SIZE >
typename array<T,SIZE>::const_reference          array<T,SIZE>::back() const
{
    be_assert(!empty(), "getting front of empty vector");
    return *(const_pointer)m_memory;
}


template< typename T, size_t SIZE >
void                                                array<T, SIZE>::clear()
{
    for (pointer t = (pointer)m_memory; t != m_end; ++t)
        t->~T();
    m_end = (pointer)m_memory;
}

}

/*****************************************************************************/
#endif
