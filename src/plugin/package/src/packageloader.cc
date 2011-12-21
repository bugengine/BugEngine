/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <packageloader.hh>
#include    <core/threads/thread.hh>
#include    <packagebuilder.hh>

namespace BugEngine
{

Allocator& packageArena()
{
    return gameArena();
}

}

namespace BugEngine { namespace PackageManager
{

Allocator& packageArena()
{
    return gameArena();
}

PackageLoader::PackageLoader(const PluginContext& context)
    :   ScriptEngine<Package>(packageArena(), context.resourceManager)
    ,   m_packageBuilder(scoped<PackageBuilder::PackageBuilder>::create(packageArena()))
{
}

PackageLoader::~PackageLoader()
{
}

void PackageLoader::runBuffer(weak<const Package> script, const Allocator::Block<u8>& buffer)
{
    script->getResourceHandleForWriting(this).handle = m_packageBuilder->createPackage(buffer);
}

}}
