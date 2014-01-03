/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <packagebuilder.hh>
#include    <buildcontext.hh>
#include    <package/package.script.hh>

int be_package_parse(void* param);

namespace BugEngine
{

namespace Arena
{
minitl::Allocator& packageBuilder()
{
    return resource();
}
}

namespace PackageBuilder
{

PackageBuilder::PackageBuilder(ref<Folder> dataFolder)
    :   m_dataFolder(dataFolder)
{
}

PackageBuilder::~PackageBuilder()
{
}

ref<Nodes::Package> PackageBuilder::createPackage(const minitl::Allocator::Block<u8>& buffer)
{
    BuildContext context(buffer, m_dataFolder);
    be_package_parse(&context);
    return context.result;
}

}}
