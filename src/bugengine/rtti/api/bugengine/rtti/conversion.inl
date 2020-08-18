/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_CONVERSION_INL_
#define BE_RTTI_CONVERSION_INL_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>

namespace BugEngine { namespace RTTI {

template < typename T >
ConversionCost calculateConversion(const Type& targetType)
{
    return be_type< T >().calculateConversion(targetType);
}

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
