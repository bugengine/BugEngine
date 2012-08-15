/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <opencl/clkernelscheduler.hh>
#include    <plugin/plugin.hh>
#include    <rtti/classinfo.script.hh>

BE_PLUGIN_REGISTER(BugEngine::Kernel::IKernelScheduler, BugEngine::OpenCLKernelScheduler);
