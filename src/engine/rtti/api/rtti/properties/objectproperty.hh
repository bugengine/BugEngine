/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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

    virtual bool                readable(weak<Object> from) const override;
    virtual bool                writable(weak<Object> from) const override;
    virtual void                set(weak<Object> dest, const Value& value) const override;
    virtual Value               get(weak<Object> from) const override;

    virtual ref<const RTTI::MetaClass> metaclass() const override;
    static  ref<const MetaClass>       static_metaclass();
private:
    ObjectProperty(const Property& other);
    ObjectProperty& operator=(const Property& other);
};

}}

#include    <rtti/properties/objectproperty.inl>

/*****************************************************************************/
#endif
