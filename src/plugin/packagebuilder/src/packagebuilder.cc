/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packagebuilder.hh>
#include    <buildcontext.hh>


int be_package_parse(void* param);
const BugEngine::Allocator::Block<u8>* g_buffer = 0;
int g_bufferPosition = 0;
int g_packageLine = 0;
int g_packageColumnBefore = 0;
int g_packageColumnAfter = 0;

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
    loadFile(package->m_packageFile, tempArena());
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
    g_packageLine = 0;
    g_packageColumnBefore = g_packageColumnAfter = 0;
    BuildContext context;
    be_package_parse(&context);
    g_buffer = 0;
    g_bufferPosition = 0;
}

}}
