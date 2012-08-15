/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <packageloader.hh>
#include    <plugin/plugin.hh>

BE_PLUGIN_REGISTER(BugEngine::IResourceLoader, BugEngine::PackageManager::PackageLoader);
BE_REGISTER_NAMESPACE_2(BugEngine, PackageManager);
