/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_SCRIPTINGAPI_HH_
#define BE_RTTI_ENGINE_SCRIPTINGAPI_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI {

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

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
