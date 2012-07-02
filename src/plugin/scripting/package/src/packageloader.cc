/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <packageloader.hh>
#include    <packagebuilder.hh>
#include    <package/nodes/package.hh>
#include    <core/runtime/md5.hh>
#include    <system/file/folder.script.hh>


namespace BugEngine { namespace Arena
{

Allocator& package()
{
    return resource();
}

}}

namespace BugEngine { namespace PackageManager
{

PackageLoader::PackageLoader(const PluginContext& context)
    :   ScriptEngine<Package>(Arena::package(), context.resourceManager)
    ,   m_packageBuilder(scoped<PackageBuilder::PackageBuilder>::create(Arena::package(), context.dataFolder))
{
}

PackageLoader::~PackageLoader()
{
}

void PackageLoader::runBuffer(weak<const Package> script, const Allocator::Block<u8>& buffer)
{
    MD5 md5 = digest(buffer);
    be_info("md5 sum of package: %s" | md5);
    ref<PackageBuilder::Nodes::Package> package = m_packageBuilder->createPackage(buffer);
    script->getResourceHandleForWriting(this).handle = package;
    package->createObjects(m_manager);
}

void PackageLoader::unloadScript(const ResourceHandle& handle)
{
    ref<PackageBuilder::Nodes::Package> package = be_checked_cast<PackageBuilder::Nodes::Package>(handle.handle);
    package->deleteObjects(m_manager);
}

}}
