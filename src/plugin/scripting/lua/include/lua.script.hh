/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_LUA_LUA_SCRIPT_HH_
#define BE_LUA_LUA_SCRIPT_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/meta/tags/editor.script.hh>
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
