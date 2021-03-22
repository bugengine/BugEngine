/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_HELPER_GET_HH_
#define BE_META_ENGINE_HELPER_GET_HH_
/**************************************************************************************************/
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta {

class Value;

template < typename T, typename Owner, T(Owner::*Member) >
struct PropertyHelper
{
    static Value get(void* from, bool isConst)
    {
        if(isConst)
        {
            const Owner* owner = reinterpret_cast< const Owner* >(from);
            return Value::ByRef(owner->*Member);
        }
        else
        {
            Owner* owner = reinterpret_cast< Owner* >(from);
            return Value::ByRef(owner->*Member);
        }
    }
};

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
