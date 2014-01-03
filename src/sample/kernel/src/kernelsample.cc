
/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelsample.hh>
#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine
{

KernelSample::KernelSample(const Plugin::Context& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
    ,   m_packageManager("plugin.scripting.package", pluginContext())
    ,   m_lua("plugin.scripting.lua", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(istring("sample-kernel.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

KernelSample::~KernelSample()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}
