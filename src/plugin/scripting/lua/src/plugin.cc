/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <context.h>
#include    <system/plugin.hh>

BE_PLUGIN_REGISTER(BugEngine::IResourceLoader, BugEngine::Lua::Context);
BE_REGISTER_NAMESPACE_2(BugEngine, Lua);
