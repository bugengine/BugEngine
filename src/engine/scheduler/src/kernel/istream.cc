/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/istream.hh>
#include    <scheduler/kernel/imemoryprovider.hh>

namespace BugEngine { namespace Kernel
{

const IStream::MemoryState IStream::MemoryState::null = { weak<const IMemoryProvider>(), weak<IMemoryBank>(), IStream::Free };

weak<const IMemoryBank> IStream::getCurrentBank() const
{
    if (m_states[0].state == Current)
    {
        return m_states[0].bank;
    }
    else
    {
        return weak<const IMemoryBank>();
    }
}

void IStream::setBank(weak<IMemoryBank> bank)
{
    be_assert(bank, "a memory bank is required");
    weak<const IMemoryProvider> provider = bank->provider();
    MemoryState* freeState = 0;
    MemoryState* bankState = 0;
    for (u32 i = 0; i < s_memoryStatesCount; ++i)
    {
        if (m_states[i].provider == provider)
        {
            be_assert(bankState == 0, "provider \"%s\" has two banks" | provider->name());
            bankState = &m_states[i];
        }
        else if (!m_states[i].provider)
        {
            freeState = &m_states[i];
        }
        else
        {
            m_states[i].state = Allocated;
        }
    }
    if (bankState)
    {
        bankState->bank = bank;
        bankState->state = Current;
    }
    else
    {
        be_assert_recover(freeState != 0, "could not reserve a bank for provider \"%s\"" | provider->name(), return);
        freeState->bank = bank;
        freeState->provider = provider;
        freeState->state = Current;
        bankState = freeState;
    }
    minitl::swap(*bankState, m_states[0]);
}

}}
