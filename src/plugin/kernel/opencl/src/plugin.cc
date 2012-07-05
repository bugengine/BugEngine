/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <opencl/clkernelscheduler.hh>
#include    <system/plugin.hh>
#include    <rtti/classinfo.script.hh>

BE_PLUGIN_REGISTER(BugEngine::Kernel::IKernelScheduler, BugEngine::OpenCLKernelScheduler);
