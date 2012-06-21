/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/kernel/ikernelscheduler.hh>

namespace BugEngine
{

IKernelScheduler::IKernelScheduler(const istring& name)
    :   m_name(name)
{
}

IKernelScheduler::~IKernelScheduler()
{
}

}
