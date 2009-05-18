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

#ifndef BE_RTTI_PROPERTYBUILDER_INL_
#define BE_RTTI_PROPERTYBUILDER_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename Reader,
          typename Writer >
refptr<Property> PropertyBuilder<Reader, Writer>::buildProperty() const
{
    return new ObjectProperty< typename Reader::PropertyType, typename Reader::Owner, Reader, Writer>();
}

template< typename Reader >
refptr<Property> PropertyBuilder<Reader, void>::buildProperty() const
{
    return new ObjectProperty< typename Reader::PropertyType, typename Reader::Owner, Reader, SetImpossible< typename Reader::PropertyType, typename Reader::Owner> >();
}

template< typename Writer >
refptr<Property> PropertyBuilder<void, Writer>::buildProperty() const
{
    return new ObjectProperty< typename Writer::PropertyType, typename Writer::Owner, GetImpossible< typename Writer::PropertyType, typename Writer::Owner>,Writer >();
}

/*template< >
template< typename Reader >
PropertyBuilder< Reader, void > PropertyBuilder< void, void >::setReader(const Reader& reader)
{
    return PropertyBuilder< Reader, void >();
}

{
    template< typename Reader >
    inline PropertyBuilder< Reader, void > setReader(const Reader& reader)
    {
        UNUSED(reader);
        return PropertyBuilder< Reader, void >();
    }
    template< typename Writer >
    inline PropertyBuilder< void, Writer > setWriter(const Writer& writer)
    {
        UNUSED(writer);
        return PropertyBuilder< void, Writer >();
    }
    template< typename Owner, size_t offset, typename T >
    static inline GetFromField<T,Owner,offset> createReadFieldFromOffset(T* ptr)
    {
        UNUSED(ptr);
        return GetFromField<T,Owner,offset>();
    }
    template< typename Owner, size_t offset, typename T >
    static inline SetFromField<T,Owner,offset> createWriteFieldFromOffset(T* ptr)
    {
        UNUSED(ptr);
        return SetFromField<T,Owner,offset>();
    }

public:
    template< typename Reader >
    inline PropertyBuilder< Reader, void > setReader(const Reader& reader);
    template< typename Writer >
    inline PropertyBuilder< Writer, void > setWriter(const Writer& writer);
    template< typename Owner, size_t offset, typename T >
    static inline GetFromField<T,Owner,offset> createReadFieldFromOffset(T* ptr);
    template< typename Owner, size_t offset, typename T >
    static inline SetFromField<T,Owner,offset> createWriteFieldFromOffset(T* ptr);
};

template< typename _Reader >
class PropertyBuilder< _Reader, void >
{
public:
    template< typename Writer >
    inline PropertyBuilder< Writer, _Reader > setWriter(const Writer& writer) const;
    refptr<Property> buildProperty() const;
};

template< typename _Writer >
class PropertyBuilder< void, _Writer >
{
public:
    template< typename Reader >
    inline PropertyBuilder< Reader, _Writer > setWriter(const Reader& reader) const;
    refptr<Property> buildProperty() const;
};*/

}}

/*****************************************************************************/
#endif
