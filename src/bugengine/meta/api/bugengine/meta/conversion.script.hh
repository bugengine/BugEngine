/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_CONVERSION_SCRIPT_HH_
#define BE_META_CONVERSION_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>

namespace BugEngine { namespace Meta {

struct Type;

struct be_api(META) ConversionCost
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

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
