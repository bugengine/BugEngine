/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/conversion.script.hh>
#include    <rtti/typeinfo.script.hh>

namespace BugEngine { namespace RTTI
{

const ConversionCost ConversionCost::s_incompatible(0, 0, 0, 0, 1);
const ConversionCost ConversionCost::s_variant(0, 0, 0, 1, 0);

template< >
be_api(RTTI)
ConversionCost calculateConversion<u8>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool:
            cost.conversion += 1;
            break;
        case ClassIndex_i8:
            cost.promotion += 2;
            break;
        case ClassIndex_u8:
            break;
        case ClassIndex_u16:
        case ClassIndex_i16:
        case ClassIndex_u32:
        case ClassIndex_i32:
        case ClassIndex_u64:
        case ClassIndex_i64:
            cost.promotion += 1;
            break;
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<i8>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool:
            cost.conversion += 1;
            break;
        case ClassIndex_u8:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
            cost.promotion += 2;
            break;
        case ClassIndex_i8:
            break;
        case ClassIndex_i16:
        case ClassIndex_i32:
        case ClassIndex_i64:
            cost.promotion += 1;
            break;
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<u16>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool:
            cost.conversion += 1;
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
            cost.promotion += 2;
            break;
        case ClassIndex_u16:
            break;
        case ClassIndex_i32:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i64:
            cost.promotion += 1;
            break;
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<i16>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool:
            cost.conversion += 1;
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
            cost.promotion += 2;
            break;
        case ClassIndex_i16:
            break;
        case ClassIndex_i32:
        case ClassIndex_i64:
            cost.promotion += 1;
            break;
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<u32>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool:
            cost.conversion += 1;
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_i32:
            cost.promotion += 2;
            break;
        case ClassIndex_u32:
            break;
        case ClassIndex_i64:
        case ClassIndex_u64:
            cost.promotion += 1;
            break;
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<i32>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool:
            cost.conversion += 1;
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
            cost.promotion += 2;
            break;
        case ClassIndex_i32:
            break;
        case ClassIndex_i64:
            cost.promotion += 1;
            break;
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<u64>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool:
            cost.conversion += 1;
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_i32:
        case ClassIndex_u32:
        case ClassIndex_i64:
            cost.promotion += 2;
            break;
        case ClassIndex_u64:
            break;
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<i64>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool:
            cost.conversion += 1;
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i32:
            cost.promotion += 2;
            break;
        case ClassIndex_i64:
            break;
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<bool>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_bool:
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i32:
        case ClassIndex_i64:
            cost.promotion += 1;
            break;
        case ClassIndex_float:
        case ClassIndex_double:
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<float>(const RTTI::Type& targetType)
{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
            break;
        case ClassIndex_double:
            cost.promotion += 1;
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i32:
        case ClassIndex_i64:
            cost.conversion += 1;
            break;
        case ClassIndex_bool:
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

template< >
be_api(RTTI)
ConversionCost calculateConversion<double>(const RTTI::Type& targetType)

{
    if (targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch (targetType.metaclass->index())
        {
        case ClassIndex_float:
            cost.promotion += 2;
            break;
        case ClassIndex_double:
            break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i32:
        case ClassIndex_i64:
            cost.conversion += 1;
            break;
        case ClassIndex_bool:
        default:
            return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if (targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

}}
