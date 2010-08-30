/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_VECTOR_INL_
#define BE_MINITL_CONTAINER_INL_VECTOR_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>


namespace minitl
{

template< typename T, int ARENA >
template< typename POLICY >
class vector<T, ARENA>::base_iterator
#ifdef BE_ENABLE_DEBUG_ITERATORS
    :   intrusive_list< base_iterator<POLICY> >::item
#endif
{
    friend class vector<T, ARENA>;
private:
    const vector<T, ARENA>*     m_owner;
    typename POLICY::pointer    m_iterator;
private:
    base_iterator(const vector<T, ARENA>* owner, typename POLICY::pointer it);
public:
    base_iterator();
    ~base_iterator();
public:
    bool operator==(const base_iterator<POLICY>& other);
    bool operator!=(const base_iterator<POLICY>& other);

    base_iterator<POLICY> operator+(typename POLICY::difference_type offset)
    {
        return base_iterator<POLICY>(m_owner, POLICY::advance(m_iterator, offset));
    }
    base_iterator<POLICY> operator-(typename POLICY::difference_type offset)
    {
        return base_iterator<POLICY>(m_owner, POLICY::advance(m_iterator, -offset));
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

template< typename T, int ARENA >
template< typename POLICY >
vector<T, ARENA>::base_iterator<POLICY>::base_iterator()
:   m_owner(0)
,   m_iterator(0)
{
}

template< typename T, int ARENA >
template< typename POLICY >
vector<T, ARENA>::base_iterator<POLICY>::base_iterator(const vector<T, ARENA>* owner, typename POLICY::pointer it)
:   m_owner(owner)
,   m_iterator(it)
{
}

template< typename T, int ARENA >
template< typename POLICY >
vector<T, ARENA>::base_iterator<POLICY>::~base_iterator()
{
}

template< typename T, int ARENA >
template< typename POLICY >
bool vector<T, ARENA>::base_iterator<POLICY>::operator==(const base_iterator<POLICY>& other)
{
    return m_iterator == other.m_iterator;
}

template< typename T, int ARENA >
template< typename POLICY >
bool vector<T, ARENA>::base_iterator<POLICY>::operator!=(const base_iterator<POLICY>& other)
{
    return m_iterator != other.m_iterator;
}

template< typename T, int ARENA >
template< typename POLICY >
typename POLICY::pointer vector<T, ARENA>::base_iterator<POLICY>::operator->() const
{
    return m_iterator;
}

template< typename T, int ARENA >
template< typename POLICY >
typename POLICY::reference vector<T, ARENA>::base_iterator<POLICY>::operator*() const
{
    return *m_iterator;
}


template< typename T, int ARENA >
struct vector<T, ARENA>::iterator_policy
{
    typedef typename vector<T, ARENA>::pointer          pointer;
    typedef typename vector<T, ARENA>::reference        reference;
    typedef typename vector<T, ARENA>::size_type        size_type;
    typedef typename vector<T, ARENA>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
};

template< typename T, int ARENA >
struct vector<T, ARENA>::const_iterator_policy
{
    typedef typename vector<T, ARENA>::const_pointer    pointer;
    typedef typename vector<T, ARENA>::const_reference  reference;
    typedef typename vector<T, ARENA>::size_type        size_type;
    typedef typename vector<T, ARENA>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
};

template< typename T, int ARENA >
struct vector<T, ARENA>::reverse_iterator_policy
{
    typedef typename vector<T, ARENA>::pointer          pointer;
    typedef typename vector<T, ARENA>::reference        reference;
    typedef typename vector<T, ARENA>::size_type        size_type;
    typedef typename vector<T, ARENA>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
};

template< typename T, int ARENA >
struct vector<T, ARENA>::const_reverse_iterator_policy
{
    typedef typename vector<T, ARENA>::const_pointer    pointer;
    typedef typename vector<T, ARENA>::const_reference  reference;
    typedef typename vector<T, ARENA>::size_type        size_type;
    typedef typename vector<T, ARENA>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
};


template< typename T, int ARENA >
vector<T, ARENA>::vector()
:   m_memory(0)
,   m_end(m_memory)
,   m_capacity(m_memory)
{
}

template< typename T, int ARENA >
vector<T, ARENA>::~vector()
{
    for(const_pointer t = m_memory; t != m_end; t = advance(t, 1))
    {
        t->~T();
    }
}

template< typename T, int ARENA >
typename vector<T, ARENA>::iterator                 vector<T, ARENA>::begin()
{
    return iterator(this, m_memory);
}

template< typename T, int ARENA >
typename vector<T, ARENA>::iterator                 vector<T, ARENA>::end()
{
    return iterator(this, m_end);
}

template< typename T, int ARENA >
typename vector<T, ARENA>::const_iterator           vector<T, ARENA>::begin() const
{
    return const_iterator(this, m_memory);
}

template< typename T, int ARENA >
typename vector<T, ARENA>::const_iterator           vector<T, ARENA>::end() const
{
    return const_iterator(this, m_end);
}

template< typename T, int ARENA >
typename vector<T, ARENA>::reverse_iterator         vector<T, ARENA>::rbegin()
{
    return reverse_iterator(this, advance(m_end, -1));
}

template< typename T, int ARENA >
typename vector<T, ARENA>::reverse_iterator         vector<T, ARENA>::rend()
{
    return reverse_iterator(this, advance(m_memory.data(), -1));
}

template< typename T, int ARENA >
typename vector<T, ARENA>::const_reverse_iterator   vector<T, ARENA>::rbegin() const
{
    return const_reverse_iterator(this, advance(m_end, -1));
}

template< typename T, int ARENA >
typename vector<T, ARENA>::const_reverse_iterator   vector<T, ARENA>::rend() const
{
    return const_reverse_iterator(this, advance(m_memory.data(), -1));
}


template< typename T, int ARENA >
typename vector<T, ARENA>::size_type                vector<T, ARENA>::size() const
{
    return distance(m_memory.data(), m_end);
}

template< typename T, int ARENA >
bool                                                vector<T, ARENA>::empty() const
{
    return m_end == m_memory;
}


template< typename T, int ARENA >
typename vector<T, ARENA>::reference                vector<T, ARENA>::operator[](size_type i)
{
    return *advance(m_memory.data(), i);
}

template< typename T, int ARENA >
typename vector<T, ARENA>::const_reference          vector<T, ARENA>::operator[](size_type i) const
{
    return *advance(m_memory.data(), i);
}


template< typename T, int ARENA >
void                                                vector<T, ARENA>::push_back(const_reference r)
{
    grow(size() + 1);
    new(m_end) T(r);
    m_end = advance(m_end, 1);
}

template< typename T, int ARENA >
typename vector<T, ARENA>::iterator                 vector<T, ARENA>::erase(iterator it)
{
    return erase(it, it+1);
}

template< typename T, int ARENA >
typename vector<T, ARENA>::iterator                 vector<T, ARENA>::erase(iterator first, iterator last)
{
    be_assert_recover(first.m_owner == this, "can't erase iterator that is not pointing on current vector", return first);
    be_assert_recover(last.m_owner == this, "can't erase iterator that is not pointing on current vector", return first);
    be_assert_recover(m_memory <= first.m_iterator && m_end > first.m_iterator, "first %p is not in the range of the vector [%p,%p)" | first.m_iterator | m_memory.data() | m_end, return first);
    be_assert_recover(m_memory <= last.m_iterator && m_end >= last.m_iterator, "last %p is not in the range of the vector [%p,%p)" | last.m_iterator | m_memory.data() | m_end, return first);
    be_assert_recover(first.m_iterator <= last.m_iterator,"first %p is not before last %p" | first.m_iterator | last.m_iterator, return first);

    for(pointer t = first.m_iterator; t != last.m_iterator; t = advance(t, 1))
    {
        t->~T();
    }
    pointer t = first.m_iterator;
    pointer t2 = last.m_iterator;
    for( ; t2 != m_end; t = advance(t, 1), t2 = advance(t2, 1))
    {
        new(t) T(*t2);
        t2->~T();
    }
    m_end = t;
    return first;
}


template< typename T, int ARENA >
typename vector<T, ARENA>::reference                vector<T, ARENA>::front()
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}

template< typename T, int ARENA >
typename vector<T, ARENA>::reference                vector<T, ARENA>::back()
{
    be_assert(!empty(), "getting back of empty vector");
    return *advance(m_end, -1);
}

template< typename T, int ARENA >
typename vector<T, ARENA>::const_reference          vector<T, ARENA>::front() const
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}

template< typename T, int ARENA >
typename vector<T, ARENA>::const_reference          vector<T, ARENA>::back() const
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}


template< typename T, int ARENA >
void                                                vector<T, ARENA>::resize(size_type size)
{
    size_type s = distance(m_memory.data(), m_end);
    if(size > s)
    {
        grow(size);
        pointer newend = advance(m_memory.data(), size);
        for(pointer t = m_end; t != newend; ++t)
            new(t) T;
        m_end = newend;
    }
    else
    {
        pointer newend = advance(m_memory.data(), size);
        for(pointer t = newend; t != m_end; ++t)
            t->~T();
        m_end = newend;
    }
}

template< typename T, int ARENA >
void                                                vector<T, ARENA>::clear()
{
    for(pointer t = m_memory.data(); t != m_end; ++t)
        t->~T();
    m_end = m_memory.data();
}

template< typename T, int ARENA >
void                                                vector<T, ARENA>::grow(size_type size)
{
    size_type capacity = distance(m_memory.data(), m_capacity);
    if(size > capacity)
    {
        size = size >> 1  | size;
        size = size >> 2  | size;
        size = size >> 4  | size;
        size = size >> 8  | size;
        size = size >> 16 | size;
    #ifdef BE_64
        size = size >> 32 | size;
    #endif
        size++;
        typename BugEngine::Memory<ARENA>::template Block<T> block(size);
        pointer t = block;
        for(pointer t2 = m_memory; t2 != m_end; t = advance(t, 1), t2 = advance(t2, 1))
        {
            new(t) T(*t2);
            t2->~T();
        }
        m_memory.swap(block);
        m_end = t;
        m_capacity = advance(m_memory.data(), size);
    }
}

}

/*****************************************************************************/
#endif
