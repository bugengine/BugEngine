
/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <particlerain.hh>
#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine { namespace ParticleRain
{

ParticleRain::ParticleRain(const Plugin::Context& context)
    :   Application(ref<DiskFolder>::create(Arena::game(),
                                            Environment::getEnvironment().getDataDirectory()),
                    context.resourceManager,
                    context.scheduler)
    ,   m_packageManager("plugin.scripting.package", pluginContext())
    ,   m_computeModule("plugin.compute.cuda", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(ifilename("particlerain/particlerain.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

ParticleRain::~ParticleRain()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}}
