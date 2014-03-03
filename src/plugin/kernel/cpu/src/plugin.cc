/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <plugin/plugin.hh>
#include    <cpukernelscheduler.hh>
#include    <rtti/classinfo.script.hh>

static ref<BugEngine::CPUKernelScheduler> create(const BugEngine::Plugin::Context& context)
{
    return ref<BugEngine::CPUKernelScheduler>::create(BugEngine::Arena::game(), context);
}

BE_PLUGIN_REGISTER(BugEngine::Kernel::IKernelScheduler, &create);
