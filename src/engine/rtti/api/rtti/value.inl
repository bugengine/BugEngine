/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_INL_
#define BE_RTTI_VALUE_INL_
/*****************************************************************************/
#include   <rtti/value.hh>
#include   <rtti/typeinfo.hh>
#include   <rtti/propertyinfo.hh>

namespace BugEngine
{

Value::PropertyProxy::PropertyProxy(Value& v, weak<const PropertyInfo> p)
    :   from(v), prop(p)
{
}
Value::PropertyProxy::operator Value()
{
    return prop->get(from.pointer());
}
void Value::PropertyProxy::operator=(const Value& v)
{
    prop->set(v, from.pointer());
}


template< typename T >
Value::Value(const T& t)
:   m_type(be_typeid<T>())
{
    if(m_type->size() > sizeof(m_buffer))
    {
        *(void**)(m_buffer) = Malloc::internalAlloc(m_type->size(), 16);
    }
    m_type->copy(&t, pointer());
}

Value::~Value()
{
    m_type->destroy(pointer());
}

void* Value::pointer()
{
    if(m_type->size() > sizeof(m_buffer))
    {
        return *(void**)(m_buffer);
    }
    else
    {
        return (void*)m_buffer;
    }
}

const void* Value::pointer() const
{
    if(m_type->size() > sizeof(m_buffer))
    {
        return *(const void**)(m_buffer);
    }
    else
    {
        return (const void*)m_buffer;
    }
}

template< typename T >
const T& Value::as() const
{
    be_assert(be_typeid<T>() == m_type, "Value has type %s; unable to unbox to type %s" | m_type->name | be_typeid<T>()->name);
    return *(const T*)pointer();
}

Value::PropertyProxy Value::operator[](istring prop)
{
    return PropertyProxy(*this, m_type->getProperty(prop));
}

}

/*****************************************************************************/
#endif
