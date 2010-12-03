/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_LIST_HH_
#define BE_MINITL_CONTAINER_LIST_HH_
/*****************************************************************************/
#include <minitl/container/iterator.hh>
#include <minitl/container/intrusive_list.hh>

namespace minitl
{

template< typename T >
class list
{
private:
    class item;
private:
    template< typename POLICY >
    class base_iterator;
    struct iterator_policy;
    struct const_iterator_policy;
    struct reverse_iterator_policy;
    struct const_reverse_iterator_policy;
public:
    typedef T                       value_type;
    typedef T*                      pointer;
    typedef T&                      reference;
    typedef const T*                const_pointer;
    typedef const T&                const_reference;
    typedef minitl::size_type       size_type;
    typedef minitl::difference_type difference_type;
public:
    typedef base_iterator<iterator_policy>                  iterator;
    typedef base_iterator<const_iterator_policy>            const_iterator;
    typedef base_iterator<reverse_iterator_policy>          reverse_iterator;
    typedef base_iterator<const_reverse_iterator_policy>    const_reverse_iterator;
private:
    intrusive_list<item>    m_list;
    size_type               m_size;
    BugEngine::Allocator&   m_allocator;
public:
    list(BugEngine::Allocator& allocator);
    ~list();

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

    void                    push_front(const_reference r);
    void                    push_back(const_reference r);
    iterator                insert(iterator after, const_reference r);
    iterator                erase(iterator it);
    iterator                erase(iterator begin, iterator end);

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

    void                    clear();
};

}

#include <minitl/container/inl/list.inl>

/*****************************************************************************/
#endif
