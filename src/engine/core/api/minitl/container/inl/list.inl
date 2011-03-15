/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_LIST_INL_
#define BE_MINITL_CONTAINER_INL_LIST_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>


namespace minitl
{

template< typename T >
class list<T>::item : public intrusive_list<typename list<T>::item>::item
{
    friend class list<T>;
public:
    T   m_object;
private:
    item(const T& object);
    ~item();
    item(const item& other);
    item& operator=(const item& other);
};

template< typename T >
list<T>::item::item(const T& object)
:   m_object(object)
{
}

template< typename T >
typename list<T>::item& list<T>::item::operator=(const item& other)
{
    m_object = other.m_object;
}

template< typename T >
list<T>::item::~item()
{
}



template< typename T >
template< typename POLICY >
class list<T>::base_iterator : public POLICY::base_iterator
{
    friend class list<T>;
private:
    explicit base_iterator(const typename POLICY::base_iterator& other);
    base_iterator& operator=(const typename POLICY::base_iterator& other)
    {
        POLICY::base_iterator::operator=(other);
        return *this;
    }
public:
    base_iterator();
    ~base_iterator();
public:
    typedef          bidirectional_iterator_tag         iterator_category;
    typedef typename POLICY::value_type                 value_type;
    typedef typename POLICY::pointer                    pointer;
    typedef typename POLICY::reference                  reference;
    typedef typename POLICY::size_type                  size_type;
    typedef typename POLICY::difference_type            difference_type;
public:
    typename POLICY::pointer                operator->() const;
    typename POLICY::reference              operator*() const;
    typename POLICY::base_iterator::pointer item() const;
};

template< typename T >
template< typename POLICY >
list<T>::base_iterator<POLICY>::base_iterator()
:   POLICY::base_iterator()
{
}

template< typename T >
template< typename POLICY >
list<T>::base_iterator<POLICY>::base_iterator(const typename POLICY::base_iterator& other)
:   POLICY::base_iterator(other)
{
}

template< typename T >
template< typename POLICY >
list<T>::base_iterator<POLICY>::~base_iterator()
{
}

template< typename T >
template< typename POLICY >
typename POLICY::pointer list<T>::base_iterator<POLICY>::operator->() const
{
    return &(this->POLICY::base_iterator::operator->()->m_object);
}

template< typename T >
template< typename POLICY >
typename POLICY::reference list<T>::base_iterator<POLICY>::operator*() const
{
    return this->POLICY::base_iterator::operator->()->m_object;
}

template< typename T >
template< typename POLICY >
typename POLICY::base_iterator::pointer list<T>::base_iterator<POLICY>::item() const
{
    return this->POLICY::base_iterator::operator->();
}

template< typename T >
struct list<T>::iterator_policy
{
    typedef typename list<T>::value_type                     value_type;
    typedef typename list<T>::pointer                        pointer;
    typedef typename list<T>::reference                      reference;
    typedef typename list<T>::size_type                      size_type;
    typedef typename list<T>::difference_type                difference_type;
    typedef typename intrusive_list<item>::iterator                 base_iterator;
};

template< typename T >
struct list<T>::const_iterator_policy
{
    typedef typename list<T>::value_type const               value_type;
    typedef typename list<T>::const_pointer                  pointer;
    typedef typename list<T>::const_reference                reference;
    typedef typename list<T>::size_type                      size_type;
    typedef typename list<T>::difference_type                difference_type;
    typedef typename intrusive_list<item>::const_iterator           base_iterator;
};

template< typename T >
struct list<T>::reverse_iterator_policy
{
    typedef typename list<T>::value_type                     value_type;
    typedef typename list<T>::pointer                        pointer;
    typedef typename list<T>::reference                      reference;
    typedef typename list<T>::size_type                      size_type;
    typedef typename list<T>::difference_type                difference_type;
    typedef typename intrusive_list<item>::reverse_iterator         base_iterator;
};

template< typename T >
struct list<T>::const_reverse_iterator_policy
{
    typedef typename list<T>::value_type const               value_type;
    typedef typename list<T>::const_pointer                  pointer;
    typedef typename list<T>::const_reference                reference;
    typedef typename list<T>::size_type                      size_type;
    typedef typename list<T>::difference_type                difference_type;
    typedef typename intrusive_list<item>::const_reverse_iterator   base_iterator;
};




template< typename T >
list<T>::list(BugEngine::Allocator& allocator)
:   m_list()
,   m_size(0)
,   m_allocator(allocator)
{
}

template< typename T >
list<T>::~list()
{
    clear();
}

template< typename T >
typename list<T>::iterator                   list<T>::begin()
{
    return iterator(m_list.begin());
}

template< typename T >
typename list<T>::iterator                   list<T>::end()
{
    return iterator(m_list.end());
}

template< typename T >
typename list<T>::const_iterator             list<T>::begin() const
{
    return const_iterator(m_list.begin());
}

template< typename T >
typename list<T>::const_iterator             list<T>::end() const
{
    return const_iterator(m_list.end());
}

template< typename T >
typename list<T>::reverse_iterator           list<T>::rbegin()
{
    return  reverse_iterator(m_list.rbegin());
}

template< typename T >
typename list<T>::reverse_iterator           list<T>::rend()
{
    return reverse_iterator(m_list.rend());
}

template< typename T >
typename list<T>::const_reverse_iterator     list<T>::rbegin() const
{
    return const_reverse_iterator(m_list.rbegin());
}

template< typename T >
typename list<T>::const_reverse_iterator     list<T>::rend() const
{
    return const_reverse_iterator(m_list.rend());
}


template< typename T >
typename list<T>::size_type                  list<T>::size() const
{
    return m_size;
}

template< typename T >
bool                                                list<T>::empty() const
{
    return m_list.empty();
}

template< typename T >
void                                                list<T>::push_front(const_reference r)
{
    item* m = m_allocator.alloc<item>();
    new(m) item(r);
    ++m_size;
    m_list.push_front(*m);
}

template< typename T >
void                                                list<T>::push_back(const_reference r)
{
    item* m = m_allocator.alloc<item>();
    new(m) item(r);
    ++m_size;
    m_list.push_back(*m);
}

template< typename T >
typename list<T>::iterator                   list<T>::insert(iterator after, const_reference r)
{
    item* m = m_allocator.alloc<item>();
    new(m) item(r);
    ++m_size;
    return iterator(m_list.insert(after, *m));
}

template< typename T >
typename list<T>::iterator                   list<T>::erase(iterator it)
{
    const item* i = it.item();
    it = m_list.erase(it);
    i->~item();
    m_allocator.free(i);
    --m_size;

    return it;
}

template< typename T >
typename list<T>::iterator                   list<T>::erase(iterator first, iterator last)
{
    while (first != last)
    {
        const item* i = first.item();
        first = m_list.erase(first);
        m_allocator.free(i);
        --m_size;
    }
    return first;
}


template< typename T >
typename list<T>::reference                  list<T>::front()
{
    return m_list.front();
}

template< typename T >
typename list<T>::reference                  list<T>::back()
{
    return m_list.back();
}

template< typename T >
typename list<T>::const_reference            list<T>::front() const
{
    return m_list.front();
}

template< typename T >
typename list<T>::const_reference            list<T>::back() const
{
    return m_list.back();
}

template< typename T >
void                                                list<T>::clear()
{
    erase(begin(), end());
    be_assert(m_size == 0, "clearing didn't get the size down to 0");
}

}

/*****************************************************************************/
#endif
