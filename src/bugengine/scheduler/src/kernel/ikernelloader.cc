/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/ikernelloader.hh>

#include <bugengine/scheduler/kernel/icodeloader.hh>

namespace BugEngine { namespace KernelScheduler {

IKernelLoader::IKernelLoader(ref< ICodeLoader > codeLoader) : m_codeLoader(codeLoader)
{
}

IKernelLoader::~IKernelLoader()
{
}

}}  // namespace BugEngine::KernelScheduler
