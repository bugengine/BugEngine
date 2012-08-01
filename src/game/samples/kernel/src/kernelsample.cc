/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelsample.hh>
#include    <core/environment.hh>
#include    <system/file/diskfolder.script.hh>

namespace BugEngine { namespace Samples
{

KernelSample::KernelSample(const PluginContext& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
    ,   m_kernelSample(scoped<Kernel::Kernel>::create(Arena::general(), "samples.kernel.add"))
{
    pluginContext().resourceManager->load(weak<Kernel::Kernel>(m_kernelSample));
}

KernelSample::~KernelSample()
{
    pluginContext().resourceManager->unload(weak<Kernel::Kernel>(m_kernelSample));
}

}}

