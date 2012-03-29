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

bool BoolValue::isCompatible(const RTTI::Type& type) const
{
    return type <= be_typeid<bool>::type();
}

RTTI::Value BoolValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from bool to %s" | type.name());
    return RTTI::Value(m_value);
}




IntValue::IntValue(i64 value)
    :   m_value(value)
{
}

IntValue::~IntValue()
{
}

bool IntValue::isCompatible(const RTTI::Type& type) const
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

RTTI::Value IntValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from int to %s" | type.name());
    if (type <= be_typeid<i8>::type())
        return RTTI::Value(be_checked_numcast<i8>(m_value));
    if (type <= be_typeid<i16>::type())
        return RTTI::Value(be_checked_numcast<i16>(m_value));
    if (type <= be_typeid<i32>::type())
        return RTTI::Value(be_checked_numcast<i32>(m_value));
    if (type <= be_typeid<i64>::type())
        return RTTI::Value(be_checked_numcast<i64>(m_value));
    if (type <= be_typeid<u8>::type())
        return RTTI::Value(be_checked_numcast<u8>(m_value));
    if (type <= be_typeid<u16>::type())
        return RTTI::Value(be_checked_numcast<u16>(m_value));
    if (type <= be_typeid<u32>::type())
        return RTTI::Value(be_checked_numcast<u32>(m_value));
    if (type <= be_typeid<u64>::type())
        return RTTI::Value(be_checked_numcast<u64>(m_value));
    return RTTI::Value();
}



FloatValue::FloatValue(double value)
    :   m_value(value)
{
}

FloatValue::~FloatValue()
{
}

bool FloatValue::isCompatible(const RTTI::Type& type) const
{
    return type <= be_typeid<float>::type()
        || type <= be_typeid<double>::type();
}

RTTI::Value FloatValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from float to %s" | type.name());
    if (type <= be_typeid<float>::type())
        return RTTI::Value((float)m_value);
    else
        return RTTI::Value((double)m_value);
}




StringValue::StringValue(const char* value)
    :   m_value(packageBuilderArena().strdup(value))
{
}

StringValue::~StringValue()
{
    packageBuilderArena().free(m_value);
}

bool StringValue::isCompatible(const RTTI::Type& type) const
{
    return type <= be_typeid<istring>::type()
        || type <= be_typeid<inamespace>::type()
        || type <= be_typeid< minitl::format<> >::type();
}

RTTI::Value StringValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from string to %s" | type.name());
    if (type <= be_typeid< istring >::type())
        return RTTI::Value(istring(m_value));
    if (type <= be_typeid< inamespace >::type())
        return RTTI::Value(inamespace(m_value));
    if (type <= be_typeid< minitl::format<> >::type())
        return RTTI::Value(minitl::format<>(m_value));
    return RTTI::Value();
}




ReferenceValue::ReferenceValue(ref<const Reference> value)
    :   m_value(value)
{
}

ReferenceValue::~ReferenceValue()
{
}

bool ReferenceValue::isCompatible(const RTTI::Type& type) const
{
    return type <= m_value->getType();
}

RTTI::Value ReferenceValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from %s to %s" | m_value->getType().name() | type.name());
    return m_value->getValue();
}

}}}
