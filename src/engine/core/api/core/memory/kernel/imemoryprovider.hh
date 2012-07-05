/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_KERNEL_IMEMORYPROVIDER_HH_
#define BE_CORE_MEMORY_KERNEL_IMEMORYPROVIDER_HH_
/*****************************************************************************/

namespace BugEngine { namespace Kernel
{

class IMemoryProvider : public minitl::pointer
{
private:
    istring const   m_name;
public:
    istring name() const { return m_name; }
};

}}

/*****************************************************************************/
#endif
