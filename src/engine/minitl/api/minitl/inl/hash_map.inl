/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INL_HASH_MAP_INL_
#define BE_MINITL_INL_HASH_MAP_INL_
/*****************************************************************************/
#include    <minitl/algorithm.hh>

namespace minitl
{

template< typename Key, typename Value, typename Hash >
template< typename POLICY >
struct hashmap<Key, Value, Hash>::iterator_base
{
private:
    typedef typename hashmap<Key, Value, Hash>::list_iterator   iterator;
private:
    hashmap<Key, Value, Hash>*  m_owner;
    iterator                    m_current;
public:
    iterator_base()
    {
    }
    iterator_base(hashmap<Key, Value, Hash>& owner, iterator l)
        :   m_owner(&owner)
        ,   m_current(l)
    {
    }
    bool operator==(const iterator_base& /*other*/) { return true; }
    bool operator!=(const iterator_base& /*other*/) { return true; }
    typename POLICY::reference operator*() const { return *(typename POLICY::value_type*)0; }
    typename POLICY::pointer operator->() const  { return (typename POLICY::pointer)0; }

    iterator_base& operator++()
    {
        m_current = POLICY::next(m_current);
        return *this;
    }
    iterator_base& operator++(int)
    {
        iterator_base copy = *this;
        m_current = POLICY::next(m_current);
        return copy;
    }
    iterator_base& operator--()
    {
        m_current = POLICY::previous(m_current);
        return *this;
    }
    iterator_base& operator--(int)
    {
        iterator_base copy = *this;
        m_current = POLICY::previous(m_current);
        return copy;
    }
};

template< typename Key, typename Value, typename Hash >
struct hashmap<Key, Value, Hash>::iterator_policy
{
    typedef typename hashmap<Key, Value, Hash>::value_type  value_type;
    typedef typename hashmap<Key, Value, Hash>::value_type& reference;
    typedef typename hashmap<Key, Value, Hash>::value_type* pointer;
    static typename hashmap<Key, Value, Hash>::list_iterator next(typename hashmap<Key, Value, Hash>::list_iterator it)
    {
        return ++it;
    }
    static typename hashmap<Key, Value, Hash>::list_iterator previous(typename hashmap<Key, Value, Hash>::list_iterator it)
    {
        return --it;
    }
};
template< typename Key, typename Value, typename Hash >
struct hashmap<Key, Value, Hash>::const_iterator_policy
{
    typedef const typename hashmap<Key, Value, Hash>::value_type  value_type;
    typedef const typename hashmap<Key, Value, Hash>::value_type& reference;
    typedef const typename hashmap<Key, Value, Hash>::value_type* pointer;
    static typename hashmap<Key, Value, Hash>::list_iterator next(typename hashmap<Key, Value, Hash>::list_iterator it)
    {
        return ++it;
    }
    static typename hashmap<Key, Value, Hash>::list_iterator previous(typename hashmap<Key, Value, Hash>::list_iterator it)
    {
        return --it;
    }
};
template< typename Key, typename Value, typename Hash >
struct hashmap<Key, Value, Hash>::reverse_iterator_policy
{
    typedef typename hashmap<Key, Value, Hash>::value_type  value_type;
    typedef typename hashmap<Key, Value, Hash>::value_type& reference;
    typedef typename hashmap<Key, Value, Hash>::value_type* pointer;
    static typename hashmap<Key, Value, Hash>::list_iterator next(typename hashmap<Key, Value, Hash>::list_iterator it)
    {
        return --it;
    }
    static typename hashmap<Key, Value, Hash>::list_iterator previous(typename hashmap<Key, Value, Hash>::list_iterator it)
    {
        return ++it;
    }
};
template< typename Key, typename Value, typename Hash >
struct hashmap<Key, Value, Hash>::const_reverse_iterator_policy
{
    typedef const typename hashmap<Key, Value, Hash>::value_type  value_type;
    typedef const typename hashmap<Key, Value, Hash>::value_type& reference;
    typedef const typename hashmap<Key, Value, Hash>::value_type* pointer;
    static typename hashmap<Key, Value, Hash>::list_iterator next(typename hashmap<Key, Value, Hash>::list_iterator it)
    {
        return --it;
    }
    static typename hashmap<Key, Value, Hash>::list_iterator previous(typename hashmap<Key, Value, Hash>::list_iterator it)
    {
        return ++it;
    }
};


template< typename Key, typename Value, typename Hash >
hashmap<Key, Value, Hash>::hashmap(minitl::Allocator& allocator, size_type reserved)
    :   m_itemPool(allocator, minitl::max(nextPowerOf2(reserved), size_type(8)))
    ,   m_items()
    ,   m_index(allocator, 1+minitl::max(nextPowerOf2(reserved), size_type(8)))
    ,   m_count(0)
{
    buildIndex();
}

template< typename Key, typename Value, typename Hash >
hashmap<Key, Value, Hash>::hashmap(const hashmap& other)
    :   m_itemPool(other.m_index.arena(), other.m_index.count()-1)
    ,   m_items()
    ,   m_index(other.m_index.arena(), other.m_index.count())
    ,   m_count(0)
{
    buildIndex();
}

template< typename Key, typename Value, typename Hash >
hashmap<Key, Value, Hash>::~hashmap()
{
    for (index_item* it = m_index.begin(); it != m_index.end()-1; /*nothing*/)
    {
        list_iterator object = it->second;
        object++;
        while (object != (it+1)->second)
        {
            list_iterator itemToDelete = object++;
            m_itemPool.release(&static_cast<item&>(*itemToDelete));
        }
        index_item* indexToDelete = it++;
        indexToDelete->~index_item();
    }
    (m_index.end()-1)->~index_item();
}

template< typename Key, typename Value, typename Hash >
void hashmap<Key, Value, Hash>::buildIndex()
{
    list_iterator current = m_items.begin();
    for (index_item* it = m_index.begin(); it != m_index.end(); ++it)
    {
        new (it) index_item;
        current = it->second = m_items.insert(current, it->first);
    }
}

template< typename Key, typename Value, typename Hash >
hashmap<Key, Value, Hash>& hashmap<Key, Value, Hash>::operator=(const hashmap& other)
{
    be_forceuse(other);
    return *this;
}

template< typename Key, typename Value, typename Hash >
void hashmap<Key, Value, Hash>::reserve(size_type /*size*/)
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::iterator hashmap<Key, Value, Hash>::begin()
{
    return iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::iterator hashmap<Key, Value, Hash>::end()
{
    return iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_iterator hashmap<Key, Value, Hash>::begin() const
{
    return const_iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_iterator hashmap<Key, Value, Hash>::end() const
{
    return const_iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::reverse_iterator hashmap<Key, Value, Hash>::rbegin()
{
    return reverse_iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::reverse_iterator hashmap<Key, Value, Hash>::rend()
{
    return reverse_iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_reverse_iterator hashmap<Key, Value, Hash>::rbegin() const
{
    return const_reverse_iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_reverse_iterator hashmap<Key, Value, Hash>::rend() const
{
    return const_reverse_iterator();
}

template< typename Key, typename Value, typename Hash >
size_type hashmap<Key, Value, Hash>::size() const
{
    return 0;
}

template< typename Key, typename Value, typename Hash >
bool hashmap<Key, Value, Hash>::empty() const
{
    return true;
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::reference hashmap<Key, Value, Hash>::operator[](const Key& /*key*/)
{
    return *(hashmap<Key, Value, Hash>::value_type*)0;
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_reference hashmap<Key, Value, Hash>::operator[](const Key& /*key*/) const
{
    return *(const hashmap<Key, Value, Hash>::value_type*)0;
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::iterator hashmap<Key, Value, Hash>::find(const Key& /*key*/)
{
    return iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_iterator hashmap<Key, Value, Hash>::find(const Key& /*key*/) const
{
    return const_iterator();
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::iterator hashmap<Key, Value, Hash>::erase(iterator /*it*/)
{
}

template< typename Key, typename Value, typename Hash >
minitl::pair<typename hashmap<Key, Value, Hash>::iterator, bool> hashmap<Key, Value, Hash>::insert(const Key& /*k*/, const Value& /*value*/)
{
    return minitl::make_pair(iterator(), false);
}

template< typename Key, typename Value, typename Hash >
minitl::pair<typename hashmap<Key, Value, Hash>::iterator, bool> hashmap<Key, Value, Hash>::insert(const minitl::pair<const Key, Value>& /*v*/)
{
    return minitl::make_pair(iterator(), false);
}

}

/*****************************************************************************/
#endif
