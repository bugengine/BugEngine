/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_CONVERSION_HH_
#define BE_META_CONVERSION_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/conversion.script.hh>
#include <bugengine/meta/typeinfo.hh>
#include <bugengine/meta/typeinfo.script.hh>

namespace BugEngine { namespace Meta {

template < typename T >
struct ConversionCalculator
{
    static inline ConversionCost calculate(const Type& targetType);
};

template < typename T >
struct ConversionCalculator< _Kernel::array< T, 2 > >
{
    static inline ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< u8 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< i8 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< u16 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< i16 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< u32 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< i32 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< u64 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< i64 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< bool >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< float >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(META) ConversionCalculator< double >
{
    static ConversionCost calculate(const Type& targetType);
};

template < typename T >
ConversionCost ConversionCalculator< T >::calculate(const Type& targetType)
{
    return be_type< T >().calculateConversion(targetType);
}

template < typename T >
ConversionCost ConversionCalculator< _Kernel::array< T, 2 > >::calculate(const Type& targetType)
{
    if(targetType.metaclass->type() == ClassType_Vector2)
    {
        return ConversionCalculator< T >::calculate(
            getTypeFromIndex(ClassIndex_Numeric(targetType.metaclass->index())));
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
