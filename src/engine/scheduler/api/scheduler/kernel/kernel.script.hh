/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_KERNEL_SCRIPT_HH_
#define BE_SCHEDULER_KERNEL_KERNEL_SCRIPT_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <resource/description.script.hh>

namespace BugEngine { namespace Kernel
{

class be_api(SCHEDULER) Kernel : public Resource::Description
{
protected:
    const inamespace   m_name;

protected:
    Kernel(const inamespace& name);
    ~Kernel();

public:
    inamespace name() const { return m_name; }
};

}}

/**************************************************************************************************/
#endif
