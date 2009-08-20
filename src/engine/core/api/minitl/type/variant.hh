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

#ifndef BE_MINITL_TYPE_UNION_
#define BE_MINITL_TYPE_UNION_
/*****************************************************************************/
#include    <minitl/type/typelist.hh>


namespace minitl
{

template< typename typelist >
void destroy(size_t typeindex, unsigned char* dest);
template< typename typelist >
void copy(size_t typeindex, unsigned char* dest, const unsigned char* src);

template< typename typelist >
class variant
{
private:
    size_t          m_typeIndex;
    unsigned char   m_value[typelist::SizeOf];
public:
    variant();
    
    template< typename T >
    inline explicit variant(const T& value);
    inline variant(const variant<typelist>& other);
    inline variant<typelist>& operator=(const variant<typelist>& other);
    template< typename T >
    inline variant& operator=(const T& value);
    
    inline ~variant();

    inline size_t typeindex() const { return m_typeIndex; }
    template< typename T >
    inline T& as();
    template< typename T >
    inline const T& as() const;
};


template< typename typelist >
variant<typelist>::variant() :
    m_typeIndex( indexof<void,typelist>::Value )
{
}


template< typename typelist >
template< typename T >
variant< typelist >::variant(const T& value) :
    m_typeIndex( indexof<T,typelist>::Value )
{
    new(m_value) T(value);
}

template< typename typelist >
variant<typelist>::variant(const variant& other) :
    m_typeIndex( other.m_typeIndex )    
{
    copy<typelist>(m_typeIndex, m_value, other.m_value);
}

template< typename typelist >
variant<typelist>& variant<typelist>::operator=(const variant<typelist>& other)
{
    destroy<typelist>(m_typeIndex, m_value);
    m_typeIndex = other.m_typeIndex;
    copy<typelist>(m_typeIndex, m_value, other.m_value);
    return *this;
}

template< typename typelist >
template< typename T >
variant<typelist>& variant<typelist>::operator=(const T& value)
{
    destroy<typelist>(m_typeIndex, m_value);
    m_typeIndex = indexof<T,typelist>::Value;
    new(m_value) T(value);
    return *this;
}

template< typename typelist >
variant< typelist >::~variant()
{
    destroy<typelist>(m_typeIndex, m_value);
}

template< typename typelist >
template< typename T >
const T& variant< typelist >::as() const
{
    Assert( m_typeIndex == (indexof<T,typelist>::Value) );
    return *reinterpret_cast<const T*>(&m_value[0]);
}

template< typename typelist >
template< typename T >
T& variant< typelist >::as()
{
    Assert( m_typeIndex == (indexof<T,typelist>::Value) );
    return *reinterpret_cast<T*>(&m_value[0]);
}

//-----------------------------------------------------------------------------

template< >
inline void destroy<void>(size_t typeindex, unsigned char *data)
{
    UNUSED(typeindex);
    UNUSED(data);
}

template< typename typelist >
inline void destroy(size_t typeindex, unsigned char *data)
{
    if(typeindex == typelist::IndexOf)
    {
        typedef typename typelist::Type thistype;
        reinterpret_cast<thistype*>(data)->~thistype();
    }
    else
    {
        destroy<typename typelist::Tail>(typeindex, data);
    }
}


template< >
inline void copy<void>(size_t typeindex, unsigned char* dest, const unsigned char* src)
{
    UNUSED(typeindex);
    UNUSED(dest);
    UNUSED(src);
}

template< typename typelist >
inline void copy(size_t typeindex, unsigned char* dest, const unsigned char* src)
{
    if(typeindex == typelist::IndexOf)
    {
        typedef typename typelist::Type thistype;
        new(dest) thistype(*reinterpret_cast<const thistype*>(src));
    }
    else
    {
        copy<typename typelist::Tail>(typeindex, dest, src);
    }
}


}

/*****************************************************************************/
#endif
