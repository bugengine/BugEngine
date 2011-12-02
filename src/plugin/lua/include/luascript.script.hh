/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_LUA_SCRIPT_SCRIPT_HH_
#define BE_LUA_SCRIPT_SCRIPT_HH_
/*****************************************************************************/
#include    <main/script.script.hh>
#include    <rtti/tags/editor.script.hh>

namespace BugEngine { namespace Lua
{

be_tag(ResourceLoaders())
class LuaScript : public Script
{
    friend class PackageLoader;
published:
    LuaScript(be_tag(EditHint::Extension(".lua")) ref<const File> file);
    ~LuaScript();
};

}}

/*****************************************************************************/
#endif
