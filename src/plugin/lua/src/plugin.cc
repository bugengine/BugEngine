/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <context.h>
#include    <system/plugin.hh>

BE_PLUGIN_REGISTER(lua, BugEngine::Lua::Context, (weak<const BugEngine::FileSystem> filesystem, BugEngine::Value value), (filesystem, value));
