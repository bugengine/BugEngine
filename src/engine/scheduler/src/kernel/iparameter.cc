/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/parameters/iparameter.script.hh>
#include    <scheduler/kernel/imemorybuffer.hh>
#include    <scheduler/kernel/imemoryhost.hh>


namespace BugEngine { namespace Kernel
{

IParameter::IParameter()
{
}

IParameter::~IParameter()
{
}

weak<const IMemoryBuffer> IParameter::getCurrentBank() const
{
    return m_buffers[0];
}

weak<const IMemoryBuffer> IParameter::getBank(weak<const IMemoryHost> host) const
{
    for (u32 i = 0; i < BufferCount; ++i)
    {
        if (m_buffers[i] && m_buffers[i]->getHost() == host)
            return m_buffers[i];
    }
    return weak<const IMemoryBuffer>();
}

/*void transfer(weak<const IMemoryHost> to)
{

}*/

}}
