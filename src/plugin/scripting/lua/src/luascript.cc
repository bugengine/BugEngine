/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <luascript.script.hh>

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
