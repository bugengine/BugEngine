/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_ISTREAM_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_ISTREAM_HH_
/*****************************************************************************/
#include    <system/scheduler/kernel/imemorybank.hh>

namespace BugEngine { namespace Kernel
{

class IMemoryProvider;
class IMemoryBank;

class be_api(SYSTEM) IStream : public minitl::refcountable
{
private:
    enum BankState
    {
        Freed,
        Reserved,
        Updated
    };
    struct MemoryState
    {
        weak<const IMemoryProvider> provider;
        weak<IMemoryBank>           bank;
        BankState                   state;

        static const MemoryState null;
        operator const void*() const    { return (const void*)(this - &null); }
        bool operator!() const          { return this == &null; }
    };
    static const u32 s_memoryStatesCount = 8;
private:
    MemoryState m_states[s_memoryStatesCount];
public:
    const MemoryState& getBank(weak<const IMemoryProvider> provider) const;
};

}}

/*****************************************************************************/
#endif
