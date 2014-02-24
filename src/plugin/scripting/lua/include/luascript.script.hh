/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_LUA_LUASCRIPT_SCRIPT_HH_
#define BE_LUA_LUASCRIPT_SCRIPT_HH_
/**************************************************************************************************/
#include    <bugengine/script.script.hh>
#include    <rtti/tags/editor.script.hh>

namespace BugEngine
{

class LuaScript : public Script
{
    friend class PackageLoader;
published:
    LuaScript(be_tag(EditHint::Extension(".lua")) weak<const File> script);
    ~LuaScript();
};

}

/**************************************************************************************************/
#endif
