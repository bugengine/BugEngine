/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_KERNEL_SCRIPT_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_KERNEL_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Kernel
{

class be_api(SYSTEM) Kernel : public Resource
{
private:
    inamespace const    m_name;
public:
    Kernel(const inamespace& name);
    const inamespace& name() const { return m_name; }
};

}}

/*****************************************************************************/
#endif
