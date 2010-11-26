/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_HASHMAP_INL_
#define BE_MINITL_CONTAINER_INL_HASHMAP_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>

namespace minitl
{

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
class hashmap<Key, Value, Arena, Hash>::base_iterator
    :   public random_access_iterator<typename POLICY::value_type, typename hashmap<Key, Value, Arena, Hash>::difference_type>
{
    friend class hashmap<Key, Value, Arena, Hash>;
public:
    typedef          random_access_iterator_tag         iterator_category;
    typedef typename POLICY::value_type                 value_type;
    typedef typename POLICY::pointer                    pointer;
    typedef typename POLICY::reference                  reference;
    typedef typename POLICY::size_type                  size_type;
    typedef typename POLICY::difference_type            difference_type;
private:
    const hashmap<Key, Value, Arena, Hash>* m_owner;
    typename POLICY::pointer                m_iterator;
private:
    base_iterator(const hashmap<Key, Value, Arena, Hash>* owner, typename POLICY::pointer it);
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

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
hashmap<Key, Value, Arena, Hash>::base_iterator<POLICY>::base_iterator()
:   m_owner(0)
,   m_iterator(0)
{
}

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
hashmap<Key, Value, Arena, Hash>::base_iterator<POLICY>::base_iterator(const hashmap<Key, Value, Arena, Hash>* owner, typename POLICY::pointer it)
:   m_owner(owner)
,   m_iterator(it)
{
}

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
template< typename OTHERPOLICY >
hashmap<Key, Value, Arena, Hash>::base_iterator<POLICY>::base_iterator(const base_iterator<OTHERPOLICY>& other)
:   m_owner(other.m_owner)
,   m_iterator(other.m_iterator)
{
}

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
hashmap<Key, Value, Arena, Hash>::base_iterator<POLICY>::~base_iterator()
{
}

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
bool hashmap<Key, Value, Arena, Hash>::base_iterator<POLICY>::operator==(const base_iterator<POLICY>& other)
{
    return m_iterator == other.m_iterator;
}

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
bool hashmap<Key, Value, Arena, Hash>::base_iterator<POLICY>::operator!=(const base_iterator<POLICY>& other)
{
    return m_iterator != other.m_iterator;
}

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
typename POLICY::pointer hashmap<Key, Value, Arena, Hash>::base_iterator<POLICY>::operator->() const
{
    return m_iterator;
}

template< typename Key, typename Value, int Arena, typename Hash >
template< typename POLICY >
typename POLICY::reference hashmap<Key, Value, Arena, Hash>::base_iterator<POLICY>::operator*() const
{
    return *m_iterator;
}


template< typename Key, typename Value, int Arena, typename Hash >
struct hashmap<Key, Value, Arena, Hash>::iterator_policy
{
    typedef typename hashmap<Key, Value, Arena, Hash>::value_type       value_type;
    typedef typename hashmap<Key, Value, Arena, Hash>::pointer          pointer;
    typedef typename hashmap<Key, Value, Arena, Hash>::reference        reference;
    typedef typename hashmap<Key, Value, Arena, Hash>::size_type        size_type;
    typedef typename hashmap<Key, Value, Arena, Hash>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
};

template< typename Key, typename Value, int Arena, typename Hash >
struct hashmap<Key, Value, Arena, Hash>::const_iterator_policy
{
    typedef typename hashmap<Key, Value, Arena, Hash>::value_type const value_type;
    typedef typename hashmap<Key, Value, Arena, Hash>::const_pointer    pointer;
    typedef typename hashmap<Key, Value, Arena, Hash>::const_reference  reference;
    typedef typename hashmap<Key, Value, Arena, Hash>::size_type        size_type;
    typedef typename hashmap<Key, Value, Arena, Hash>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
};

template< typename Key, typename Value, int Arena, typename Hash >
struct hashmap<Key, Value, Arena, Hash>::reverse_iterator_policy
{
    typedef typename hashmap<Key, Value, Arena, Hash>::value_type       value_type;
    typedef typename hashmap<Key, Value, Arena, Hash>::pointer          pointer;
    typedef typename hashmap<Key, Value, Arena, Hash>::reference        reference;
    typedef typename hashmap<Key, Value, Arena, Hash>::size_type        size_type;
    typedef typename hashmap<Key, Value, Arena, Hash>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
};

template< typename Key, typename Value, int Arena, typename Hash >
struct hashmap<Key, Value, Arena, Hash>::const_reverse_iterator_policy
{
    typedef typename hashmap<Key, Value, Arena, Hash>::value_type const value_type;
    typedef typename hashmap<Key, Value, Arena, Hash>::const_pointer    pointer;
    typedef typename hashmap<Key, Value, Arena, Hash>::const_reference  reference;
    typedef typename hashmap<Key, Value, Arena, Hash>::size_type        size_type;
    typedef typename hashmap<Key, Value, Arena, Hash>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
};



template< typename Key, typename Value, int Arena, typename Hash >
hashmap< Key, Value, Arena, Hash >::hashmap(size_type reserved)
:   m_objects(reserved)
,   m_hashes(reserved)
,   m_size(0)
,   m_capacity(reserved)
{
}

template< typename Key, typename Value, int Arena, typename Hash >
void hashmap< Key, Value, Arena, Hash >::reserve(size_type reserved)
{
    if(reserved <= m_capacity)
        return;
    BugEngine::Memory<Arena>::Block<value_type*> newhashes(reserved);
    minitl::pool<Arena, value_type*> newobjects(reserved);
    for(value_type** it = m_hashes.data; it < m_hashes.data+m_capacity; ++it)
    {
        value_type* entry = *it;
        newobjects.allocate(*entry);
        m_objects.release(entry);
    }
}




}

/*****************************************************************************/
#endif
