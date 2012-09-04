/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_HASH_MAP_HH_
#define BE_MINITL_HASH_MAP_HH_
/*****************************************************************************/
#include    <minitl/pair.hh>
#include    <minitl/traits.hh>
#include    <minitl/iterator.hh>
#include    <minitl/intrusive_list.hh>
#include    <minitl/pool.hh>

namespace minitl
{

template< typename T >
struct hash;

template< typename Key, typename Value, typename Hash = hash<Key> >
class hashmap
{
private:
    template< typename POLICY >
    struct iterator_base;

    struct iterator_policy;
    struct reverse_iterator_policy;
    struct const_iterator_policy;
    struct const_reverse_iterator_policy;
public:
    typedef iterator_base<iterator_policy>                  iterator;
    typedef iterator_base<reverse_iterator_policy>          reverse_iterator;
    typedef iterator_base<const_iterator_policy>            const_iterator;
    typedef iterator_base<const_reverse_iterator_policy>    const_reverse_iterator;

    typedef minitl::pair<const Key, Value>          value_type;
    typedef minitl::pair<const Key, Value>&         reference;
    typedef const minitl::pair<const Key, Value>&   const_reference;
private:
    struct empty_item : public minitl::intrusive_list<empty_item>::item
    {
    };
    struct item : public empty_item
    {
        value_type value;
        item(const value_type& value) : value(value) {}
    };
    typedef typename intrusive_list<empty_item>::iterator list_iterator;
    typedef pair<empty_item, list_iterator> index_item;
private:
    pool<item>                      m_itemPool;
    intrusive_list<empty_item>      m_items;
    Allocator::Block<index_item>    m_index;
    size_type                       m_count;
private:
    void buildIndex();
public:
    hashmap(Allocator& allocator, size_type reserved = 0);
    ~hashmap();
    hashmap(const hashmap& other);
    hashmap& operator=(const hashmap& other);

    void                    reserve(size_type size);

    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;

    size_type               size() const;
    bool                    empty() const;

    reference               operator[](const Key& key);
    const_reference         operator[](const Key& key) const;

    iterator                find(const Key& key);
    const_iterator          find(const Key& key) const;

    iterator                erase(iterator it);

    pair<iterator, bool>    insert(const Key& k, const Value& value);
    pair<iterator, bool>    insert(const minitl::pair<const Key, Value>& v);
};

}

#include    <minitl/inl/hash_map.inl>

/*****************************************************************************/
#endif
