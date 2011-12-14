/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGELOADER_HH_
#define BE_PACKAGE_PACKAGELOADER_HH_
/*****************************************************************************/
#include    <package.script.hh>
#include    <bugengine/scriptengine.hh>
#include    <system/plugin.hh>

namespace BugEngine { namespace PackageManager
{

class PackageLoader : public ScriptEngine<Package>
{
    BE_NOCOPY(PackageLoader);
public:
    PackageLoader(const PluginContext& context);
    ~PackageLoader();
private:
    virtual void runBuffer(weak<const Package> script, const Allocator::Block<u8>& buffer) override;
};

}}

/*****************************************************************************/
#endif
