/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>

#include <lua.script.hh>

namespace BugEngine {

LuaScript::LuaScript(weak< const File > file) : Script(file)
{
}

LuaScript::~LuaScript()
{
}

}  // namespace BugEngine
