/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_KERNEL_IMEMORYBANK_HH_
#define BE_CORE_MEMORY_KERNEL_IMEMORYBANK_HH_
/*****************************************************************************/

namespace BugEngine { namespace Kernel
{

class IMemoryProvider;

class IMemoryBank : public minitl::pointer
{
private:
    weak<const IMemoryProvider> m_provider;
};

}}

/*****************************************************************************/
#endif
