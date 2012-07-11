/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGEBUILDER_HH_
#define BE_PACKAGE_PACKAGEBUILDER_HH_
/*****************************************************************************/
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
    ref<Nodes::Package> createPackage(const Allocator::Block<u8>& buffer);
};

}
}

/*****************************************************************************/
#endif
