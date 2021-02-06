/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CONVERSION_SCRIPT_HH_
#define BE_RTTI_CONVERSION_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>

namespace BugEngine { namespace RTTI {

struct Type;

struct be_api(RTTI) ConversionCost
{
public:
    u8  incompatible;
    u8  variant;
    u16 conversion;
    u16 promotion;
    u16 qualification;

    ConversionCost(u16 qualification = 0, u16 promotion = 0, u16 conversion = 0, u8 variant = 0,
                   u8 incompatible = 0)
        : incompatible(incompatible)
        , variant(variant)
        , conversion(conversion)
        , promotion(promotion)
        , qualification(qualification)
    {
    }

    u64 value() const
    {
        return (u64(qualification)) | (u64(promotion) << 16) | (u64(conversion) << 32)
               | (u64(variant) << 48) | (u64(incompatible) << 56);
    }

    bool operator==(ConversionCost other) const
    {
        return value() == other.value();
    }
    bool operator!=(ConversionCost other) const
    {
        return value() != other.value();
    }
    bool operator<(ConversionCost other) const
    {
        return value() < other.value();
    }
    bool operator<=(ConversionCost other) const
    {
        return value() <= other.value();
    }
    bool operator>(ConversionCost other) const
    {
        return value() > other.value();
    }
    bool operator>=(ConversionCost other) const
    {
        return value() >= other.value();
    }
    ConversionCost operator+=(ConversionCost other)
    {
        incompatible  = be_checked_numcast< u8 >(incompatible + other.incompatible);
        variant       = be_checked_numcast< u8 >(incompatible + other.variant);
        conversion    = be_checked_numcast< u16 >(conversion + other.conversion);
        promotion     = be_checked_numcast< u16 >(promotion + other.promotion);
        qualification = be_checked_numcast< u16 >(qualification + other.qualification);
        return *this;
    }

    static const ConversionCost s_incompatible;
    static const ConversionCost s_variant;
};

template < typename T >
ConversionCost calculateConversion(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< u8 >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< i8 >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< u16 >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< i16 >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< u32 >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< i32 >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< u64 >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< i64 >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< bool >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< float >(const Type& targetType);

template <>
be_api(RTTI) ConversionCost calculateConversion< double >(const Type& targetType);

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
