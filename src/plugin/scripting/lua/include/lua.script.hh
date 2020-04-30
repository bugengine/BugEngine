/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_LUA_LUA_SCRIPT_HH_
#define BE_LUA_LUA_SCRIPT_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/rtti/tags/editor.script.hh>
#include <bugengine/script.script.hh>

namespace BugEngine {

class LuaScript : public Script
{
    friend class PackageLoader;
    published : LuaScript(be_tag(EditHint::Extension(".lua")) weak< const File > script);
    ~LuaScript();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
