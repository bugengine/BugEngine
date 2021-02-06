/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <codeobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

CodeObject::CodeObject(const inamespace& name) : m_kernel(name, ipath("kernel"))
{
}

CodeObject::~CodeObject()
{
}

}}}  // namespace BugEngine::KernelScheduler::CPU
