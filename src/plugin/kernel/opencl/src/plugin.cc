/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <opencl/clkernelscheduler.hh>
#include    <plugin/plugin.hh>
#include    <rtti/classinfo.script.hh>

static ref<BugEngine::OpenCLKernelScheduler> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::OpenCLKernelScheduler>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::Kernel::IKernelScheduler, &create);
