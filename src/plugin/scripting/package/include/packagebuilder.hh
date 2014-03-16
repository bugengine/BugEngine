/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGEBUILDER_HH_
#define BE_PACKAGE_PACKAGEBUILDER_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>
#include    <package/nodes/package.hh>

namespace BugEngine
{
namespace PackageBuilder
{

class PackageBuilder : public minitl::pointer
{
    BE_NOCOPY(PackageBuilder);
private:
    ref<Folder> m_dataFolder;
public:
    PackageBuilder(ref<Folder> dataFolder);
    ~PackageBuilder();
public:
    ref<Nodes::Package> createPackage(const ifilename& filename,
                                      const minitl::Allocator::Block<u8>& buffer);
};

}
}

/**************************************************************************************************/
#endif
