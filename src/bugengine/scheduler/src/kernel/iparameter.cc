/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/imemorybuffer.hh>
#include <bugengine/scheduler/kernel/imemoryhost.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace KernelScheduler {

IParameter::IParameter()
{
}

IParameter::~IParameter()
{
}

weak< const IMemoryBuffer > IParameter::getCurrentBank() const
{
    return m_buffers[0];
}

weak< const IMemoryBuffer > IParameter::getBank(weak< const IMemoryHost > host) const
{
    for(u32 i = 0; i < BufferCount; ++i)
    {
        if(m_buffers[i] && m_buffers[i]->getHost() == host) return m_buffers[i];
    }
    return weak< const IMemoryBuffer >();
}

}}  // namespace BugEngine::KernelScheduler
