/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <system/plugin.hh>
#include    <cpukernelscheduler.hh>
#include    <rtti/classinfo.script.hh>

BE_PLUGIN_REGISTER(BugEngine::Kernel::IKernelScheduler, BugEngine::CpuKernelScheduler);
