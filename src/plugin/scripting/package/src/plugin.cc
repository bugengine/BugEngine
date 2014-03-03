/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <packageloader.hh>
#include    <plugin/plugin.hh>
#include    <rtti/engine/namespace.hh>

static ref<BugEngine::PackageManager::PackageLoader> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::PackageManager::PackageLoader>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::Resource::ILoader, &create);
BE_REGISTER_NAMESPACE_2(BugEngine, PackageManager);
