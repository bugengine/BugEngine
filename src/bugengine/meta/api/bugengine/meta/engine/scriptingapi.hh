/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_SCRIPTINGAPI_HH_
#define BE_META_ENGINE_SCRIPTINGAPI_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta {

struct ScriptingArrayAPI
{
    Type value_type;
    u32 (*size)(const Value& owner);
    Value (*index)(Value& owner, u32 index);
    Value (*indexConst)(const Value& owner, u32 index);
};

struct ScriptingAPI
{
    raw< const ScriptingArrayAPI > arrayScripting;
};

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
