/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_INL_HASH_MAP_INL_
#define BE_MINITL_INL_HASH_MAP_INL_
/*****************************************************************************/

namespace minitl
{

template< typename Key, typename Value, typename Hash >
hashmap<Key, Value, Hash>::hashmap(minitl::Allocator& allocator, size_type reserved)
{
}

template< typename Key, typename Value, typename Hash >
hashmap<Key, Value, Hash>::~hashmap()
{
}

template< typename Key, typename Value, typename Hash >
void hashmap<Key, Value, Hash>::reserve(size_type size)
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::iterator hashmap<Key, Value, Hash>::begin()
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::iterator hashmap<Key, Value, Hash>::end()
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_iterator hashmap<Key, Value, Hash>::begin() const
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_iterator hashmap<Key, Value, Hash>::end() const
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::reverse_iterator hashmap<Key, Value, Hash>::rbegin()
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::reverse_iterator hashmap<Key, Value, Hash>::rend()
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_reverse_iterator hashmap<Key, Value, Hash>::rbegin() const
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_reverse_iterator hashmap<Key, Value, Hash>::rend() const
{
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
typename hashmap<Key, Value, Hash>::reference hashmap<Key, Value, Hash>::operator[](const Key& key)
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_reference hashmap<Key, Value, Hash>::operator[](const Key& key) const
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::iterator hashmap<Key, Value, Hash>::find(const Key& key)
{
}

template< typename Key, typename Value, typename Hash >
typename hashmap<Key, Value, Hash>::const_iterator hashmap<Key, Value, Hash>::find(const Key& key) const
{
}

template< typename Key, typename Value, typename Hash >
void hashmap<Key, Value, Hash>::erase(iterator it)
{
}

template< typename Key, typename Value, typename Hash >
minitl::pair<typename hashmap<Key, Value, Hash>::iterator, bool> hashmap<Key, Value, Hash>::insert(const Key& k, const Value& value)
{
}

template< typename Key, typename Value, typename Hash >
minitl::pair<typename hashmap<Key, Value, Hash>::iterator, bool> hashmap<Key, Value, Hash>::insert(const minitl::pair<const Key, Value>& v)
{
}

}

/*****************************************************************************/
#endif
