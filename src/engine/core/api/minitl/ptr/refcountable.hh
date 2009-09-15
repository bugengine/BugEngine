/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_MINITL_PTR_REFCOUNTABLE_
#define BE_MINITL_PTR_REFCOUNTABLE_
/*****************************************************************************/

namespace minitl
{

template< typename T >
class refcountable;

template< typename T >
inline void addref(const refcountable<T> *ptr);
template< typename T >
inline const T* decref(const T *ptr);

template< class T = void >
class refcountable : public T
{
    template< typename U > friend inline void addref(const refcountable<U>* ptr);
    template< typename U > friend inline const U* decref(const U *ptr);
    template< typename U > friend inline void addweak(const refcountable<U>* ptr);
    template< typename U > friend inline void decweak(const U *ptr);
private:
    mutable i_u32 m_refCount;
#ifdef BE_ENABLE_WEAKCHECK
    mutable i_u32 m_weakCount;
#endif
public:
    refcountable()
    :   m_refCount(0)
#ifdef BE_ENABLE_WEAKCHECK
    ,   m_weakCount(0)
#endif
    {}
    inline virtual ~refcountable()
    {
        be_assert(m_refCount == 0, "object is destroyed but has %d references" | m_refCount);
#ifdef BE_ENABLE_WEAKCHECK
        be_assert(m_weakCount == 0, "object is destroyed but has %d weak references" | m_weakCount);
#endif
    }
};

template< >
class refcountable<void>
{
    template< typename U > friend inline void addref(const refcountable<U>* ptr);
    template< typename U > friend inline const U* decref(const U *ptr);
    template< typename U > friend inline void addweak(const refcountable<U>* ptr);
    template< typename U > friend inline void decweak(const U *ptr);
private:
    mutable i_u32 m_refCount;
#ifdef BE_ENABLE_WEAKCHECK
    mutable i_u32 m_weakCount;
#endif
public:
    refcountable()
    :   m_refCount(0)
#ifdef BE_ENABLE_WEAKCHECK
    ,   m_weakCount(0)
#endif
    {}
    inline virtual ~refcountable()
    {
        be_assert(m_refCount == 0, "object is destroyed but has %d references" | m_refCount);
#ifdef BE_ENABLE_WEAKCHECK
        be_assert(m_weakCount == 0, "object is destroyed but has %d weak references" | m_weakCount);
#endif
    }
};


template< typename T >
inline void addref(const refcountable<T> *ptr)
{
    if(ptr)
        ++ptr->m_refCount;
}

template< typename T >
inline const T* decref(const T *ptr)
{
    if(!ptr)
        return 0;
    be_assert(ptr->m_refCount > 0, "object has no reference; cannot dereference it again");
    if(! --ptr->m_refCount)
    {
        checked_delete<const T>(ptr);
        return 0;
    }
    else
    {
        return ptr;
    }
}

template< typename T >
inline void addweak(const refcountable<T> *ptr)
{
#ifdef BE_ENABLE_WEAKCHECK
    if(ptr)
        ++ptr->m_weakCount;
#endif
}

template< typename T >
inline void decweak(const T *ptr)
{
#ifdef BE_ENABLE_WEAKCHECK
    if(!ptr)
        return;
    be_assert(ptr->m_weakCount, "object has no weak reference; cannot dereference it again");
    --ptr->m_weakCount;
#endif
}

}

/*****************************************************************************/
#endif
