/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>

#include    <lua.script.hh>

namespace BugEngine
{

LuaScript::LuaScript(weak<const File> file)
    :   Script(file)
{
}

LuaScript::~LuaScript()
{
}

}
