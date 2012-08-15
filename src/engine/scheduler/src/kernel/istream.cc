/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/istream.hh>

namespace BugEngine { namespace Kernel
{

const IStream::MemoryState IStream::MemoryState::null = { weak<const IMemoryProvider>(), weak<IMemoryBank>(), IStream::Freed };

const IStream::MemoryState& IStream::getBank(weak<const IMemoryProvider> provider) const
{
    for (u32 i = 0; i < s_memoryStatesCount; ++i)
    {
        if (m_states[i].provider == provider)
        {
            return m_states[i];
        }
    }

    return MemoryState::null;
}


}}
