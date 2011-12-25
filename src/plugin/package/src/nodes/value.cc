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

TypeInfo BoolValue::getType() const
{
    return be_typeid<bool>::type();
}



IntValue::IntValue(i64 value)
    :   m_value(value)
{
}

IntValue::~IntValue()
{
}

TypeInfo IntValue::getType() const
{
    return be_typeid<i64>::type();
}



FloatValue::FloatValue(double value)
    :   m_value(value)
{
}

FloatValue::~FloatValue()
{
}

TypeInfo FloatValue::getType() const
{
    return be_typeid<double>::type();
}



StringValue::StringValue(const istring& value)
    :   m_value(value)
{
}

StringValue::~StringValue()
{
}

TypeInfo StringValue::getType() const
{
    return be_typeid<istring>::type();
}



ReferenceValue::ReferenceValue(ref<const Reference> value)
    :   m_value(value)
{
}

ReferenceValue::~ReferenceValue()
{
}

TypeInfo ReferenceValue::getType() const
{
    return m_value->getType();
}

}}}
