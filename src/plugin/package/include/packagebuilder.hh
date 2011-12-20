/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGEBUILDER_HH_
#define BE_PACKAGE_PACKAGEBUILDER_HH_
/*****************************************************************************/
#include    <package/nodes/package.hh>

namespace BugEngine { namespace PackageBuilder
{

class PackageBuilder : public minitl::pointer
{
    BE_NOCOPY(PackageBuilder);
public:
    PackageBuilder();
    ~PackageBuilder();
private:
    virtual ref<Nodes::Package> createPackage(const Allocator::Block<u8>& buffer) override;
};

}}

/*****************************************************************************/
#endif
