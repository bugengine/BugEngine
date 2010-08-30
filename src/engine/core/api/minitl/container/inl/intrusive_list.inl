/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_INTRUSIVE_LIST_INL_
#define BE_MINITL_CONTAINER_INL_INTRUSIVE_LIST_INL_
/*****************************************************************************/
#include    <core/debug/assert.hh>


namespace minitl
{

template< typename T, int INDEX >
class intrusive_list<T, INDEX>::item
{
    friend class intrusive_list<T, INDEX>;
private:
    mutable item*   m_next;
    mutable item*   m_previous;
protected:
    item();
    ~item();
};

template< typename T, int INDEX >
intrusive_list<T, INDEX>::item::item()
:   m_next(0)
,   m_previous(0)
{
}

template< typename T, int INDEX >
intrusive_list<T, INDEX>::item::~item()
{
    be_assert(m_next == 0, "destroying item that is still in a list");
    be_assert(m_previous == 0, "destroying item that is still in a list");
}


template< typename T, int INDEX >
template< typename POLICY >
class intrusive_list<T, INDEX>::base_iterator
{
    friend class intrusive_list<T, INDEX>;
private:
    const typename intrusive_list<T, INDEX>::item*    m_iterator;
private:
    base_iterator(const typename intrusive_list<T, INDEX>::item* it);
public:
    base_iterator();
    ~base_iterator();
public:
    bool operator==(const base_iterator<POLICY>& other);
    bool operator!=(const base_iterator<POLICY>& other);

    base_iterator<POLICY>& operator++()
    {
        m_iterator = POLICY::next(m_iterator);
        return *this;
    }
    base_iterator<POLICY>  operator++(int)
    {
        base_iterator<POLICY> p = *this;
        m_iterator = POLICY::next(m_iterator);
        return p;
    }
    base_iterator<POLICY>& operator--()
    {
        m_iterator = POLICY::previous(m_iterator);
        return *this;
    }
    base_iterator<POLICY>  operator--(int)
    {
        base_iterator<POLICY> p = *this;
        m_iterator = POLICY::previous(m_iterator);
        return p;
    }

    typename POLICY::pointer    operator->() const;
    typename POLICY::reference  operator*() const;
};

template< typename T, int INDEX >
template< typename POLICY >
intrusive_list<T, INDEX>::base_iterator<POLICY>::base_iterator()
:   m_iterator(0)
{
}

template< typename T, int INDEX >
template< typename POLICY >
intrusive_list<T, INDEX>::base_iterator<POLICY>::base_iterator(const typename intrusive_list<T, INDEX>::item* it)
:   m_iterator(it)
{
}

template< typename T, int INDEX >
template< typename POLICY >
intrusive_list<T, INDEX>::base_iterator<POLICY>::~base_iterator()
{
}

template< typename T, int INDEX >
template< typename POLICY >
bool intrusive_list<T, INDEX>::base_iterator<POLICY>::operator==(const base_iterator<POLICY>& other)
{
    return m_iterator == other.m_iterator;
}

template< typename T, int INDEX >
template< typename POLICY >
bool intrusive_list<T, INDEX>::base_iterator<POLICY>::operator!=(const base_iterator<POLICY>& other)
{
    return m_iterator != other.m_iterator;
}

template< typename T, int INDEX >
template< typename POLICY >
typename POLICY::pointer intrusive_list<T, INDEX>::base_iterator<POLICY>::operator->() const
{
    return static_cast<pointer>(m_iterator);
}

template< typename T, int INDEX >
template< typename POLICY >
typename POLICY::reference intrusive_list<T, INDEX>::base_iterator<POLICY>::operator*() const
{
    return *static_cast<pointer>(m_iterator);
}


template< typename T, int INDEX >
struct intrusive_list<T, INDEX>::iterator_policy
{
    typedef typename intrusive_list<T, INDEX>::pointer          pointer;
    typedef typename intrusive_list<T, INDEX>::reference        reference;
    typedef typename intrusive_list<T, INDEX>::size_type        size_type;
    typedef typename intrusive_list<T, INDEX>::difference_type  difference_type;
    static pointer next(pointer i) { return i->list<T, INDEX>::item::m_next; }
    static pointer previous(pointer i) { return i->list<T, INDEX>::item::m_prev; }
};

template< typename T, int INDEX >
struct intrusive_list<T, INDEX>::const_iterator_policy
{
    typedef typename intrusive_list<T, INDEX>::const_pointer    pointer;
    typedef typename intrusive_list<T, INDEX>::const_reference  reference;
    typedef typename intrusive_list<T, INDEX>::size_type        size_type;
    typedef typename intrusive_list<T, INDEX>::difference_type  difference_type;
    static pointer next(pointer i) { return i->list<T, INDEX>::item::m_next; }
    static pointer previous(pointer i) { return i->list<T, INDEX>::item::m_prev; }
};

template< typename T, int INDEX >
struct intrusive_list<T, INDEX>::reverse_iterator_policy
{
    typedef typename intrusive_list<T, INDEX>::pointer          pointer;
    typedef typename intrusive_list<T, INDEX>::reference        reference;
    typedef typename intrusive_list<T, INDEX>::size_type        size_type;
    typedef typename intrusive_list<T, INDEX>::difference_type  difference_type;
    static pointer next(pointer i) { return i->list<T, INDEX>::item::m_prev; }
    static pointer previous(pointer i) { return i->list<T, INDEX>::item::m_next; }
};

template< typename T, int INDEX >
struct intrusive_list<T, INDEX>::const_reverse_iterator_policy
{
    typedef typename intrusive_list<T, INDEX>::const_pointer    pointer;
    typedef typename intrusive_list<T, INDEX>::const_reference  reference;
    typedef typename intrusive_list<T, INDEX>::size_type        size_type;
    typedef typename intrusive_list<T, INDEX>::difference_type  difference_type;
    static pointer next(pointer i) { return i->list<T, INDEX>::item::m_prev; }
    static pointer previous(pointer i) { return i->list<T, INDEX>::item::m_next; }
};


template< typename T, int INDEX >
intrusive_list<T, INDEX>::intrusive_list()
:   m_root()
,   m_size(0)
{
}

template< typename T, int INDEX >
intrusive_list<T, INDEX>::~intrusive_list()
{
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::begin()
{
    return iterator(m_root->m_next);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::end()
{
    return iterator(m_root);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_iterator           intrusive_list<T, INDEX>::begin() const
{
    return const_iterator(m_root->m_next);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_iterator           intrusive_list<T, INDEX>::end() const
{
    return const_iterator(m_root);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::reverse_iterator         intrusive_list<T, INDEX>::rbegin()
{
    return reverse_iterator(m_root->m_previous);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::reverse_iterator         intrusive_list<T, INDEX>::rend()
{
    return reverse_iterator(m_root);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_reverse_iterator   intrusive_list<T, INDEX>::rbegin() const
{
    return const_reverse_iterator(m_root->m_previous);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_reverse_iterator   intrusive_list<T, INDEX>::rend() const
{
    return const_reverse_iterator(m_root);
}


template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::size_type                intrusive_list<T, INDEX>::size() const
{
    return m_size;
}

template< typename T, int INDEX >
bool                                                        intrusive_list<T, INDEX>::empty() const
{
    return m_size == 0;
}

template< typename T, int INDEX >
void                                                        intrusive_list<T, INDEX>::push_front(const_reference r)
{
}

template< typename T, int INDEX >
void                                                        intrusive_list<T, INDEX>::push_back(const_reference r)
{
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::erase(iterator it)
{
    return erase(it, it+1);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::erase(iterator first, iterator last)
{
}


template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::reference                intrusive_list<T, INDEX>::front()
{
    return *static_cast<T*>(m_root->m_next);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::reference                intrusive_list<T, INDEX>::back()
{
    return *static_cast<T*>(m_root->m_previous);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_reference          intrusive_list<T, INDEX>::front() const
{
    return *static_cast<const T*>(m_root->m_next);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_reference          intrusive_list<T, INDEX>::back() const
{
    return *static_cast<const T*>(m_root->m_previous);
}

template< typename T, int INDEX >
void                                                        intrusive_list<T, INDEX>::clear()
{
}

}

/*****************************************************************************/
#endif
