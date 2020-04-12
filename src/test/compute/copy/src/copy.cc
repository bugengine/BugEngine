/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#include <stdafx.h>
#include <copy.hh>
#include <filesystem/diskfolder.script.hh>
#include <plugin/plugin.hh>
#include <rtti/engine/namespace.hh>

namespace BugEngine { namespace Test { namespace Compute { namespace Copy
{

CopyApplication::CopyApplication(const Plugin::Context& context)
    :   Application(ref<DiskFolder>::create(Arena::game(),
                                            Environment::getEnvironment().getDataDirectory() + ipath("test/compute/copy")),
                    context.resourceManager,
                    context.scheduler)
    ,   m_packageManager("plugin.scripting.package", pluginContext())
    ,   m_computeModule("plugin.compute.cuda", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(ifilename("copy.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

CopyApplication::~CopyApplication()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}}}}

BE_PLUGIN_REGISTER(BugEngine::Test::Compute::Copy::CopyApplication)
