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
#include    <rtti/metaclass.hh>
#include    <rtti/properties/objectproperty.hh>
#include    <rtti/properties/getters.hh>
#include    <rtti/properties/setters.hh>

namespace BugEngine { namespace RTTI { namespace _
{

template<typename Owner, size_t offset, typename T>
static inline GetFromField<Owner, T, offset> createReadFieldFromOffset(T* t)
{
    return GetFromField<Owner, T, offset>();
}
template<typename Owner, size_t offset, typename T>
static inline SetFromField<Owner, T, offset> createWriteFieldFromOffset(T* t)
{
    return SetFromField<Owner, T, offset>();
}

template< typename Owner, typename T >
struct CreateReadFieldFromGetterHelper
{
    template< T (Owner::*Getter)() >
    inline GetFromGetter< Owner, T, Getter > operator()()
    {
        return GetFromGetter< Owner, T, Getter >();
    }
};
template< typename Owner, typename T >
struct CreateReadFieldFromGetterConstHelper
{
    template< T (Owner::*Getter)() const >
    inline GetFromGetterConst< Owner, T, Getter > operator()()
    {
        return GetFromGetterConst< Owner, T, Getter >();
    }
};
template< typename Owner, typename T >
static inline CreateReadFieldFromGetterHelper< Owner, T > createHelperFromGetter(T (Owner::*)())
{
    return CreateReadFieldFromGetterHelper< Owner, T >();
}
template< typename Owner, typename T >
static inline CreateReadFieldFromGetterConstHelper< Owner, T > createHelperFromGetter(T (Owner::*)() const)
{
    return CreateReadFieldFromGetterConstHelper< Owner, T >();
}

template< typename Owner, typename T >
struct CreateWriteFieldFromSetterHelper
{
    template< void (Owner::*Setter)(T) >
    inline SetFromSetter< Owner, T, Setter > operator()()
    {
        return SetFromSetter< Owner, T, Setter >();
    }
};
template< typename Owner, typename T >
static inline CreateWriteFieldFromSetterHelper< Owner, T > createHelperFromSetter(void (Owner::*)(T))
{
    return CreateWriteFieldFromSetterHelper< Owner, T >();
}

class BasePropertyBuilder
{
protected:
    Object::MetaClass * m_metaclass;
    const char *        m_name;
protected:
    inline BasePropertyBuilder(Object::MetaClass* metaclass, const char *name)
        :   m_metaclass(metaclass)
        ,   m_name(name)
    {
    }
    inline ~BasePropertyBuilder()
    {
    }
};

template<typename Getter, typename Setter>
class PropertyBuilder : public BasePropertyBuilder
{
private:
    typedef typename Setter::Owner          Owner;
    typedef typename Setter::PropertyType   T;
public:
    inline PropertyBuilder(Object::MetaClass* metaclass, const char *name)
        : BasePropertyBuilder(metaclass, name)
    {
    }
    inline ~PropertyBuilder()
    {
        Assert(m_metaclass);
        m_metaclass->addProperty(m_name, new ObjectProperty<Owner, T, Getter, Setter >());
    }
};


template<>
class PropertyBuilder<void, void> : public BasePropertyBuilder
{
public:
    inline PropertyBuilder(Object::MetaClass* metaclass, const char *name)
        : BasePropertyBuilder(metaclass, name)
    {
    }
    inline ~PropertyBuilder()
    {
        Assert(m_metaclass == 0);
    }

    template< typename Owner, typename T, size_t offset >
    inline PropertyBuilder<GetFromField<Owner, T, offset>, void> operator[](const GetFromField<Owner, T, offset>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<GetFromField<Owner, T, offset>, void>(mc, m_name);
    }
    template< typename Owner, typename T, T (Owner::*Getter)() >
    inline PropertyBuilder<GetFromGetter<Owner, T, Getter>, void> operator[](const GetFromGetter<Owner, T, Getter>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<GetFromGetter<Owner, T, Getter>, void>(mc, m_name);
    }
    template< typename Owner, typename T, T (Owner::*Getter)() const >
    inline PropertyBuilder<GetFromGetterConst<Owner, T, Getter>, void> operator[](const GetFromGetterConst<Owner, T, Getter>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<GetFromGetterConst<Owner, T, Getter>, void>(mc, m_name);
    }

    template< typename Owner, typename T, size_t offset >
    inline PropertyBuilder<void, SetFromField<Owner, T, offset> > operator[](const SetFromField<Owner, T, offset>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<void, SetFromField<Owner, T, offset> >(mc, m_name);
    }
    template< typename Owner, typename T, void (Owner::*Setter)(T) >
    inline PropertyBuilder<void, SetFromSetter<Owner, T, Setter> > operator[](const SetFromSetter<Owner, T, Setter>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<void, SetFromSetter<Owner, T, Setter> >(mc, m_name);
    }
};

template<class Getter>
class PropertyBuilder<Getter, void> : public BasePropertyBuilder
{
private:
    typedef typename Getter::Owner          Owner;
    typedef typename Getter::PropertyType   T;
public:
    inline PropertyBuilder(Object::MetaClass* metaclass, const char *name)
        : BasePropertyBuilder(metaclass, name)
    {
    }

    inline ~PropertyBuilder()
    {
        if(m_metaclass)
            m_metaclass->addProperty(m_name, new ObjectProperty<Owner, T, Getter, SetImpossible<Owner,T> >());
    }

    template< typename Owner, typename T, size_t offset >
    inline PropertyBuilder<Getter, SetFromField<Owner, T, offset>> operator[](const SetFromField<Owner, T, offset>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<Getter, SetFromField<Owner, T, offset> >(mc, m_name);
    }
    template< typename Owner, typename T, void (Owner::*Setter)(T) >
    inline PropertyBuilder<Getter, SetFromSetter<Owner, T, Setter> > operator[](const SetFromSetter<Owner, T, Setter>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<Getter, SetFromSetter<Owner, T, Setter> >(mc, m_name);
    }
};

template<class Setter>
class PropertyBuilder<void, Setter> : public BasePropertyBuilder
{
private:
    typedef typename Setter::Owner          Owner;
    typedef typename Setter::PropertyType   T;
public:
    inline PropertyBuilder(Object::MetaClass* metaclass, const char *name)
        : BasePropertyBuilder(metaclass, name)
    {
    }
    inline ~PropertyBuilder()
    {
        if(m_metaclass)
            m_metaclass->addProperty(m_name, new ObjectProperty<Owner, T, GetImpossible<Owner,T>, Setter >());
    }

    template< size_t offset >
    inline PropertyBuilder<GetFromField<Owner, T, offset>, Setter> operator[](const GetFromField<Owner, T, offset>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<GetFromField<Owner, T, offset>, Setter>(mc, m_name);
    }
    template< T (Owner::*Getter)() >
    inline PropertyBuilder<GetFromGetter<Owner, T, Getter>, void> operator[](const GetFromGetter<Owner, T, Getter>& o)
    {
        Object::MetaClass* mc = m_metaclass;
        m_metaclass = 0;
        return PropertyBuilder<GetFromGetter<Owner, T, Getter>, Setter>(mc, m_name);
	}
	template< T (Owner::*Getter)() const >
	inline PropertyBuilder<GetFromGetterConst<Owner, T, Getter>, void> operator[](const GetFromGetterConst<Owner, T, Getter>& o)
	{
		Object::MetaClass* mc = m_metaclass;
		m_metaclass = 0;
		return PropertyBuilder<GetFromGetterConst<Owner, T, Getter>, Setter>(mc, m_name);
	}
};

}}}

/*****************************************************************************/
#endif
