/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_KERNEL_IMEMORYPROVIDER_HH_
#define BE_CORE_MEMORY_KERNEL_IMEMORYPROVIDER_HH_
/*****************************************************************************/

namespace BugEngine { namespace Kernel
{

class be_api(CORE) IMemoryProvider : public minitl::pointer
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
