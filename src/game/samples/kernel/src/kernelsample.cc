/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelsample.hh>
#include    <kerneltask.script.hh>
#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine { namespace Samples
{

KernelSample::KernelSample(const PluginContext& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
    ,   m_kernelTask(scoped<KernelSampleTask>::create(Arena::general()))
{
    pluginContext().resourceManager->load(weak<KernelSampleTask>(m_kernelTask));
}

KernelSample::~KernelSample()
{
    pluginContext().resourceManager->unload(weak<KernelSampleTask>(m_kernelTask));
}

}}

