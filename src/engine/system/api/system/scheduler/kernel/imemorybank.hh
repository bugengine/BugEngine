/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_IMEMORYBANK_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_IMEMORYBANK_HH_
/*****************************************************************************/

namespace BugEngine { namespace Kernel
{

class IMemoryBank : public minitl::pointer
{
private:
    weak<const IMemoryProvider> m_provider;
};

}}

/*****************************************************************************/
#endif
