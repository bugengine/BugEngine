/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
    UNUSED(t);
    return GetFromField<Owner, T, offset>();
}
template<typename Owner, size_t offset, typename T>
static inline SetFromField<Owner, T, offset> createWriteFieldFromOffset(T* t)
{
    UNUSED(t);
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
    weak<MetaClass> m_metaclass;
    const char *    m_name;
protected:
    inline BasePropertyBuilder(weak<MetaClass> metaclass, const char *name)
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
    inline PropertyBuilder(weak<MetaClass> metaclass, const char *name)
        : BasePropertyBuilder(metaclass, name)
    {
    }
    inline ~PropertyBuilder()
    {
        m_metaclass->addProperty(m_name, ref<ObjectProperty<Owner, T, Getter, Setter > >::create());
    }
};


template<>
class PropertyBuilder<void, void> : public BasePropertyBuilder
{
public:
    inline PropertyBuilder(weak<MetaClass> metaclass, const char *name)
        : BasePropertyBuilder(metaclass, name)
    {
    }
    inline ~PropertyBuilder()
    {
    }

    template< typename Owner, typename T, size_t offset >
    inline PropertyBuilder<GetFromField<Owner, T, offset>, void> operator[](const GetFromField<Owner, T, offset>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
        return PropertyBuilder<GetFromField<Owner, T, offset>, void>(mc, m_name);
    }
    template< typename Owner, typename T, T (Owner::*Getter)() >
    inline PropertyBuilder<GetFromGetter<Owner, T, Getter>, void> operator[](const GetFromGetter<Owner, T, Getter>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
        return PropertyBuilder<GetFromGetter<Owner, T, Getter>, void>(mc, m_name);
    }
    template< typename Owner, typename T, T (Owner::*Getter)() const >
    inline PropertyBuilder<GetFromGetterConst<Owner, T, Getter>, void> operator[](const GetFromGetterConst<Owner, T, Getter>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
        return PropertyBuilder<GetFromGetterConst<Owner, T, Getter>, void>(mc, m_name);
    }

    template< typename Owner, typename T, size_t offset >
    inline PropertyBuilder<void, SetFromField<Owner, T, offset> > operator[](const SetFromField<Owner, T, offset>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
        return PropertyBuilder<void, SetFromField<Owner, T, offset> >(mc, m_name);
    }
    template< typename Owner, typename T, void (Owner::*Setter)(T) >
    inline PropertyBuilder<void, SetFromSetter<Owner, T, Setter> > operator[](const SetFromSetter<Owner, T, Setter>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
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
    inline PropertyBuilder(weak<MetaClass> metaclass, const char *name)
        : BasePropertyBuilder(metaclass, name)
    {
    }

    inline ~PropertyBuilder()
    {
        if(m_metaclass)
            m_metaclass->addProperty(m_name, ref< ObjectProperty<Owner, T, Getter, SetImpossible<Owner,T> > >::create());
    }

    template< typename Owner, typename T, size_t offset >
    inline PropertyBuilder<Getter, SetFromField<Owner, T, offset> > operator[](const SetFromField<Owner, T, offset>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
        return PropertyBuilder<Getter, SetFromField<Owner, T, offset> >(mc, m_name);
    }
    template< typename Owner, typename T, void (Owner::*Setter)(T) >
    inline PropertyBuilder<Getter, SetFromSetter<Owner, T, Setter> > operator[](const SetFromSetter<Owner, T, Setter>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
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
    inline PropertyBuilder(weak<MetaClass> metaclass, const char *name)
        : BasePropertyBuilder(metaclass, name)
    {
    }
    inline ~PropertyBuilder()
    {
        if(m_metaclass)
            m_metaclass->addProperty(m_name, ref< new ObjectProperty<Owner, T, GetImpossible<Owner,T>, Setter > >::create());
    }

    template< size_t offset >
    inline PropertyBuilder<GetFromField<Owner, T, offset>, Setter> operator[](const GetFromField<Owner, T, offset>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
        return PropertyBuilder<GetFromField<Owner, T, offset>, Setter>(mc, m_name);
    }
    template< T (Owner::*Getter)() >
    inline PropertyBuilder<GetFromGetter<Owner, T, Getter>, void> operator[](const GetFromGetter<Owner, T, Getter>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
        return PropertyBuilder<GetFromGetter<Owner, T, Getter>, Setter>(mc, m_name);
    }
    template< T (Owner::*Getter)() const >
    inline PropertyBuilder<GetFromGetterConst<Owner, T, Getter>, void> operator[](const GetFromGetterConst<Owner, T, Getter>& /*just for type detection*/)
    {
        weak<Object::MetaClass> mc = m_metaclass;
        m_metaclass.clear();
        return PropertyBuilder<GetFromGetterConst<Owner, T, Getter>, Setter>(mc, m_name);
    }
};

}}}

/*****************************************************************************/
#endif
