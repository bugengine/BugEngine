/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <luascript.script.hh>

namespace BugEngine { namespace Lua
{

LuaScript::LuaScript(ref<const File> file)
    :   Script(file)
{
}

LuaScript::~LuaScript()
{
}

}}
