/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/metaclass.hh>
#include    <rtti/property.hh>
#include    <rtti/method.hh>
#include    <rtti/namespace.hh>
#include    <rtti/helper.hh>

namespace BugEngine { namespace RTTI
{

class MetaClass::StaticProperty : public Property
{
private:
    Value   m_value;
public:
    StaticProperty(Value v);
    ~StaticProperty();

    virtual bool                readable(weak<Object> /*from*/) const override                    { return true; };
    virtual bool                writable(weak<Object> /*from*/) const override                    { return false; };
    virtual void                set(weak<Object> /*dest*/, const Value& /*value*/) const override { be_notreached(); }
    virtual Value               get(weak<Object> /*from*/) const override                         { return m_value; }
};

MetaClass::StaticProperty::StaticProperty(Value v)
    :   m_value(v)
{
}

MetaClass::StaticProperty::~StaticProperty()
{
}

//-----------------------------------------------------------------------------

MetaMetaClass::MetaMetaClass(const inamespace& name, ref<const Object::MetaClass> parent)
    :   MetaClass(name, parent, ref<MetaMetaClass>(), false)
{
    if(!parent)
    {
        addMethod("name", BugEngine::RTTI::_::createMethodBuilder(&Object::MetaClass::name).buildMethod<&Object::MetaClass::name>());
        addMethod("create", BugEngine::RTTI::_::createMethodBuilder(&Object::MetaClass::create).buildMethod<&Object::MetaClass::create>());
    }
}

MetaMetaClass::~MetaMetaClass()
{
}

//-----------------------------------------------------------------------------

MetaClass::MetaClass(const inamespace& name, ref<const MetaClass> parent, ref<MetaMetaClass> mc, bool registerClass)
    :   m_name(name[name.size()-1])
    ,   m_parent(parent)
    ,   m_metaclass(mc)
    ,   m_properties()
{
    if(parent)
        m_properties = parent->m_properties;
    else if(m_metaclass)
        addMethod("metaclass", BugEngine::RTTI::_::createMethodBuilder(&Object::metaclass).buildMethod<&Object::metaclass>());

    if(registerClass)
    {
        m_dbOwner = Namespace::root()->insert(name, Value(weak<Object>(this)));
    }
}

MetaClass::~MetaClass()
{
    if(m_dbOwner)
        m_dbOwner->set(0, Value());
}

const istring& MetaClass::name() const
{
    return m_name;
}

ref<const MetaClass> MetaClass::metaclass() const
{
    return m_metaclass;
}

ref<const MetaClass> MetaClass::parent() const
{
    return m_parent;
}

void MetaClass::addProperty(const istring& name, ref<const Property> prop)
{
    std::pair<PropertyIterator,bool> result = m_properties.insert(std::make_pair(name, prop));
    be_assert(result.second, "could not register property %s; a property of that name already exists in class %s" | name.c_str() | this->name());
}

void MetaClass::addMethod(const istring& name, ref<Method> method)
{
    addProperty(name, ref<StaticProperty>::create(Value(ref<Object>(method))));
}

weak<const Property> Object::MetaClass::getProperty(const istring& name) const
{
    PropertyConstIterator it = m_properties.find(name);
    if(it != m_properties.end())
        return it->second;
    else
        return weak<const Property>();
}

Value MetaClass::call(Value* /*params*/, size_t /*nbParams*/) const
{
    be_notreached();
    return Value();
}

void MetaClass::init(weak<MetaClass> /*mc*/)
{
}

ref<Object> MetaClass::create() const
{
    be_notreached();
    return ref<Object>();
}

}}
