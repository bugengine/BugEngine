/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <codeobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

CodeObject::CodeObject(const inamespace& name) : m_kernel(name, ipath("bugengine/kernel"))
{
}

CodeObject::~CodeObject()
{
}

}}}  // namespace BugEngine::KernelScheduler::CPU
