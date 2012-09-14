/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <context.h>
#include    <plugin/plugin.hh>
#include    <rtti/engine/namespace.hh>

BE_PLUGIN_REGISTER(BugEngine::Resource::ILoader, BugEngine::Lua::Context);
BE_REGISTER_NAMESPACE_2(BugEngine, Lua);
