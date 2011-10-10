/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packagebuilder.hh>
#include    <core/threads/thread.hh>

int be_package_parse(void* param);
const BugEngine::Allocator::Block<u8>* g_buffer = 0;
int g_bufferPosition = 0;

namespace BugEngine { namespace PackageBuilder
{

Allocator& packageArena()
{
    return gameArena();
}

PackageBuilder::PackageBuilder()
    :   IScriptEngine(packageArena())
{
}

PackageBuilder::~PackageBuilder()
{
}

void PackageBuilder::loadPackage(weak<const Package> package)
{
    loadFile(package->m_packageFile);
}

void PackageBuilder::unloadPackage(const ResourceHandle& /*handle*/)
{
}

void PackageBuilder::addNamespace(istring name, const RTTI::ClassInfo* classinfo)
{
}

void PackageBuilder::runBuffer(const Allocator::Block<u8>& buffer)
{
    g_buffer = &buffer;
    g_bufferPosition = 0;
    be_package_parse(0);
    g_buffer = 0;
    g_bufferPosition = 0;
}

}}
