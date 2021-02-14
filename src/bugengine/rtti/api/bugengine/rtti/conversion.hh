/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CONVERSION_HH_
#define BE_RTTI_CONVERSION_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/conversion.script.hh>
#include <bugengine/rtti/typeinfo.hh>
#include <bugengine/rtti/typeinfo.script.hh>

namespace BugEngine { namespace RTTI {

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
struct be_api(RTTI) ConversionCalculator< u8 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< i8 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< u16 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< i16 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< u32 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< i32 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< u64 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< i64 >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< bool >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< float >
{
    static ConversionCost calculate(const Type& targetType);
};

template <>
struct be_api(RTTI) ConversionCalculator< double >
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

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
