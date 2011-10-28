/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packageloader.hh>
#include    <system/plugin.hh>

BE_PLUGIN_REGISTER(package, BugEngine::PackageManager::PackageLoader, (), ());
BE_REGISTER_NAMESPACE_2(BugEngine, PackageManager);
