/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/imemorybuffer.hh>

namespace BugEngine { namespace KernelScheduler {

IMemoryBuffer::IMemoryBuffer(weak< const IMemoryHost > host) : m_host(host)
{
}

IMemoryBuffer::~IMemoryBuffer()
{
}

}}  // namespace BugEngine::KernelScheduler
