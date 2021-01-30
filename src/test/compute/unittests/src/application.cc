/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/filesystem/diskfolder.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti/engine/namespace.hh>
#include <application.hh>

namespace BugEngine { namespace Test { namespace Compute { namespace UnitTests {

UnitTestsApplication::UnitTestsApplication(const Plugin::Context& context)
    : Application(
        ref< DiskFolder >::create(Arena::game(), Environment::getEnvironment().getDataDirectory()
                                                     + ipath("test/compute/unittests")),
        context.resourceManager, context.scheduler)
    , m_packageManager("plugin.scripting.package", pluginContext())
    , m_computeCudaModule("plugin.compute.cuda", pluginContext())
    , m_computeCLModule("plugin.compute.opencl", pluginContext())
    , m_mainPackage(ref< Package >::create(
          Arena::game(), pluginContext().dataFolder->openFile(ifilename("unittests.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

UnitTestsApplication::~UnitTestsApplication()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}}}}  // namespace BugEngine::Test::Compute::UnitTests

BE_PLUGIN_REGISTER(BugEngine::Test::Compute::UnitTests::UnitTestsApplication)
