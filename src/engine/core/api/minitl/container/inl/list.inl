/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_LIST_INL_
#define BE_MINITL_CONTAINER_INL_LIST_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>


namespace minitl
{

template< typename T, int ARENA >
class list<T, ARENA>::item : public intrusive_list<typename list<T, ARENA>::item>::item
{
    friend class list<T, ARENA>;
    template< typename POLICY >
    friend class list<T, ARENA>::base_iterator;
private:
    T   m_object;
private:
    item(const T& object);
    ~item();
    item(const item& other);
    item& operator=(const item& other);
};

template< typename T, int ARENA >
list<T, ARENA>::item::item(const T& object)
:   m_object(object)
{
}

template< typename T, int ARENA >
typename list<T, ARENA>::item& list<T, ARENA>::item::operator=(const item& other)
{
    m_object = other.m_object;
}

template< typename T, int ARENA >
list<T, ARENA>::item::~item()
{
}



template< typename T, int ARENA >
template< typename POLICY >
class list<T, ARENA>::base_iterator : public POLICY::base_iterator
{
    friend class list<T, ARENA>;
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

template< typename T, int ARENA >
template< typename POLICY >
list<T, ARENA>::base_iterator<POLICY>::base_iterator()
:   POLICY::base_iterator()
{
}

template< typename T, int ARENA >
template< typename POLICY >
list<T, ARENA>::base_iterator<POLICY>::base_iterator(const typename POLICY::base_iterator& other)
:   POLICY::base_iterator(other)
{
}

template< typename T, int ARENA >
template< typename POLICY >
list<T, ARENA>::base_iterator<POLICY>::~base_iterator()
{
}

template< typename T, int ARENA >
template< typename POLICY >
typename POLICY::pointer list<T, ARENA>::base_iterator<POLICY>::operator->() const
{
    return &(this->POLICY::base_iterator::operator->()->m_object);
}

template< typename T, int ARENA >
template< typename POLICY >
typename POLICY::reference list<T, ARENA>::base_iterator<POLICY>::operator*() const
{
    return this->POLICY::base_iterator::operator->()->m_object;
}

template< typename T, int ARENA >
template< typename POLICY >
typename POLICY::base_iterator::pointer list<T, ARENA>::base_iterator<POLICY>::item() const
{
    return this->POLICY::base_iterator::operator->();
}

template< typename T, int ARENA >
struct list<T, ARENA>::iterator_policy
{
    typedef typename list<T, ARENA>::value_type                     value_type;
    typedef typename list<T, ARENA>::pointer                        pointer;
    typedef typename list<T, ARENA>::reference                      reference;
    typedef typename list<T, ARENA>::size_type                      size_type;
    typedef typename list<T, ARENA>::difference_type                difference_type;
    typedef typename intrusive_list<item>::iterator                 base_iterator;
};

template< typename T, int ARENA >
struct list<T, ARENA>::const_iterator_policy
{
    typedef typename list<T, ARENA>::value_type const               value_type;
    typedef typename list<T, ARENA>::const_pointer                  pointer;
    typedef typename list<T, ARENA>::const_reference                reference;
    typedef typename list<T, ARENA>::size_type                      size_type;
    typedef typename list<T, ARENA>::difference_type                difference_type;
    typedef typename intrusive_list<item>::const_iterator           base_iterator;
};

template< typename T, int ARENA >
struct list<T, ARENA>::reverse_iterator_policy
{
    typedef typename list<T, ARENA>::value_type                     value_type;
    typedef typename list<T, ARENA>::pointer                        pointer;
    typedef typename list<T, ARENA>::reference                      reference;
    typedef typename list<T, ARENA>::size_type                      size_type;
    typedef typename list<T, ARENA>::difference_type                difference_type;
    typedef typename intrusive_list<item>::reverse_iterator         base_iterator;
};

template< typename T, int ARENA >
struct list<T, ARENA>::const_reverse_iterator_policy
{
    typedef typename list<T, ARENA>::value_type const               value_type;
    typedef typename list<T, ARENA>::const_pointer                  pointer;
    typedef typename list<T, ARENA>::const_reference                reference;
    typedef typename list<T, ARENA>::size_type                      size_type;
    typedef typename list<T, ARENA>::difference_type                difference_type;
    typedef typename intrusive_list<item>::const_reverse_iterator   base_iterator;
};




template< typename T, int ARENA >
list<T, ARENA>::list()
:   m_list()
,   m_size(0)
{
}

template< typename T, int ARENA >
list<T, ARENA>::~list()
{
    clear();
}

template< typename T, int ARENA >
typename list<T, ARENA>::iterator                   list<T, ARENA>::begin()
{
    return iterator(m_list.begin());
}

template< typename T, int ARENA >
typename list<T, ARENA>::iterator                   list<T, ARENA>::end()
{
    return iterator(m_list.end());
}

template< typename T, int ARENA >
typename list<T, ARENA>::const_iterator             list<T, ARENA>::begin() const
{
    return const_iterator(m_list.begin());
}

template< typename T, int ARENA >
typename list<T, ARENA>::const_iterator             list<T, ARENA>::end() const
{
    return const_iterator(m_list.end());
}

template< typename T, int ARENA >
typename list<T, ARENA>::reverse_iterator           list<T, ARENA>::rbegin()
{
    return  reverse_iterator(m_list.rbegin());
}

template< typename T, int ARENA >
typename list<T, ARENA>::reverse_iterator           list<T, ARENA>::rend()
{
    return reverse_iterator(m_list.rend());
}

template< typename T, int ARENA >
typename list<T, ARENA>::const_reverse_iterator     list<T, ARENA>::rbegin() const
{
    return const_reverse_iterator(m_list.rbegin());
}

template< typename T, int ARENA >
typename list<T, ARENA>::const_reverse_iterator     list<T, ARENA>::rend() const
{
    return const_reverse_iterator(m_list.rend());
}


template< typename T, int ARENA >
typename list<T, ARENA>::size_type                  list<T, ARENA>::size() const
{
    return m_size
}

template< typename T, int ARENA >
bool                                                list<T, ARENA>::empty() const
{
    return m_list.empty();
}

template< typename T, int ARENA >
void                                                list<T, ARENA>::push_front(const_reference r)
{
    item* m = BugEngine::Memory<ARENA>::allocArray<item>(1);
    new(m) item(r);
    ++m_size;
    m_list.push_front(*m);
}

template< typename T, int ARENA >
void                                                list<T, ARENA>::push_back(const_reference r)
{
    item* m = BugEngine::Memory<ARENA>::allocArray<item>(1);
    new(m) item(r);
    ++m_size;
    m_list.push_back(*m);
}

template< typename T, int ARENA >
typename list<T, ARENA>::iterator                   list<T, ARENA>::insert(iterator after, const_reference r)
{
    item* m = BugEngine::Memory<ARENA>::allocArray<item>(1);
    new(m) item(r);
    ++m_size;
    return iterator(m_list.insert(after, *m));
}

template< typename T, int ARENA >
typename list<T, ARENA>::iterator                   list<T, ARENA>::erase(iterator it)
{
    const item* i = it.item();
    it = m_list.erase(it);
    i->~item();
    BugEngine::Memory<ARENA>::free(i);
    --m_size;

    return it;
}

template< typename T, int ARENA >
typename list<T, ARENA>::iterator                   list<T, ARENA>::erase(iterator first, iterator last)
{
    while(first != last)
    {
        const item* i = first.item();
        first = m_list.erase(first);
        BugEngine::Memory<ARENA>::free(i);
        --m_size;
    }
    return first;
}


template< typename T, int ARENA >
typename list<T, ARENA>::reference                  list<T, ARENA>::front()
{
    return m_list.front();
}

template< typename T, int ARENA >
typename list<T, ARENA>::reference                  list<T, ARENA>::back()
{
    return m_list.back();
}

template< typename T, int ARENA >
typename list<T, ARENA>::const_reference            list<T, ARENA>::front() const
{
    return m_list.front();
}

template< typename T, int ARENA >
typename list<T, ARENA>::const_reference            list<T, ARENA>::back() const
{
    return m_list.back();
}

template< typename T, int ARENA >
void                                                list<T, ARENA>::clear()
{
    erase(begin(), end());
    be_assert(m_size == 0, "clearing didn't get the size down to 0");
}

}

/*****************************************************************************/
#endif
