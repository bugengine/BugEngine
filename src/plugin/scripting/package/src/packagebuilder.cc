/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <packagebuilder.hh>
#include    <buildcontext.hh>
#include    <package/package.script.hh>

int be_package_parse(void* param);

namespace BugEngine { namespace PackageBuilder
{

Allocator& packageBuilderArena()
{
    return gameArena();
}

PackageBuilder::PackageBuilder()
{
}

PackageBuilder::~PackageBuilder()
{
}

ref<Nodes::Package> PackageBuilder::createPackage(const Allocator::Block<u8>& buffer)
{
    BuildContext context(buffer);
    be_package_parse(&context);
    return context.result;
}


}}
