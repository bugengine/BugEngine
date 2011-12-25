/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/value.hh>
#include    <package/nodes/reference.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Value::Value()
{
}

Value::~Value()
{
}


BoolValue::BoolValue(bool value)
    :   m_value(value)
{
}

BoolValue::~BoolValue()
{
}

bool BoolValue::isCompatible(const TypeInfo& type) const
{
    return type <= be_typeid<bool>::type();
}



IntValue::IntValue(i64 value)
    :   m_value(value)
{
}

IntValue::~IntValue()
{
}

bool IntValue::isCompatible(const TypeInfo& type) const
{
    return type <= be_typeid<i8>::type()
        || type <= be_typeid<i16>::type()
        || type <= be_typeid<i32>::type()
        || type <= be_typeid<i64>::type()
        || type <= be_typeid<u8>::type()
        || type <= be_typeid<u16>::type()
        || type <= be_typeid<u32>::type()
        || type <= be_typeid<u64>::type();
}



FloatValue::FloatValue(double value)
    :   m_value(value)
{
}

FloatValue::~FloatValue()
{
}

bool FloatValue::isCompatible(const TypeInfo& type) const
{
    return type <= be_typeid<float>::type()
        || type <= be_typeid<double>::type();
}



StringValue::StringValue(const char* value)
    :   m_value(packageBuilderArena().strdup(value))
{
}

StringValue::~StringValue()
{
    packageBuilderArena().free(m_value);
}

bool StringValue::isCompatible(const TypeInfo& type) const
{
    return type <= be_typeid<istring>::type()
        || type <= be_typeid<inamespace>::type()
        || type <= be_typeid< minitl::format<> >::type();
}



ReferenceValue::ReferenceValue(ref<const Reference> value)
    :   m_value(value)
{
}

ReferenceValue::~ReferenceValue()
{
}

bool ReferenceValue::isCompatible(const TypeInfo& type) const
{
    return type <= m_value->getType();
}

}}}
