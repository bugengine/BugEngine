/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <packageloader.hh>
#include    <packagebuilder.hh>
#include    <package/nodes/package.hh>
#include    <core/md5.hh>
#include    <filesystem/folder.script.hh>


namespace BugEngine { namespace Arena
{

minitl::Allocator& package()
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

void PackageLoader::runBuffer(weak<const Package> /*script*/, Resource::Resource& resource, const minitl::Allocator::Block<u8>& buffer)
{
    MD5 md5 = digest(buffer);
    be_info("md5 sum of package: %s" | md5);
    ref<PackageBuilder::Nodes::Package> package = m_packageBuilder->createPackage(buffer);
    resource.setRefHandle(package);
    package->createObjects(m_manager);
}

void PackageLoader::unloadScript(Resource::Resource& resource)
{
    resource.getRefHandle<PackageBuilder::Nodes::Package>()->deleteObjects(m_manager);
    resource.clearRefHandle();
}

}}
