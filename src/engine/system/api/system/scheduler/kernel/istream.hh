/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_ISTREAM_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_ISTREAM_HH_
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

template< typename T >
class Stream : public IStream
{
    
};

}}

/*****************************************************************************/
#endif
