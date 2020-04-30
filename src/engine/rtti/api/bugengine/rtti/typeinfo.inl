/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_INL_
#define BE_RTTI_TYPEINFO_INL_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/typeinfo.hh>
#include <bugengine/rtti/conversion.inl>

namespace BugEngine { namespace RTTI {

template < typename T >
bool Type::isA() const
{
    return isA(be_type< T >());
}

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
