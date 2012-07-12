/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_KERNEL_ISTREAM_HH_
#define BE_CORE_MEMORY_KERNEL_ISTREAM_HH_
/*****************************************************************************/
#include    <core/memory/kernel/imemorybank.hh>

namespace BugEngine { namespace Kernel
{

class IMemoryProvider;
class IMemoryBank;

class IStream : public minitl::refcountable
{
private:
    struct MemoryState
    {
        weak<const IMemoryProvider> provider;
        weak<IMemoryBank>           bank;
    };
    static const u32 s_memoryStatesCount = 4;
private:
    MemoryState m_states[s_memoryStatesCount];
public:
    weak<IMemoryBank>           currentBank()       { return m_states[0].bank; }
    weak<const IMemoryProvider> currentProvider()   { return m_states[0].provider; }
    weak<IMemoryBank>           getBank(weak<const IMemoryProvider> provider);
};

}}

/*****************************************************************************/
#endif
