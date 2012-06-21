/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_KERNEL_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_KERNEL_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine
{

class be_api(SYSTEM) Kernel : public Resource
{
private:
    inamespace const    m_name;
public:
    Kernel(const inamespace& name);
    const inamespace& name() const { return m_name; }
};

}

/*****************************************************************************/
#endif
