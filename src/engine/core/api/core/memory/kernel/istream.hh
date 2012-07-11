/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_KERNEL_ISTREAM_HH_
#define BE_CORE_MEMORY_KERNEL_ISTREAM_HH_
/*****************************************************************************/

namespace BugEngine { namespace Kernel
{

class IMemoryProvider;

class IStream : public minitl::refcountable
{
protected:
//    scoped<IMemoryProvider> m_providers[8];
public:
//    weak<const IMemoryProvider> provider(u32 index) const    { return m_providers[index]; };
};

}}

/*****************************************************************************/
#endif
