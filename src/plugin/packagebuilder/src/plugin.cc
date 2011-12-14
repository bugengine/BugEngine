/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <packagebuilder/stdafx.h>
#include    <packageloader.hh>
#include    <system/plugin.hh>

BE_PLUGIN_REGISTER(packagebuilder, BugEngine::IResourceLoader, BugEngine::PackageBuilder::PackageLoader);
BE_REGISTER_NAMESPACE_2(packagebuilder, BugEngine, PackageBuilder);
