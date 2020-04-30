/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/package.script.hh>
#include <buildcontext.hh>
#include <packagebuilder.hh>

int be_package_parse(void* param);

namespace BugEngine {

namespace Arena {
minitl::Allocator& packageBuilder()
{
    return resource();
}
}  // namespace Arena

namespace PackageBuilder {

PackageBuilder::PackageBuilder(ref< Folder > dataFolder) : m_dataFolder(dataFolder)
{
}

PackageBuilder::~PackageBuilder()
{
}

ref< Nodes::Package > PackageBuilder::createPackage(const ifilename&                      filename,
                                                    const minitl::Allocator::Block< u8 >& buffer)
{
    BuildContext context(filename, buffer, m_dataFolder);
    be_package_parse(&context);
    return context.result;
}

}  // namespace PackageBuilder
}  // namespace BugEngine
