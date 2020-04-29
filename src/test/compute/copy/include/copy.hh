/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#ifndef BE_TEST_COMPUTE_COPY_COPY_HH_
#define BE_TEST_COMPUTE_COPY_COPY_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <bugengine/application.hh>
#include    <plugin/plugin.hh>
#include    <package/package.script.hh>

namespace BugEngine { namespace Test { namespace Compute { namespace Copy
{

class CopyApplication : public Application
{
private:
    Plugin::Plugin<Resource::ILoader>   const   m_packageManager;
    Plugin::Plugin<Resource::ILoader>   const   m_computeCudaModule;
    Plugin::Plugin<Resource::ILoader>   const   m_computeCLModule;
    ref<const Package>                  const   m_mainPackage;
public:
    CopyApplication(const Plugin::Context& context);
    ~CopyApplication();
};

}}}}

/**************************************************************************************************/
#endif
