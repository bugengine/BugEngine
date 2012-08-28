/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_HASH_MAP_HH_
#define BE_MINITL_HASH_MAP_HH_
/*****************************************************************************/
#include    <minitl/pair.hh>
#include    <minitl/traits.hh>
#include    <minitl/iterator.hh>

namespace minitl
{

template< typename T >
struct hash;

template< typename Key, typename Value, typename Hash = hash<Key> >
class hashmap
{
private:
    struct iterator;
    struct const_iterator;
    struct reverse_iterator;
    struct const_reverse_iterator;

    typedef minitl::pair<const Key, Value>  value_type;
    typedef minitl::pair<const Key, Value>& reference;
    typedef minitl::pair<const Key, Value>& const_reference;
public:
    hashmap(minitl::Allocator& allocator, size_type reserved = 0);
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

#include    <minitl/inl/hash_map.inl>

/*****************************************************************************/
#endif
