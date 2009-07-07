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

#ifndef BE_RTTI_OBJECTPROPERTY_HH_
#define BE_RTTI_OBJECTPROPERTY_HH_
/*****************************************************************************/
#include    <rtti/property.hh>
#include    <rtti/marshallers/marshallers.hh>
#include    <rtti/properties/getters.hh>
#include    <rtti/properties/setters.hh>

namespace BugEngine { namespace RTTI
{

template< typename OWNER,
          typename T,
          typename ValueGetter = GetImpossible<OWNER,T>,
          typename ValueSetter = SetImpossible<OWNER,T> >
class ObjectProperty : public Property
{
protected:
    class MetaClass : public Property::MetaClass
    {
    public:
        MetaClass();
        ~MetaClass();
    };
private:
    RTTI::Marshaller<T>         m_marshaller;
public:
    inline ObjectProperty();
    virtual ~ObjectProperty();

    virtual bool                readable(Object* from) const override;
    virtual bool                writable(Object* from) const override;
    virtual void                set(Object* dest, const Value& value) const override;
    virtual Value               get(Object* from) const override;

    virtual const MetaClass*    metaclass() const override;
    static  const MetaClass*    static_metaclass();
private:
    ObjectProperty(const Property& other);
    ObjectProperty& operator=(const Property& other);
};

}}

#include    <rtti/properties/objectproperty.inl>

/*****************************************************************************/
#endif
