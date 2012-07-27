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
    return type.isA<bool>();
}

RTTI::Value BoolValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from bool to %s" | type);
    be_forceuse(type);
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
    return type.isA<i8>()
        || type.isA<i16>()
        || type.isA<i32>()
        || type.isA<i64>()
        || type.isA<u8>()
        || type.isA<u16>()
        || type.isA<u32>()
        || type.isA<u64>();
}

RTTI::Value IntValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from int to %s" | type);
    if (type.isA<i8>())
        return RTTI::Value(be_checked_numcast<i8>(m_value));
    if (type.isA<i16>())
        return RTTI::Value(be_checked_numcast<i16>(m_value));
    if (type.isA<i32>())
        return RTTI::Value(be_checked_numcast<i32>(m_value));
    if (type.isA<i64>())
        return RTTI::Value(be_checked_numcast<i64>(m_value));
    if (type.isA<u8>())
        return RTTI::Value(be_checked_numcast<u8>(m_value));
    if (type.isA<u16>())
        return RTTI::Value(be_checked_numcast<u16>(m_value));
    if (type.isA<u32>())
        return RTTI::Value(be_checked_numcast<u32>(m_value));
    if (type.isA<u64>())
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
    return type.isA<float>()
        || type.isA<double>();
}

RTTI::Value FloatValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from float to %s" | type);
    if (type.isA<float>())
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
    return type.isA<istring>()
        || type.isA<inamespace>();
}

RTTI::Value StringValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from string to %s" | type);
    if (type.isA< istring >())
        return RTTI::Value(istring(m_value));
    if (type.isA< inamespace >())
        return RTTI::Value(inamespace(m_value));
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
    return type.isA(m_value->getType());
}

RTTI::Value ReferenceValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from %s to %s" | m_value->getType() | type);
    be_forceuse(type);
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
    return type.isA< weak<const File> >();
}

RTTI::Value FileValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from %s to %s" | be_typeid< weak<const File> >::type() | type);
    be_forceuse(type);
    return RTTI::Value(m_value);
}

}}}
