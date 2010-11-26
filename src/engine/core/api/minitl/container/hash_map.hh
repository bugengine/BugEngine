/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_HASHMAP_HH_
#define BE_MINITL_CONTAINER_HASHMAP_HH_
/*****************************************************************************/
#include    <minitl/container/pair.hh>
#include    <minitl/container/traits.hh>
#include    <minitl/memory/pool.hh>

namespace minitl
{

template< typename T >
struct hash;

template< typename Key, typename Value, int Arena, typename Hash = minitl::hash<Key> >
class hashmap
{
private:
    template< typename POLICY > class base_iterator;
    struct iterator_policy;
    struct const_iterator_policy;
    struct reverse_iterator_policy;
    struct const_reverse_iterator_policy;
public:
    typedef minitl::pair<const Key, Value>          value_type;
    typedef minitl::pair<const Key, Value>*         pointer;
    typedef minitl::pair<const Key, Value>&         reference;
    typedef const minitl::pair<const Key, Value>*   const_pointer;
    typedef const minitl::pair<const Key, Value>&   const_reference;
    typedef minitl::size_type                       size_type;
    typedef minitl::difference_type                 difference_type;
public:
    typedef base_iterator<iterator_policy>                  iterator;
    typedef base_iterator<const_iterator_policy>            const_iterator;
    typedef base_iterator<reverse_iterator_policy>          reverse_iterator;
    typedef base_iterator<const_reverse_iterator_policy>    const_reverse_iterator;
private:
    minitl::pool<Arena, value_type>                         m_objects;
    typename BugEngine::Memory<Arena>::Block<value_type*>   m_hashes;
    size_type                                               m_size;
    size_type                                               m_capacity;
public:
    hashmap(size_type reserved = 0);
    ~hashmap();

    void                            reserve(size_type size);

    iterator                        begin();
    iterator                        end();
    const_iterator                  begin() const;
    const_iterator                  end() const;
    reverse_iterator                rbegin();
    reverse_iterator                rend();
    const_reverse_iterator          rbegin() const;
    const_reverse_iterator          rend() const;

    size_type                       size() const;
    bool                            empty() const;

    reference                       operator[](const Key& key);
    const_reference                 operator[](const Key& key) const;

    iterator                        find(const Key& key);
    const_iterator                  find(const Key& key) const;

    void                            erase(iterator it);

    minitl::pair<iterator, bool>    insert(const Key& k, const Value& value);
    minitl::pair<iterator, bool>    insert(const minitl::pair<const Key, Value>& v);
};

}

#include    <minitl/container/inl/hash_map.inl>

/*****************************************************************************/
#endif
