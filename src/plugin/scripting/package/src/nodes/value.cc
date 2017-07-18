/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/value.hh>
#include    <package/nodes/reference.hh>
#include    <package/nodes/object.hh>

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


Int2Value::Int2Value(i64 x, i64 y)
    :   m_value(make_bigint2(x, y))
{
}

Int2Value::~Int2Value()
{
}

bool Int2Value::isCompatible(const RTTI::Type& type) const
{
    return be_typeid<byte2>::type().isA(type)
        || be_typeid<short2>::type().isA(type)
        || be_typeid<int2>::type().isA(type)
        || be_typeid<bigint2>::type().isA(type)
        || be_typeid<ushort2>::type().isA(type)
        || be_typeid<uint2>::type().isA(type)
        || be_typeid<biguint2>::type().isA(type);
}

RTTI::Value Int2Value::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from int to %s" | type);
    if (be_typeid<byte2>::type().isA(type))
        return RTTI::Value(make_byte2(be_checked_numcast<i8>(m_value[0]), be_checked_numcast<i8>(m_value[1])));
    if (be_typeid<short2>::type().isA(type))
        return RTTI::Value(make_short2(be_checked_numcast<i16>(m_value[0]), be_checked_numcast<i16>(m_value[1])));
    if (be_typeid<int2>::type().isA(type))
        return RTTI::Value(make_int2(be_checked_numcast<i32>(m_value[0]), be_checked_numcast<i32>(m_value[1])));
    if (be_typeid<bigint2>::type().isA(type))
        return RTTI::Value(make_bigint2(be_checked_numcast<i64>(m_value[0]), be_checked_numcast<i64>(m_value[1])));
    if (be_typeid<ushort2>::type().isA(type))
        return RTTI::Value(make_ushort2(be_checked_numcast<u16>(m_value[0]), be_checked_numcast<u16>(m_value[1])));
    if (be_typeid<uint2>::type().isA(type))
        return RTTI::Value(make_uint2(be_checked_numcast<u32>(m_value[0]), be_checked_numcast<u32>(m_value[1])));
    if (be_typeid<biguint2>::type().isA(type))
        return RTTI::Value(make_biguint2(be_checked_numcast<u64>(m_value[0]), be_checked_numcast<u64>(m_value[1])));
    return RTTI::Value();
}

Int3Value::Int3Value(i64 x, i64 y, i64 z)
    :   m_value(make_bigint3(x, y, z))
{
}

Int3Value::~Int3Value()
{
}

bool Int3Value::isCompatible(const RTTI::Type& type) const
{
    return be_typeid<byte3>::type().isA(type)
        || be_typeid<short3>::type().isA(type)
        || be_typeid<int3>::type().isA(type)
        || be_typeid<bigint3>::type().isA(type)
        || be_typeid<ushort3>::type().isA(type)
        || be_typeid<uint3>::type().isA(type)
        || be_typeid<biguint3>::type().isA(type);
}

RTTI::Value Int3Value::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from int to %s" | type);
    if (be_typeid<byte3>::type().isA(type))
        return RTTI::Value(make_byte3(be_checked_numcast<i8>(m_value[0]), be_checked_numcast<i8>(m_value[1]), be_checked_numcast<i8>(m_value[2])));
    if (be_typeid<short3>::type().isA(type))
        return RTTI::Value(make_short3(be_checked_numcast<i16>(m_value[0]), be_checked_numcast<i16>(m_value[1]), be_checked_numcast<i16>(m_value[2])));
    if (be_typeid<int3>::type().isA(type))
        return RTTI::Value(make_int3(be_checked_numcast<i32>(m_value[0]), be_checked_numcast<i32>(m_value[1]), be_checked_numcast<i32>(m_value[2])));
    if (be_typeid<bigint3>::type().isA(type))
        return RTTI::Value(make_bigint3(be_checked_numcast<i64>(m_value[0]), be_checked_numcast<i64>(m_value[1]), be_checked_numcast<i64>(m_value[2])));
    if (be_typeid<ushort3>::type().isA(type))
        return RTTI::Value(make_ushort3(be_checked_numcast<u16>(m_value[0]), be_checked_numcast<u16>(m_value[1]), be_checked_numcast<u16>(m_value[2])));
    if (be_typeid<uint3>::type().isA(type))
        return RTTI::Value(make_uint3(be_checked_numcast<u32>(m_value[0]), be_checked_numcast<u32>(m_value[1]), be_checked_numcast<u32>(m_value[2])));
    if (be_typeid<biguint3>::type().isA(type))
        return RTTI::Value(make_biguint3(be_checked_numcast<u64>(m_value[0]), be_checked_numcast<u64>(m_value[1]), be_checked_numcast<u64>(m_value[2])));
    return RTTI::Value();
}

Int4Value::Int4Value(i64 x, i64 y, i64 z, i64 w)
    :   m_value(make_bigint4(x, y, z, w))
{
}

Int4Value::~Int4Value()
{
}

bool Int4Value::isCompatible(const RTTI::Type& type) const
{
    return be_typeid<byte4>::type().isA(type)
        || be_typeid<short4>::type().isA(type)
        || be_typeid<int4>::type().isA(type)
        || be_typeid<bigint4>::type().isA(type)
        || be_typeid<ushort4>::type().isA(type)
        || be_typeid<uint4>::type().isA(type)
        || be_typeid<biguint4>::type().isA(type);
}

RTTI::Value Int4Value::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from int to %s" | type);
    if (be_typeid<byte4>::type().isA(type))
        return RTTI::Value(make_byte4(be_checked_numcast<i8>(m_value[0]), be_checked_numcast<i8>(m_value[1]), be_checked_numcast<i8>(m_value[2]), be_checked_numcast<i8>(m_value[3])));
    if (be_typeid<short4>::type().isA(type))
        return RTTI::Value(make_short4(be_checked_numcast<i16>(m_value[0]), be_checked_numcast<i16>(m_value[1]), be_checked_numcast<i16>(m_value[2]), be_checked_numcast<i16>(m_value[3])));
    if (be_typeid<int4>::type().isA(type))
        return RTTI::Value(make_int4(be_checked_numcast<i32>(m_value[0]), be_checked_numcast<i32>(m_value[1]), be_checked_numcast<i32>(m_value[2]), be_checked_numcast<i32>(m_value[3])));
    if (be_typeid<bigint4>::type().isA(type))
        return RTTI::Value(make_bigint4(be_checked_numcast<i64>(m_value[0]), be_checked_numcast<i64>(m_value[1]), be_checked_numcast<i64>(m_value[2]), be_checked_numcast<i64>(m_value[3])));
    if (be_typeid<ushort4>::type().isA(type))
        return RTTI::Value(make_ushort4(be_checked_numcast<u16>(m_value[0]), be_checked_numcast<u16>(m_value[1]), be_checked_numcast<u16>(m_value[2]), be_checked_numcast<u16>(m_value[3])));
    if (be_typeid<uint4>::type().isA(type))
        return RTTI::Value(make_uint4(be_checked_numcast<u32>(m_value[0]), be_checked_numcast<u32>(m_value[1]), be_checked_numcast<u32>(m_value[2]), be_checked_numcast<u32>(m_value[3])));
    if (be_typeid<biguint4>::type().isA(type))
        return RTTI::Value(make_biguint4(be_checked_numcast<u64>(m_value[0]), be_checked_numcast<u64>(m_value[1]), be_checked_numcast<u64>(m_value[2]), be_checked_numcast<u64>(m_value[3])));
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



Float2Value::Float2Value(double x, double y)
    :   m_value(make_float2((float)x, (float)y))
{
}

Float2Value::~Float2Value()
{
}

bool Float2Value::isCompatible(const RTTI::Type& type) const
{
    return be_typeid<float2>::type().isA(type);
}

RTTI::Value Float2Value::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from float to %s" | type);
    return RTTI::Value(m_value);
}



Float3Value::Float3Value(double x, double y, double z)
    :   m_value(make_float3((float)x, (float)y, (float)z))
{
}

Float3Value::~Float3Value()
{
}

bool Float3Value::isCompatible(const RTTI::Type& type) const
{
    return be_typeid<float3>::type().isA(type);
}

RTTI::Value Float3Value::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from float to %s" | type);
    return RTTI::Value(m_value);
}



Float4Value::Float4Value(double x, double y, double z, double w)
    :   m_value(make_float4((float)x, (float)y, (float)z, (float)w))
{
}

Float4Value::~Float4Value()
{
}

bool Float4Value::isCompatible(const RTTI::Type& type) const
{
    return be_typeid<float4>::type().isA(type);
}

RTTI::Value Float4Value::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from float to %s" | type);
    return RTTI::Value(m_value);
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


ObjectValue::ObjectValue(ref<Object> value)
    :   m_value(value)
{
}

ObjectValue::~ObjectValue()
{
}

bool ObjectValue::isCompatible(const RTTI::Type& type) const
{
    return m_value->getType().isA(type);
}

RTTI::Value ObjectValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), "invalid conversion from %s to %s" | m_value->getType() | type);
    be_forceuse(type);
    return m_value->create();
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
