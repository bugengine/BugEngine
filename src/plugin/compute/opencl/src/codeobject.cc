/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <codeobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

CodeObject::CodeObject(weak< const Context > context, const inamespace& name)
    : m_kernel(name, ipath("kernel"))
{
    const char* kernelSource = *m_kernel.getSymbol< const char* >(
        istring(minitl::format< 128u >("s_clKernel%d") | context->getPointerSize()));
    const u64 kernelSourceSize = *m_kernel.getSymbol< const u64 >(
        istring(minitl::format< 128u >("s_clKernel%dSize") | context->getPointerSize()));
    m_program = context->buildProgram(kernelSourceSize, kernelSource);
}

CodeObject::~CodeObject()
{
    clReleaseProgram(m_program);
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
