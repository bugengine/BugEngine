/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelsample.hh>
#include    <kerneltask.script.hh>
#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine
{

KernelSample::KernelSample(const Plugin::Context& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
    ,   m_packageManager("scripting.package", pluginContext())
    ,   m_lua("scripting.lua", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(istring("sample-kernel.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

KernelSample::~KernelSample()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}
