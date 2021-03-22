/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_HELPER_SET_HH_
#define BE_META_ENGINE_HELPER_SET_HH_
/**************************************************************************************************/
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta {

template < typename T, typename Owner, T(Owner::*Field) >
static inline void set(weak< const Property > /*_this*/, void* from, Value& value, bool isConst)
{
    be_assert_recover(!isConst, "Setting property on const object", return );
    be_forceuse(isConst);
    (Owner*)from->*Field = value.as< T >();
}

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
