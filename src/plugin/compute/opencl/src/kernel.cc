/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin.compute.opencl/scheduler.hh>
#include <context.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

KernelObject::KernelObject(weak< const Context > context, const inamespace& name)
    : m_kernel(name, ipath("kernel"))
    , m_kernelBlob(*m_kernel.getSymbol< const char* >(
         istring(minitl::format< 128u >("s_clKernel%d") | context->getPointerSize())))
    , m_program(context->buildProgram(
         *m_kernel.getSymbol< const u64 >(
            istring(minitl::format< 128u >("s_clKernel%dSize") | context->getPointerSize())),
         m_kernelBlob))
{
}

KernelObject::~KernelObject()
{
    clReleaseProgram(m_program);
}

void KernelObject::run(const minitl::array< weak< const IMemoryBuffer > >& params)
{
    be_forceuse(params);
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
