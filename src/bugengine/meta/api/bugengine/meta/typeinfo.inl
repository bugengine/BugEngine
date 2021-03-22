/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_TYPEINFO_INL_
#define BE_META_TYPEINFO_INL_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/conversion.hh>
#include <bugengine/meta/typeinfo.hh>

namespace BugEngine { namespace Meta {

template < typename T >
bool Type::isA() const
{
    return isA(be_type< T >());
}

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
