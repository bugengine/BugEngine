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

#ifndef BE_RTTI_PROPERTYBUILDER_HH_
#define BE_RTTI_PROPERTYBUILDER_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename T, typename Owner >
struct GetterBuilder
{
    template< T (Owner::*GETTER)() >
    GetFromGetter<T,Owner,GETTER> operator()() { return GetFromGetter<T,Owner,GETTER>(); }
};
template< typename T, typename Owner >
struct GetterBuilderConst
{
    template< T (Owner::*GETTER)() const >
    GetFromGetterConst<T,Owner,GETTER> operator()() { return GetFromGetterConst<T,Owner,GETTER>(); }
};
template< typename T, typename Owner >
struct SetterBuilder
{
    template< void (Owner::*SETTER)(T) >
    SetFromSetter<T,Owner,SETTER> operator()() { return SetFromSetter<T,Owner,SETTER>(); }
};

template< typename Reader,
          typename Writer >
class PropertyBuilder
{
public:
    inline refptr<Property> buildProperty() const;
};

template< >
class PropertyBuilder< void, void >
{
public:
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
    template< typename T, typename Owner >
    static inline GetterBuilder<T,Owner> createReadFieldFromMethod(T (Owner::*g)())
    {
        UNUSED(g);
        return GetterBuilder<T,Owner>();
    }
    template< typename T, typename Owner >
    static inline GetterBuilderConst<T,Owner> createReadFieldFromMethod(T (Owner::*g)() const)
    {
        UNUSED(g);
        return GetterBuilderConst<T,Owner>();
    }
    template< typename T, typename Owner >
    static inline SetterBuilder<T,Owner> createWriteFieldFromMethod(void (Owner::*g)(T))
    {
        UNUSED(g);
        return SetterBuilder<T,Owner>();
    }
};

template< typename _Reader >
class PropertyBuilder< _Reader, void >
{
public:
    template< typename Writer >
    inline PropertyBuilder< _Reader, Writer > setWriter(const Writer& writer) const
    {
        UNUSED(writer);
        return PropertyBuilder< _Reader, Writer >();
    }
    inline refptr<Property> buildProperty() const;
};

template< typename _Writer >
class PropertyBuilder< void, _Writer >
{
public:
    template< typename Reader >
    inline PropertyBuilder< Reader, _Writer > setReader(const Reader& reader) const
    {
        UNUSED(reader);
        return PropertyBuilder< Reader, _Writer >();
    }
    inline refptr<Property> buildProperty() const;
};

}}

#include    <rtti/properties/propertybuilder.inl>

/*****************************************************************************/
#endif
