/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#ifndef BE_TEST_COMPUTE_UNITTESTS_APPLICATION_HH_
#define BE_TEST_COMPUTE_UNITTESTS_APPLICATION_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/application.hh>
#include <bugengine/plugin.scripting.package/package.script.hh>
#include <bugengine/plugin/plugin.hh>

namespace BugEngine { namespace Test { namespace Compute { namespace UnitTests {

class UnitTestsApplication : public Application
{
private:
    Plugin::Plugin< Resource::ILoader > const m_packageManager;
    Plugin::Plugin< Resource::ILoader > const m_computeCudaModule;
    Plugin::Plugin< Resource::ILoader > const m_computeCLModule;
    ref< const Package > const                m_mainPackage;

public:
    UnitTestsApplication(const Plugin::Context& context);
    ~UnitTestsApplication();
};

}}}}  // namespace BugEngine::Test::Compute::UnitTests

/**************************************************************************************************/
#endif
