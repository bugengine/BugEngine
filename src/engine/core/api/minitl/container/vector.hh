/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_VECTOR_HH_
#define BE_MINITL_CONTAINER_VECTOR_HH_
/*****************************************************************************/
#include    <minitl/container/iterator.hh>
#include    <core/memory/malloc.hh>

namespace minitl
{

template< typename T, int ARENA >
class vector
{
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
    typename BugEngine::Memory<ARENA>::template Block<T>    m_memory;
    T*                                                      m_end;
    T*                                                      m_capacity;
private:
    void grow(size_type capacity);
public:
    vector();
    ~vector();

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

    reference               operator[](size_type i);
    const_reference         operator[](size_type i) const;

    void                    push_back(const_reference r);
    iterator                erase(iterator it);
    iterator                erase(iterator begin, iterator end);

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

    void                    resize(size_type size);
    void                    clear();
};

}

#include <minitl/container/inl/vector.inl>

/*****************************************************************************/
#endif
