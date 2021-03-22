/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/core/md5.hh>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin.scripting.package/nodes/package.hh>
#include <packagebuilder.hh>
#include <packageloader.hh>

namespace BugEngine { namespace Arena {

minitl::Allocator& package()
{
    return resource();
}

}}  // namespace BugEngine::Arena

namespace BugEngine { namespace PackageManager {

PackageLoader::PackageLoader(const Plugin::Context& context)
    : ScriptEngine< Package >(Arena::package(), context.resourceManager)
    , m_packageBuilder(
          scoped< PackageBuilder::PackageBuilder >::create(Arena::package(), context.dataFolder))
{
}

PackageLoader::~PackageLoader()
{
}

void PackageLoader::unload(Resource::Resource& handle)
{
    {
        weak< PackageBuilder::Nodes::Package > package
            = handle.getRefHandle< PackageBuilder::Nodes::Package >();
        if(package)
        {
            package->deleteObjects(m_manager);
        }
    }
    handle.clearRefHandle();
}

void PackageLoader::runBuffer(weak< const Package > script, Resource::Resource& resource,
                              const minitl::Allocator::Block< u8 >& buffer)
{
    MD5 md5 = digest(buffer);
    be_info("md5 sum of package: %s" | md5);
    ref< PackageBuilder::Nodes::Package > package
        = m_packageBuilder->createPackage(script->getScriptName(), buffer);
    for(Meta::AST::MessageList::const_iterator it = package->context().messages.begin();
        it != package->context().messages.end(); ++it)
    {
        Logger::root()->log(it->severity, __FILE__, __LINE__, it->message.c_str());
    }
    if(package->success())
    {
        resource.setRefHandle(package);
        package->createObjects(m_manager);
    }
}

void PackageLoader::reloadBuffer(weak< const Package > script, Resource::Resource& resource,
                                 const minitl::Allocator::Block< u8 >& buffer)
{
    MD5 md5 = digest(buffer);
    be_info("md5 sum of package: %s" | md5);
    ref< PackageBuilder::Nodes::Package > newPackage
        = m_packageBuilder->createPackage(script->getScriptName(), buffer);
    weak< PackageBuilder::Nodes::Package > oldPackage
        = resource.getRefHandle< PackageBuilder::Nodes::Package >();
    newPackage->diffFromPackage(oldPackage, m_manager);
    oldPackage = weak< PackageBuilder::Nodes::Package >();
    resource.clearRefHandle();
    resource.setRefHandle(newPackage);
}

}}  // namespace BugEngine::PackageManager
