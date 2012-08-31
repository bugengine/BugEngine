/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGELOADER_HH_
#define BE_PACKAGE_PACKAGELOADER_HH_
/*****************************************************************************/
#include    <package/package.script.hh>
#include    <bugengine/scriptengine.hh>
#include    <plugin/plugin.hh>

namespace BugEngine { namespace PackageBuilder
{
class PackageBuilder;
}}

namespace BugEngine { namespace PackageManager
{

class PackageLoader : public ScriptEngine<Package>
{
    BE_NOCOPY(PackageLoader);
private:
    scoped<PackageBuilder::PackageBuilder>   m_packageBuilder;
public:
    PackageLoader(const PluginContext& context);
    ~PackageLoader();
private:
    virtual void runBuffer(weak<const Package> script, Resource::Resource& resource, const minitl::Allocator::Block<u8>& buffer) override;
    virtual void unloadScript(Resource::Resource& handle) override;
};

}}

/*****************************************************************************/
#endif
