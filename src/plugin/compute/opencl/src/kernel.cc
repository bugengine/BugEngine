/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <kernelobject.hh>
#include    <opencl/scheduler.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL
{

KernelObject::KernelObject(weak<const Scheduler> scheduler, const inamespace& name)
    :   m_kernel(name, ipath("kernel"))
    ,   m_kernelBlob(*m_kernel.getSymbol<const unsigned char*>(istring(minitl::format<128u>("s_clKernel%d") | scheduler->m_ptrSize)))
    ,   m_program(scheduler->buildKernel(*m_kernel.getSymbol<const unsigned long>(istring(minitl::format<128u>("s_clKernel%dSize") | scheduler->m_ptrSize)), m_kernelBlob))
{
}

KernelObject::~KernelObject()
{
    clReleaseKernel(m_program.first);
    clReleaseProgram(m_program.second);
}

void KernelObject::run(const minitl::array< weak<const IMemoryBuffer> >& params)
{
    be_forceuse(params);
}

}}}
