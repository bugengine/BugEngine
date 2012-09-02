/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INL_HASH_MAP_INL_
#define BE_MINITL_INL_HASH_MAP_INL_
/*****************************************************************************/

namespace minitl
{

template< typename Key, typename Value, typename Hash >
template< typename POLICY >
struct hashmap<Key, Value, Hash>::iterator_base
{
    bool operator==(const iterator_base& /*other*/) { return true; }
    bool operator!=(const iterator_base& /*other*/) { return true; }
    typename POLICY::reference operator*() const { return *(typename POLICY::value_type*)0; }
    typename POLICY::pointer operator->() const  { return (typename POLICY::pointer)0; }
};

template< typename Key, typename Value, typename Hash >
struct hashmap<Key, Value, Hash>::iterator_policy
{
    typedef typename hashmap<Key, Value, Hash>::value_type  value_type;
    typedef typename hashmap<Key, Value, Hash>::value_type& reference;
    typedef typename hashmap<Key, Value, Hash>::value_type* pointer;
};
template< typename Key, typename Value, typename Hash >
struct hashmap<Key, Value, Hash>::const_iterator_policy
{
    typedef const typename hashmap<Key, Value, Hash>::value_type  value_type;
    typedef const typename hashmap<Key, Value, Hash>::value_type& reference;
    typedef const typename hashmap<Key, Value, Hash>::value_type* pointer;
};
template< typename Key, typename Value, typename Hash >
struct hashmap<Key, Value, Hash>::reverse_iterator_policy
{
    typedef typename hashmap<Key, Value, Hash>::value_type  value_type;
    typedef typename hashmap<Key, Value, Hash>::value_type& reference;
    typedef typename hashmap<Key, Value, Hash>::value_type* pointer;
};
template< typename Key, typename Value, typename Hash >
struct hashmap<Key, Value, Hash>::const_reverse_iterator_policy
{
    typedef const typename hashmap<Key, Value, Hash>::value_type  value_type;
    typedef const typename hashmap<Key, Value, Hash>::value_type& reference;
    typedef const typename hashmap<Key, Value, Hash>::value_type* pointer;
};

template< typename Key, typename Value, typename Hash >
hashmap<Key, Value, Hash>::hashmap(minitl::Allocator& /*allocator*/, size_type /*reserved*/)
{
}

template< typename Key, typename Value, typename Hash >
hashmap<Key, Value, Hash>::~hashmap()
{
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
void hashmap<Key, Value, Hash>::erase(iterator /*it*/)
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
