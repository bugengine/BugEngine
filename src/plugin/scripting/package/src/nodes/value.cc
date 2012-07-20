/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/value.hh>
#include    <package/nodes/reference.hh>

#include    <system/file/folder.script.hh>

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
    return be_typeid<bool>::type().isA(type);
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
    return be_typeid<i8>::type().isA(type)
        || be_typeid<i16>::type().isA(type)
        || be_typeid<i32>::type().isA(type)
        || be_typeid<i64>::type().isA(type)
        || be_typeid<u8>::type().isA(type)
        || be_typeid<u16>::type().isA(type)
        || be_typeid<u32>::type().isA(type)
        || be_typeid<u64>::type().isA(type);
}

RTTI::Value IntValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from int to %s" | type.name());
    if (be_typeid<i8>::type().isA(type))
        return RTTI::Value(be_checked_numcast<i8>(m_value));
    if (be_typeid<i16>::type().isA(type))
        return RTTI::Value(be_checked_numcast<i16>(m_value));
    if (be_typeid<i32>::type().isA(type))
        return RTTI::Value(be_checked_numcast<i32>(m_value));
    if (be_typeid<i64>::type().isA(type))
        return RTTI::Value(be_checked_numcast<i64>(m_value));
    if (be_typeid<u8>::type().isA(type))
        return RTTI::Value(be_checked_numcast<u8>(m_value));
    if (be_typeid<u16>::type().isA(type))
        return RTTI::Value(be_checked_numcast<u16>(m_value));
    if (be_typeid<u32>::type().isA(type))
        return RTTI::Value(be_checked_numcast<u32>(m_value));
    if (be_typeid<u64>::type().isA(type))
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
    return be_typeid<float>::type().isA(type)
        || be_typeid<double>::type().isA(type);
}

RTTI::Value FloatValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from float to %s" | type.name());
    if (be_typeid<float>::type().isA(type))
        return RTTI::Value((float)m_value);
    else
        return RTTI::Value((double)m_value);
}




StringValue::StringValue(const char* value)
    :   m_value(Arena::packageBuilder().strdup(value))
{
}

StringValue::~StringValue()
{
    Arena::packageBuilder().free(m_value);
}

bool StringValue::isCompatible(const RTTI::Type& type) const
{
    return be_typeid<istring>::type().isA(type)
        || be_typeid<inamespace>::type().isA(type)
        || be_typeid< BugEngine::Debug::Format<> >::type().isA(type);
}

RTTI::Value StringValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from string to %s" | type.name());
    if (be_typeid< istring >::type().isA(type))
        return RTTI::Value(istring(m_value));
    if (be_typeid< inamespace >::type().isA(type))
        return RTTI::Value(inamespace(m_value));
    if (be_typeid< BugEngine::Debug::Format<> >::type().isA(type))
        return RTTI::Value(BugEngine::Debug::Format<>(m_value));
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
    return m_value->getType().isA(type);
}

RTTI::Value ReferenceValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from %s to %s" | m_value->getType().name() | type.name());
    return m_value->getValue();
}





FileValue::FileValue(weak<Folder> folder, const char *value)
    :   m_value(folder->openFile(ifilename(value)))
{
    if (!m_value)
        be_error("%s: file not found"|value);
}

FileValue::~FileValue()
{
}

bool FileValue::isCompatible(const RTTI::Type& type) const
{
    return be_typeid< weak<const File> >::type().isA(type);
}

RTTI::Value FileValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from %s to %s" | be_typeid< weak<const File> >::type().name() | type.name());
    return RTTI::Value(m_value);
}

}}}
