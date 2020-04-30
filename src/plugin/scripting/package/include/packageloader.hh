/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGELOADER_HH_
#define BE_PACKAGE_PACKAGELOADER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/package.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/scriptengine.hh>

namespace BugEngine { namespace PackageBuilder {
class PackageBuilder;
}}  // namespace BugEngine::PackageBuilder

namespace BugEngine { namespace PackageManager {

class PackageLoader : public ScriptEngine< Package >
{
    BE_NOCOPY(PackageLoader);

private:
    scoped< PackageBuilder::PackageBuilder > m_packageBuilder;

public:
    PackageLoader(const Plugin::Context& context);
    ~PackageLoader();

private:
    virtual void unload(Resource::Resource& handle) override;
    virtual void runBuffer(weak< const Package > script, Resource::Resource& resource,
                           const minitl::Allocator::Block< u8 >& buffer) override;
    virtual void reloadBuffer(weak< const Package > script, Resource::Resource& resource,
                              const minitl::Allocator::Block< u8 >& buffer) override;
};

}}  // namespace BugEngine::PackageManager

/**************************************************************************************************/
#endif
