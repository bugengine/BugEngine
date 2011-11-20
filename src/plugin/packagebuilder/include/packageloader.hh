/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_PACKAGELOADER_HH_
#define BE_PACKAGEBUILDER_PACKAGELOADER_HH_
/*****************************************************************************/
#include    <package.script.hh>
#include    <main/scripting.hh>

namespace BugEngine { namespace PackageBuilder
{

class PackageLoader : public IScriptEngine
{
    BE_NOCOPY(PackageLoader);
public:
    PackageLoader();
    ~PackageLoader();

    void loadPackage(weak<const Package> package);
    void unloadPackage(const ResourceHandle& handle);
private:
    virtual void addNamespace(istring name, raw<const RTTI::ClassInfo> classinfo) override;
    virtual void runBuffer(const Allocator::Block<u8>& buffer) override;
};

}}

/*****************************************************************************/
#endif
