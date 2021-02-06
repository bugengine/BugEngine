/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <scheduler.hh>

BE_PLUGIN_REGISTER(BugEngine::KernelScheduler::Cuda::Scheduler);
