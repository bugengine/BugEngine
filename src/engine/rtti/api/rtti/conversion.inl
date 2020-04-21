/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_CONVERSION_SCRIPT_HH_
#define BE_RTTI_CONVERSION_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>

namespace BugEngine { namespace RTTI
{

template< typename T >
ConversionCost calculateConversion(const Type& targetType)
{
    return be_type<T>().calculateConversion(targetType);
}

}}

/**************************************************************************************************/
#endif
