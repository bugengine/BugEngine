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
    mutable T*   m_next;
    mutable T*   m_previous;
protected:
    item();
    ~item();
    item(const item& other);
    item& operator=(const item& other);

    void insert(const T* after) const;
    void unhook() const;
};

template< typename T, int INDEX >
intrusive_list<T, INDEX>::item::item()
:   m_next(static_cast<T*>(this))
,   m_previous(static_cast<T*>(this))
{
}

template< typename T, int INDEX >
intrusive_list<T, INDEX>::item::item(const item& other)
:   m_next(static_cast<T*>(this))
,   m_previous(static_cast<T*>(this))
{
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::item& intrusive_list<T, INDEX>::item::operator=(const item& other)
{
    return *this;
}

template< typename T, int INDEX >
intrusive_list<T, INDEX>::item::~item()
{
    be_assert_recover(m_next == this, "destroying item that is still in a list", unhook());
    be_assert_recover(m_previous == this, "destroying item that is still in a list", unhook());
}

template< typename T, int INDEX >
void intrusive_list<T, INDEX>::item::insert(const T* after) const
{
    be_assert_recover(m_next == this, "list item already belongs to a list", return);
    be_assert_recover(m_previous == this, "list item already belongs to a list", return);
    m_next = after->m_next;
    m_previous = const_cast<T*>(after);
    after->m_next =  static_cast<T*>(const_cast<item*>(this));
    m_next->m_previous =  static_cast<T*>(const_cast<item*>(this));
}

template< typename T, int INDEX >
void intrusive_list<T, INDEX>::item::unhook() const
{
    be_assert_recover(m_next != this, "list item does not belong to a list", return);
    be_assert_recover(m_previous != this, "list item does not belong to a list", return);
    m_next->m_previous = m_previous;
    m_previous->m_next = m_next;
    m_next = static_cast<T*>(const_cast<item*>(this));
    m_previous = static_cast<T*>(const_cast<item*>(this));
}


template< typename T, int INDEX >
template< typename POLICY >
class intrusive_list<T, INDEX>::base_iterator
{
    friend class intrusive_list<T, INDEX>;
public:
    typedef          bidirectional_iterator_tag         iterator_category;
    typedef typename POLICY::value_type                 value_type;
    typedef typename POLICY::pointer                    pointer;
    typedef typename POLICY::reference                  reference;
    typedef typename POLICY::size_type                  size_type;
    typedef typename POLICY::difference_type            difference_type;
private:
    typename POLICY::pointer    m_iterator;
private:
    explicit base_iterator(typename POLICY::pointer it);
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
intrusive_list<T, INDEX>::base_iterator<POLICY>::base_iterator(typename POLICY::pointer it)
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
    return m_iterator;
}

template< typename T, int INDEX >
template< typename POLICY >
typename POLICY::reference intrusive_list<T, INDEX>::base_iterator<POLICY>::operator*() const
{
    return *m_iterator;
}


template< typename T, int INDEX >
struct intrusive_list<T, INDEX>::iterator_policy
{
    typedef typename intrusive_list<T, INDEX>::value_type       value_type;
    typedef typename intrusive_list<T, INDEX>::pointer          pointer;
    typedef typename intrusive_list<T, INDEX>::reference        reference;
    typedef typename intrusive_list<T, INDEX>::size_type        size_type;
    typedef typename intrusive_list<T, INDEX>::difference_type  difference_type;
    static pointer next(pointer i) { return i->intrusive_list<T, INDEX>::item::m_next; }
    static pointer previous(pointer i) { return i->intrusive_list<T, INDEX>::item::m_prev; }
};

template< typename T, int INDEX >
struct intrusive_list<T, INDEX>::const_iterator_policy
{
    typedef typename intrusive_list<T, INDEX>::value_type const value_type;
    typedef typename intrusive_list<T, INDEX>::const_pointer    pointer;
    typedef typename intrusive_list<T, INDEX>::const_reference  reference;
    typedef typename intrusive_list<T, INDEX>::size_type        size_type;
    typedef typename intrusive_list<T, INDEX>::difference_type  difference_type;
    static pointer next(pointer i) { return i->intrusive_list<T, INDEX>::item::m_next; }
    static pointer previous(pointer i) { return i->intrusive_list<T, INDEX>::item::m_prev; }
};

template< typename T, int INDEX >
struct intrusive_list<T, INDEX>::reverse_iterator_policy
{
    typedef typename intrusive_list<T, INDEX>::value_type       value_type;
    typedef typename intrusive_list<T, INDEX>::pointer          pointer;
    typedef typename intrusive_list<T, INDEX>::reference        reference;
    typedef typename intrusive_list<T, INDEX>::size_type        size_type;
    typedef typename intrusive_list<T, INDEX>::difference_type  difference_type;
    static pointer next(pointer i) { return i->intrusive_list<T, INDEX>::item::m_prev; }
    static pointer previous(pointer i) { return i->intrusive_list<T, INDEX>::item::m_next; }
};

template< typename T, int INDEX >
struct intrusive_list<T, INDEX>::const_reverse_iterator_policy
{
    typedef typename intrusive_list<T, INDEX>::value_type const value_type;
    typedef typename intrusive_list<T, INDEX>::const_pointer    pointer;
    typedef typename intrusive_list<T, INDEX>::const_reference  reference;
    typedef typename intrusive_list<T, INDEX>::size_type        size_type;
    typedef typename intrusive_list<T, INDEX>::difference_type  difference_type;
    static pointer next(pointer i) { return i->intrusive_list<T, INDEX>::item::m_prev; }
    static pointer previous(pointer i) { return i->intrusive_list<T, INDEX>::item::m_next; }
};


template< typename T, int INDEX >
intrusive_list<T, INDEX>::intrusive_list()
:   m_root()
{
}

template< typename T, int INDEX >
intrusive_list<T, INDEX>::~intrusive_list()
{
    clear();
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::begin()
{
    return iterator(m_root.m_next);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::end()
{
    return iterator(static_cast<pointer>(&m_root));
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_iterator           intrusive_list<T, INDEX>::begin() const
{
    return const_iterator(m_root.m_next);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_iterator           intrusive_list<T, INDEX>::end() const
{
    return const_iterator(static_cast<const_pointer>(&m_root));
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::reverse_iterator         intrusive_list<T, INDEX>::rbegin()
{
    return reverse_iterator(m_root.m_previous);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::reverse_iterator         intrusive_list<T, INDEX>::rend()
{
    return reverse_iterator(static_cast<pointer>(&m_root));
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_reverse_iterator   intrusive_list<T, INDEX>::rbegin() const
{
    return const_reverse_iterator(m_root.m_previous);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_reverse_iterator   intrusive_list<T, INDEX>::rend() const
{
    return const_reverse_iterator(static_cast<const_pointer>(&m_root));
}


template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::size_type                intrusive_list<T, INDEX>::size() const
{
    size_type size = 0;
    for(const item* t = m_root.m_next; t != m_root.m_previous; t = t->m_next)
    {
        size++;
    }
    return size;
}

template< typename T, int INDEX >
bool                                                        intrusive_list<T, INDEX>::empty() const
{
    return m_root.m_next == &m_root;
}

template< typename T, int INDEX >
void                                                        intrusive_list<T, INDEX>::push_front(const_reference r)
{
    r.item::insert(&m_root);
}

template< typename T, int INDEX >
void                                                        intrusive_list<T, INDEX>::push_back(const_reference r)
{
    r.item::insert(m_root.m_previous);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::insert(typename intrusive_list<T, INDEX>::iterator after, const_reference r)
{
    r.item::insert(after.m_iterator);
    return ++after;
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::erase(iterator it)
{
    const item* i = it.m_iterator;
    ++it;
    i->unhook();
    return it;
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::iterator                 intrusive_list<T, INDEX>::erase(iterator first, iterator last)
{
    while(first != last)
    {
        item* i = first.m_iterator;
        ++first;
        i->unhook();
    }
}


template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::reference                intrusive_list<T, INDEX>::front()
{
    return *static_cast<T*>(m_root.m_next);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::reference                intrusive_list<T, INDEX>::back()
{
    return *static_cast<T*>(m_root.m_previous);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_reference          intrusive_list<T, INDEX>::front() const
{
    return *static_cast<const T*>(m_root.m_next);
}

template< typename T, int INDEX >
typename intrusive_list<T, INDEX>::const_reference          intrusive_list<T, INDEX>::back() const
{
    return *static_cast<const T*>(m_root.m_previous);
}

template< typename T, int INDEX >
void                                                        intrusive_list<T, INDEX>::clear()
{
    iterator first = begin();
    iterator last = end();
    while(first != last)
    {
        item* i = first.m_iterator;
        ++first;
        i->unhook();
    }
}

}

/*****************************************************************************/
#endif
