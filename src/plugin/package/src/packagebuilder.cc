/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <packagebuilder.hh>
#include    <buildcontext.hh>
#include    <package/package.script.hh>

int be_package_parse(void* param);
int be_package_lex_destroy();
const BugEngine::Allocator::Block<u8>* g_buffer = 0;
int g_bufferPosition = 0;
int g_packageLine = 0;
int g_packageColumnBefore = 0;
int g_packageColumnAfter = 0;

namespace BugEngine
{

Allocator& packageBuilderArena()
{
    return gameArena();
}

}

namespace BugEngine { namespace PackageBuilder
{

PackageBuilder::PackageBuilder()
{
}

PackageBuilder::~PackageBuilder()
{
}

ref<Nodes::Package> PackageBuilder::createPackage(const Allocator::Block<u8>& buffer)
{
    g_buffer = &buffer;
    g_bufferPosition = 0;
    g_packageLine = 0;
    g_packageColumnBefore = g_packageColumnAfter = 0;
    BuildContext context;
    be_package_parse(&context);
    be_package_lex_destroy();
    g_buffer = 0;
    g_bufferPosition = 0;
    return context.result;
}


}}
