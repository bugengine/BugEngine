/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <codeobject.hh>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

KernelObject::KernelObject(weak< const CodeObject > code, const istring name)
    : m_kernel(clCreateKernel(code->m_program, (minitl::format< 128u >("%s_spir") | name), 0))
{
    be_info("OpenCL kernel entry point: %p" | m_kernel);
}

KernelObject::~KernelObject()
{
    clReleaseKernel(m_kernel);
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
