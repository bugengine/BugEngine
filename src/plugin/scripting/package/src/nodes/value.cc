/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/value.hh>
#include    <package/nodes/reference.hh>
#include    <package/nodes/object.hh>

#include    <filesystem/folder.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/value.hh>
#include    <rtti/engine/array.factory.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

template< typename U, typename V, u32 DIM >
struct make_value
{
    static inline RTTI::Value make(_Kernel::array<V, DIM> value);
};

template< typename U, typename V >
struct make_value<U, V, 2>
{
    static inline RTTI::Value make(_Kernel::array<V, 2> value)
    {
        _Kernel::array<U, 2> result = {{ be_checked_numcast<U>(value[0]),
                                         be_checked_numcast<U>(value[1])}};
        return RTTI::Value(result);
    }
};
template< typename U, typename V >
struct make_value<U, V, 3>
{
    static inline RTTI::Value make(_Kernel::array<V, 3> value)
    {
        _Kernel::array<U, 3> result = {{ be_checked_numcast<U>(value[0]),
                                         be_checked_numcast<U>(value[1]),
                                         be_checked_numcast<U>(value[2])}};
        return RTTI::Value(result);
    }
};
template< typename U, typename V >
struct make_value<U, V, 4>
{
    static inline RTTI::Value make(_Kernel::array<V, 4> value)
    {
        _Kernel::array<U, 4> result = {{ be_checked_numcast<U>(value[0]),
                                         be_checked_numcast<U>(value[1]),
                                         be_checked_numcast<U>(value[2]),
                                         be_checked_numcast<U>(value[3])}};
        return RTTI::Value(result);
    }
};
template< typename U, typename V >
struct make_value<U, V, 8>
{
    static inline RTTI::Value make(_Kernel::array<V, 8> value)
    {
        _Kernel::array<U, 8> result = {{ be_checked_numcast<U>(value[0]),
                                         be_checked_numcast<U>(value[1]),
                                         be_checked_numcast<U>(value[2]),
                                         be_checked_numcast<U>(value[3]),
                                         be_checked_numcast<U>(value[4]),
                                         be_checked_numcast<U>(value[5]),
                                         be_checked_numcast<U>(value[6]),
                                         be_checked_numcast<U>(value[7])}};
        return RTTI::Value(result);
    }
};
template< typename U, typename V >
struct make_value<U, V, 16>
{
    static inline RTTI::Value make(_Kernel::array<V, 16> value)
    {
        _Kernel::array<U, 16> result = {{ be_checked_numcast<U>(value[0]),
                                          be_checked_numcast<U>(value[1]),
                                          be_checked_numcast<U>(value[2]),
                                          be_checked_numcast<U>(value[3]),
                                          be_checked_numcast<U>(value[4]),
                                          be_checked_numcast<U>(value[5]),
                                          be_checked_numcast<U>(value[6]),
                                          be_checked_numcast<U>(value[7]),
                                          be_checked_numcast<U>(value[8]),
                                          be_checked_numcast<U>(value[9]),
                                          be_checked_numcast<U>(value[10]),
                                          be_checked_numcast<U>(value[11]),
                                          be_checked_numcast<U>(value[12]),
                                          be_checked_numcast<U>(value[13]),
                                          be_checked_numcast<U>(value[14]),
                                          be_checked_numcast<U>(value[15])}};
        return RTTI::Value(result);
    }
};


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

RTTI::ConversionCost BoolValue::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Number)
    {
        RTTI::ConversionCost cost;
        switch (type.metaclass->index())
        {
        case RTTI::ClassIndex_bool:
            break;
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
            cost.conversion += 1;
            break;
        case RTTI::ClassIndex_float:
        case RTTI::ClassIndex_double:
            cost = RTTI::ConversionCost::s_incompatible;
            break;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value BoolValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from bool to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_bool: return RTTI::Value(m_value);
    case RTTI::ClassIndex_u8:   return RTTI::Value(m_value ? (u8)1 : (u8)0);
    case RTTI::ClassIndex_i8:   return RTTI::Value(m_value ? (i8)1 : (i8)0);
    case RTTI::ClassIndex_u16:  return RTTI::Value(m_value ? (u16)1 : (u16)0);
    case RTTI::ClassIndex_i16:  return RTTI::Value(m_value ? (i16)1 : (i16)0);
    case RTTI::ClassIndex_u32:  return RTTI::Value(m_value ? (u32)1 : (u32)0);
    case RTTI::ClassIndex_i32:  return RTTI::Value(m_value ? (i32)1 : (i32)0);
    case RTTI::ClassIndex_u64:  return RTTI::Value(m_value ? (u64)1 : (u64)0);
    case RTTI::ClassIndex_i64:  return RTTI::Value(m_value ? (i64)1 : (i64)0);
    default: be_notreached(); return RTTI::Value(false);
    }
}




IntValue::IntValue(i64 value)
    :   m_value(value)
{
}

IntValue::~IntValue()
{
}

RTTI::ConversionCost IntValue::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Number)
    {
        RTTI::ConversionCost cost;
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_float:
        case RTTI::ClassIndex_double:
        case RTTI::ClassIndex_bool:
            cost.conversion += 1;
            break;
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
            cost.promotion += 2;
            break;
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
            cost.promotion += 1;
            break;
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
            break;
        default:
            be_notreached();
            cost = RTTI::ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value IntValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from int to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_bool:     return RTTI::Value(m_value != 0);
    case RTTI::ClassIndex_u8:       return RTTI::Value(be_checked_numcast<u8>(m_value));
    case RTTI::ClassIndex_i8:       return RTTI::Value(be_checked_numcast<i8>(m_value));
    case RTTI::ClassIndex_u16:      return RTTI::Value(be_checked_numcast<u16>(m_value));
    case RTTI::ClassIndex_i16:      return RTTI::Value(be_checked_numcast<i16>(m_value));
    case RTTI::ClassIndex_u32:      return RTTI::Value(be_checked_numcast<u32>(m_value));
    case RTTI::ClassIndex_i32:      return RTTI::Value(be_checked_numcast<i32>(m_value));
    case RTTI::ClassIndex_u64:      return RTTI::Value(be_checked_numcast<u64>(m_value));
    case RTTI::ClassIndex_i64:      return RTTI::Value(be_checked_numcast<i64>(m_value));
    case RTTI::ClassIndex_float:    return RTTI::Value(be_checked_numcast<float>(m_value));
    case RTTI::ClassIndex_double:   return RTTI::Value(be_checked_numcast<double>(m_value));
    default: be_notreached(); return RTTI::Value();
    }
}


Int2Value::Int2Value(i64 x, i64 y)
    :   m_value(make_bigint2(x, y))
{
}

Int2Value::~Int2Value()
{
}

RTTI::ConversionCost Int2Value::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Vector2)
    {
        RTTI::ConversionCost cost;
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_float:
        case RTTI::ClassIndex_double:
            cost.conversion += 1;
            break;
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
            cost.promotion += 2;
            break;
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
            cost.promotion += 1;
            break;
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
            break;
        default:
            be_notreached();
            cost = RTTI::ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value Int2Value::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from int2 to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_u8:       return make_value<u8, i64, 2>::make(m_value);
    case RTTI::ClassIndex_i8:       return make_value<i8, i64, 2>::make(m_value);
    case RTTI::ClassIndex_u16:      return make_value<u16, i64, 2>::make(m_value);
    case RTTI::ClassIndex_i16:      return make_value<i16, i64, 2>::make(m_value);
    case RTTI::ClassIndex_u32:      return make_value<u32, i64, 2>::make(m_value);
    case RTTI::ClassIndex_i32:      return make_value<i32, i64, 2>::make(m_value);
    case RTTI::ClassIndex_u64:      return make_value<u64, i64, 2>::make(m_value);
    case RTTI::ClassIndex_i64:      return make_value<i64, i64, 2>::make(m_value);
    case RTTI::ClassIndex_float:    return make_value<float, i64, 2>::make(m_value);
    case RTTI::ClassIndex_double:   return make_value<double, i64, 2>::make(m_value);
    default: be_notreached(); return RTTI::Value();
    }
}

Int3Value::Int3Value(i64 x, i64 y, i64 z)
    :   m_value(make_bigint3(x, y, z))
{
}

Int3Value::~Int3Value()
{
}

RTTI::ConversionCost Int3Value::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Vector3)
    {
        RTTI::ConversionCost cost;
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_float:
        case RTTI::ClassIndex_double:
        case RTTI::ClassIndex_bool:
            cost.conversion += 1;
            break;
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
            cost.promotion += 2;
            break;
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
            cost.promotion += 1;
            break;
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
            break;
        default:
            be_notreached();
            cost = RTTI::ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value Int3Value::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from int3 to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_u8:       return make_value<u8, i64, 3>::make(m_value);
    case RTTI::ClassIndex_i8:       return make_value<i8, i64, 3>::make(m_value);
    case RTTI::ClassIndex_u16:      return make_value<u16, i64, 3>::make(m_value);
    case RTTI::ClassIndex_i16:      return make_value<i16, i64, 3>::make(m_value);
    case RTTI::ClassIndex_u32:      return make_value<u32, i64, 3>::make(m_value);
    case RTTI::ClassIndex_i32:      return make_value<i32, i64, 3>::make(m_value);
    case RTTI::ClassIndex_u64:      return make_value<u64, i64, 3>::make(m_value);
    case RTTI::ClassIndex_i64:      return make_value<i64, i64, 3>::make(m_value);
    case RTTI::ClassIndex_float:    return make_value<float, i64, 3>::make(m_value);
    case RTTI::ClassIndex_double:   return make_value<double, i64, 3>::make(m_value);
    default: be_notreached(); return RTTI::Value();
    }
}

Int4Value::Int4Value(i64 x, i64 y, i64 z, i64 w)
    :   m_value(make_bigint4(x, y, z, w))
{
}

Int4Value::~Int4Value()
{
}

RTTI::ConversionCost Int4Value::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Vector4)
    {
        RTTI::ConversionCost cost;
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_float:
        case RTTI::ClassIndex_double:
        case RTTI::ClassIndex_bool:
            cost.conversion += 1;
            break;
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
            cost.promotion += 2;
            break;
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
            cost.promotion += 1;
            break;
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
            break;
        default:
            be_notreached();
            cost = RTTI::ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value Int4Value::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from int4 to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_u8:       return make_value<u8, i64, 4>::make(m_value);
    case RTTI::ClassIndex_i8:       return make_value<i8, i64, 4>::make(m_value);
    case RTTI::ClassIndex_u16:      return make_value<u16, i64, 4>::make(m_value);
    case RTTI::ClassIndex_i16:      return make_value<i16, i64, 4>::make(m_value);
    case RTTI::ClassIndex_u32:      return make_value<u32, i64, 4>::make(m_value);
    case RTTI::ClassIndex_i32:      return make_value<i32, i64, 4>::make(m_value);
    case RTTI::ClassIndex_u64:      return make_value<u64, i64, 4>::make(m_value);
    case RTTI::ClassIndex_i64:      return make_value<i64, i64, 4>::make(m_value);
    case RTTI::ClassIndex_float:    return make_value<float, i64, 4>::make(m_value);
    case RTTI::ClassIndex_double:   return make_value<double, i64, 4>::make(m_value);
    default: be_notreached(); return RTTI::Value();
    }
}


FloatValue::FloatValue(double value)
    :   m_value(value)
{
}

FloatValue::~FloatValue()
{
}

RTTI::ConversionCost FloatValue::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Number)
    {
        RTTI::ConversionCost cost;
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_float:
            cost.promotion += 1;
            break;
        case RTTI::ClassIndex_double:
            break;
        case RTTI::ClassIndex_bool:
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
            cost.conversion += 1;
            break;
        default:
            be_notreached();
            cost = RTTI::ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value FloatValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from float to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_u8:       return RTTI::Value(be_checked_numcast<u8>(m_value));
    case RTTI::ClassIndex_i8:       return RTTI::Value(be_checked_numcast<i8>(m_value));
    case RTTI::ClassIndex_u16:      return RTTI::Value(be_checked_numcast<u16>(m_value));
    case RTTI::ClassIndex_i16:      return RTTI::Value(be_checked_numcast<i16>(m_value));
    case RTTI::ClassIndex_u32:      return RTTI::Value(be_checked_numcast<u32>(m_value));
    case RTTI::ClassIndex_i32:      return RTTI::Value(be_checked_numcast<i32>(m_value));
    case RTTI::ClassIndex_u64:      return RTTI::Value(be_checked_numcast<u64>(m_value));
    case RTTI::ClassIndex_i64:      return RTTI::Value(be_checked_numcast<i64>(m_value));
    case RTTI::ClassIndex_float:    return RTTI::Value(be_checked_numcast<float>(m_value));
    case RTTI::ClassIndex_double:   return RTTI::Value(be_checked_numcast<double>(m_value));
    default: be_notreached(); return RTTI::Value();
    }
}



Float2Value::Float2Value(double x, double y)
    :   m_value(make_double2(x, y))
{
}

Float2Value::~Float2Value()
{
}

RTTI::ConversionCost Float2Value::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Vector2)
    {
        RTTI::ConversionCost cost;
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_float:
            cost.promotion += 1;
            break;
        case RTTI::ClassIndex_double:
            break;
        case RTTI::ClassIndex_bool:
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
            cost.conversion += 1;
            break;
        default:
            be_notreached();
            cost = RTTI::ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value Float2Value::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from float2 to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_u8:       return make_value<u8, double, 2>::make(m_value);
    case RTTI::ClassIndex_i8:       return make_value<i8, double, 2>::make(m_value);
    case RTTI::ClassIndex_u16:      return make_value<u16, double, 2>::make(m_value);
    case RTTI::ClassIndex_i16:      return make_value<i16, double, 2>::make(m_value);
    case RTTI::ClassIndex_u32:      return make_value<u32, double, 2>::make(m_value);
    case RTTI::ClassIndex_i32:      return make_value<i32, double, 2>::make(m_value);
    case RTTI::ClassIndex_u64:      return make_value<u64, double, 2>::make(m_value);
    case RTTI::ClassIndex_i64:      return make_value<i64, double, 2>::make(m_value);
    case RTTI::ClassIndex_float:    return make_value<float, double, 2>::make(m_value);
    case RTTI::ClassIndex_double:   return make_value<double, double, 2>::make(m_value);
    default: be_notreached(); return RTTI::Value();
    }
}



Float3Value::Float3Value(double x, double y, double z)
    :   m_value(make_double3(x, y, z))
{
}

Float3Value::~Float3Value()
{
}

RTTI::ConversionCost Float3Value::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Vector3)
    {
        RTTI::ConversionCost cost;
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_float:
            cost.promotion += 1;
            break;
        case RTTI::ClassIndex_double:
            break;
        case RTTI::ClassIndex_bool:
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
            cost.conversion += 1;
            break;
        default:
            be_notreached();
            cost = RTTI::ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value Float3Value::as(const RTTI::Type& type) const
{

    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from float3 to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_u8:       return make_value<u8, double, 3>::make(m_value);
    case RTTI::ClassIndex_i8:       return make_value<i8, double, 3>::make(m_value);
    case RTTI::ClassIndex_u16:      return make_value<u16, double, 3>::make(m_value);
    case RTTI::ClassIndex_i16:      return make_value<i16, double, 3>::make(m_value);
    case RTTI::ClassIndex_u32:      return make_value<u32, double, 3>::make(m_value);
    case RTTI::ClassIndex_i32:      return make_value<i32, double, 3>::make(m_value);
    case RTTI::ClassIndex_u64:      return make_value<u64, double, 3>::make(m_value);
    case RTTI::ClassIndex_i64:      return make_value<i64, double, 3>::make(m_value);
    case RTTI::ClassIndex_float:    return make_value<float, double, 3>::make(m_value);
    case RTTI::ClassIndex_double:   return make_value<double, double, 3>::make(m_value);
    default: be_notreached(); return RTTI::Value();
    }
}



Float4Value::Float4Value(double x, double y, double z, double w)
    :   m_value(make_double4(x, y, z, w))
{
}

Float4Value::~Float4Value()
{
}

RTTI::ConversionCost Float4Value::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Vector4)
    {
        RTTI::ConversionCost cost;
        switch(type.metaclass->index())
        {
        case RTTI::ClassIndex_float:
            cost.promotion += 1;
            break;
        case RTTI::ClassIndex_double:
            break;
        case RTTI::ClassIndex_bool:
        case RTTI::ClassIndex_u8:
        case RTTI::ClassIndex_i8:
        case RTTI::ClassIndex_u16:
        case RTTI::ClassIndex_i16:
        case RTTI::ClassIndex_u32:
        case RTTI::ClassIndex_i32:
        case RTTI::ClassIndex_u64:
        case RTTI::ClassIndex_i64:
            cost.conversion += 1;
            break;
        default:
            be_notreached();
            cost = RTTI::ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value Float4Value::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from float4 to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(m_value);
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_u8:       return make_value<u8, double, 4>::make(m_value);
    case RTTI::ClassIndex_i8:       return make_value<i8, double, 4>::make(m_value);
    case RTTI::ClassIndex_u16:      return make_value<u16, double, 4>::make(m_value);
    case RTTI::ClassIndex_i16:      return make_value<i16, double, 4>::make(m_value);
    case RTTI::ClassIndex_u32:      return make_value<u32, double, 4>::make(m_value);
    case RTTI::ClassIndex_i32:      return make_value<i32, double, 4>::make(m_value);
    case RTTI::ClassIndex_u64:      return make_value<u64, double, 4>::make(m_value);
    case RTTI::ClassIndex_i64:      return make_value<i64, double, 4>::make(m_value);
    case RTTI::ClassIndex_float:    return make_value<float, double, 4>::make(m_value);
    case RTTI::ClassIndex_double:   return make_value<double, double, 4>::make(m_value);
    default: be_notreached(); return RTTI::Value();
    }
}




StringValue::StringValue(const char* value)
    :   m_value(Arena::packageBuilder().strdup(value))
{
}

StringValue::~StringValue()
{
    Arena::packageBuilder().free(m_value);
}

RTTI::ConversionCost StringValue::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_String)
    {
        RTTI::ConversionCost cost;
        return cost;
    }
    else if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::ConversionCost::s_variant;
    }
    else
    {
        return RTTI::ConversionCost::s_incompatible;
    }
}

RTTI::Value StringValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from float4 to %s" | type);
    if (type.metaclass->type() == RTTI::ClassType_Variant)
    {
        return RTTI::Value(text(m_value));
    }
    else switch (type.metaclass->index())
    {
    case RTTI::ClassIndex_istring:      return RTTI::Value(istring(m_value));
    case RTTI::ClassIndex_inamespace:   return RTTI::Value(inamespace(m_value));
    case RTTI::ClassIndex_ifilename:    return RTTI::Value(ifilename(m_value));
    case RTTI::ClassIndex_ipath:        return RTTI::Value(ipath(m_value));
    case RTTI::ClassIndex_text:         return RTTI::Value(text(m_value));
    default: be_notreached(); return RTTI::Value();
    }
}



ReferenceValue::ReferenceValue(ref<const Reference> value)
    :   m_value(value)
{
}

ReferenceValue::~ReferenceValue()
{
}

RTTI::ConversionCost ReferenceValue::calculateConversion(const RTTI::Type& type) const
{
    return m_value->getType().calculateConversion(type);
}

RTTI::Value ReferenceValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from %s to %s" | m_value->getType() | type);
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

RTTI::ConversionCost ObjectValue::calculateConversion(const RTTI::Type& type) const
{
    return m_value->getType().calculateConversion(type);
}

RTTI::Value ObjectValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from %s to %s" | m_value->getType() | type);
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

RTTI::ConversionCost FileValue::calculateConversion(const RTTI::Type& type) const
{
    return be_typeid< weak<const File> >::type().calculateConversion(type);
}

RTTI::Value FileValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from %s to %s" | be_typeid< weak<const File> >::type() | type);
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

RTTI::ConversionCost ArrayValue::calculateConversion(const RTTI::Type& type) const
{
    if (type.metaclass->type() == RTTI::ClassType_Array)
    {
        raw<const RTTI::ScriptingArrayAPI> api = type.metaclass->apiMethods->arrayScripting;
        const RTTI::Type& valueType = api->value_type;
        RTTI::ConversionCost c;
        for (minitl::vector< ref<Value> >::const_iterator it = m_values.begin();
             it != m_values.end();
             ++it)
        {
            c += (*it)->calculateConversion(valueType);
        }
        return c;

    }
    return RTTI::ConversionCost::s_incompatible;
}

RTTI::Value ArrayValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from array to %s" | type);
    raw<const RTTI::ScriptingArrayAPI> api = type.metaclass->apiMethods->arrayScripting;
    const RTTI::Type& valueType = api->value_type;
    minitl::array<RTTI::Value> v(Arena::temporary(), be_checked_numcast<u32>(m_values.size()));
    for (u32 i = 0; i < m_values.size(); ++i)
    {
        v[i] = m_values[i]->as(valueType);
    }
    return type.metaclass->constructor->doCall(v.begin(), be_checked_numcast<u32>(m_values.size()));
}

}}}
