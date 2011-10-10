/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_PACKAGEBUILDER_HH_
#define BE_PACKAGEBUILDER_PACKAGEBUILDER_HH_
/*****************************************************************************/
#include    <package.script.hh>
#include    <main/scripting.hh>

namespace BugEngine { namespace PackageBuilder
{

class PackageBuilder : public IScriptEngine
{
    BE_NOCOPY(PackageBuilder);
public:
    PackageBuilder();
    ~PackageBuilder();

    void loadPackage(weak<const Package> package);
    void unloadPackage(const ResourceHandle& handle);
private:
    virtual void addNamespace(istring name, const RTTI::ClassInfo* classinfo) override;
    virtual void runBuffer(const Allocator::Block<u8>& buffer) override;
};

}}

/*****************************************************************************/
#endif
