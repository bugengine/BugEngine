/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packageloader.hh>
#include    <core/threads/thread.hh>

namespace BugEngine { namespace PackageManager
{

Allocator& packageArena()
{
    return gameArena();
}

PackageLoader::PackageLoader()
    :   IScriptEngine(packageArena())
{
}

PackageLoader::~PackageLoader()
{
}

void PackageLoader::loadPackage(weak<const Package> package)
{
    loadFile(package->m_packageFile);
}

void PackageLoader::unloadPackage(const ResourceHandle& /*handle*/)
{
}

void PackageLoader::addNamespace(istring name, const RTTI::ClassInfo* classinfo)
{
}

void PackageLoader::doBuffer(const Allocator::Block<u8>& buffer)
{
    struct PackageHeader
    {
        int version;
    };
}

}}
