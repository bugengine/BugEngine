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
#include    <cassert>

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
private:
    mutable unsigned int m_refCount;
private:
    inline unsigned int refcount(void) const { return m_refCount; }
    inline void         addref(void) const   { m_refCount++; assert(m_refCount); }
    inline void         decref(void) const   { assert(m_refCount); m_refCount--; }
public:
    refcountable() : m_refCount(0) {}
    inline virtual ~refcountable() { assert(m_refCount == 0); }
};

template< >
class refcountable<void>
{
    template< typename T > friend inline void addref(const refcountable<T> *ptr);
    template< typename T > friend inline const T* decref(const T *ptr);
private:
    mutable unsigned int m_refCount;
private:
    inline unsigned int refcount(void) const { return m_refCount; }
    inline void         addref(void) const   { m_refCount++; assert(m_refCount); }
    inline void         decref(void) const   { assert(m_refCount); m_refCount--; }
public:
    refcountable() : m_refCount(0) {}
    inline virtual ~refcountable() { assert(m_refCount == 0); }    
};


template< typename T >
inline void addref(const refcountable<T> *ptr)
{
    ptr->addref();
}

template< typename T >
inline const T* decref(const T *ptr)
{
    assert(ptr->refcount() > 0);
    ptr->decref();
    if(! ptr->refcount())
    {
        checked_delete<const T>(ptr);
        return 0;
    }
    else
    {
        return ptr;
    }
}

}

/*****************************************************************************/
#endif
