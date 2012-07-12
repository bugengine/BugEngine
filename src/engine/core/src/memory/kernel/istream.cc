/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/kernel/istream.hh>

namespace BugEngine { namespace Kernel
{

/*void IStream::setCurrent(weak<IMemoryBank> bank)
{
    weak<const IMemoryProvider> provider = bank->provider();
    for (u32 i = 0; i < s_memoryStatesCount; ++i)
    {
        if (m_states[i].provider == provider)
        {
            minitl::swap(m_states[i].provider, m_states[0].provider);
            minitl::swap(m_states[i].bank, m_states[0].bank);
            return;
        }
        else if(m_states[i].provider == 0)
        {
            m_states[i].provider = provider;
            m_states[i].bank = bank;
            return;
        }
    }
    be_notreached();
}*/

weak<IMemoryBank> IStream::getBank(weak<const IMemoryProvider> provider)
{
    for (u32 i = 0; i < s_memoryStatesCount; ++i)
    {
        if (m_states[i].provider == provider)
        {
            return m_states[i].bank;
        }
    }

    return weak<IMemoryBank>();
}


}}
