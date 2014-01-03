/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/value.hh>
#include    <package/nodes/reference.hh>

#include    <filesystem/folder.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/value.hh>
#include    <rtti/engine/array.hh>

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
    be_assert(isCompatible(type), "invalid conversion from int to %s" | type);
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
    be_assert(isCompatible(type), "invalid conversion from float to %s" | type);
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
        || be_typeid<inamespace>::type().isA(type);
}

RTTI::Value StringValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from string to %s" | type);
    if (be_typeid< istring >::type().isA(type))
        return RTTI::Value(istring(m_value));
    if (be_typeid< inamespace >::type().isA(type))
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
    return m_value->getType().isA(type);
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
    return be_typeid< weak<const File> >::type().isA(type);
}

RTTI::Value FileValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from %s to %s" | be_typeid< weak<const File> >::type() | type);
    be_forceuse(type);
    return RTTI::Value(m_value);
}



ArrayValue::ArrayValue(const minitl::vector< ref<Value> >& values)
    :   m_values(values)
{
}

ArrayValue::~ArrayValue()
{
}

bool ArrayValue::isCompatible(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Array)
    {
        raw<const RTTI::ObjectInfo> prop = type.metaclass->objects;
        while (prop && prop->name != istring("value_type"))
        {
            prop = prop->next;
        }
        if (prop)
        {
            if (prop->value.type().isA(be_typeid<const RTTI::Type>::type()))
            {
                RTTI::Type valueType = prop->value.as<const RTTI::Type>();
                for (minitl::vector< ref<Value> >::const_iterator it = m_values.begin(); it != m_values.end(); ++it)
                {
                    if (!(*it)->isCompatible(valueType))
                    {
                        return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

RTTI::Value ArrayValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from array to %s" | type);
    raw<const RTTI::ObjectInfo> prop = type.metaclass->objects;
    while (prop && prop->name != istring("value_type"))
    {
        prop = prop->next;
    }
    be_assert(prop, "unable to find the array value type");
    RTTI::Type valueType = prop->value.as<const RTTI::Type>();

    minitl::array<RTTI::Value> v(Arena::temporary(), be_checked_numcast<u32>(m_values.size()));
    for (u32 i = 0; i < m_values.size(); ++i)
    {
        v[i] = m_values[i]->as(valueType);
    }
    return type.metaclass->constructor->doCall(v.begin(), be_checked_numcast<u32>(m_values.size()));
}

}}}
