/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/conversion.hh>
#include <bugengine/rtti/typeinfo.script.hh>

namespace BugEngine { namespace RTTI {

const ConversionCost ConversionCost::s_incompatible(0, 0, 0, 0, 1);
const ConversionCost ConversionCost::s_variant(0, 0, 0, 1, 0);

be_api(RTTI) ConversionCost ConversionCalculator< u8 >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool: cost.conversion += 1; break;
        case ClassIndex_i8: cost.promotion += 2; break;
        case ClassIndex_u8: break;
        case ClassIndex_u16:
        case ClassIndex_i16:
        case ClassIndex_u32:
        case ClassIndex_i32:
        case ClassIndex_u64:
        case ClassIndex_i64: cost.promotion += 1; break;
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< i8 >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool: cost.conversion += 1; break;
        case ClassIndex_u8:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64: cost.promotion += 2; break;
        case ClassIndex_i8: break;
        case ClassIndex_i16:
        case ClassIndex_i32:
        case ClassIndex_i64: cost.promotion += 1; break;
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< u16 >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool: cost.conversion += 1; break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16: cost.promotion += 2; break;
        case ClassIndex_u16: break;
        case ClassIndex_i32:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i64: cost.promotion += 1; break;
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< i16 >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool: cost.conversion += 1; break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64: cost.promotion += 2; break;
        case ClassIndex_i16: break;
        case ClassIndex_i32:
        case ClassIndex_i64: cost.promotion += 1; break;
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< u32 >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool: cost.conversion += 1; break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_i32: cost.promotion += 2; break;
        case ClassIndex_u32: break;
        case ClassIndex_i64:
        case ClassIndex_u64: cost.promotion += 1; break;
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< i32 >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool: cost.conversion += 1; break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64: cost.promotion += 2; break;
        case ClassIndex_i32: break;
        case ClassIndex_i64: cost.promotion += 1; break;
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< u64 >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool: cost.conversion += 1; break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_i32:
        case ClassIndex_u32:
        case ClassIndex_i64: cost.promotion += 2; break;
        case ClassIndex_u64: break;
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< i64 >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float:
        case ClassIndex_double:
        case ClassIndex_bool: cost.conversion += 1; break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i32: cost.promotion += 2; break;
        case ClassIndex_i64: break;
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< bool >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_bool: break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i32:
        case ClassIndex_i64: cost.promotion += 1; break;
        case ClassIndex_float:
        case ClassIndex_double:
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< float >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float: break;
        case ClassIndex_double: cost.promotion += 1; break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i32:
        case ClassIndex_i64: cost.conversion += 1; break;
        case ClassIndex_bool:
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

be_api(RTTI) ConversionCost ConversionCalculator< double >::calculate(const RTTI::Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Number)
    {
        ConversionCost cost;
        switch(targetType.metaclass->index())
        {
        case ClassIndex_float: cost.promotion += 2; break;
        case ClassIndex_double: break;
        case ClassIndex_i8:
        case ClassIndex_u8:
        case ClassIndex_i16:
        case ClassIndex_u16:
        case ClassIndex_u32:
        case ClassIndex_u64:
        case ClassIndex_i32:
        case ClassIndex_i64: cost.conversion += 1; break;
        case ClassIndex_bool:
        default: return ConversionCost::s_incompatible;
        }
        return cost;
    }
    else if(targetType.metaclass->type() == ClassType_Variant)
    {
        return ConversionCost::s_variant;
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

}}  // namespace BugEngine::RTTI
