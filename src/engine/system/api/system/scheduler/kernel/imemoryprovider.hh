/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_IMEMORYPROVIDER_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_IMEMORYPROVIDER_HH_
/*****************************************************************************/

namespace BugEngine { namespace Kernel
{

class be_api(SYSTEM) IMemoryProvider : public minitl::pointer
{
private:
    istring const   m_name;
protected:
    IMemoryProvider(const istring& name);
public:
    istring name() const { return m_name; }
};

}}

/*****************************************************************************/
#endif
