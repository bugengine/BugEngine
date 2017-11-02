/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_ICODELOADER_HH_
#define BE_SCHEDULER_KERNEL_ICODELOADER_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <resource/loader.hh>

namespace BugEngine { namespace Kernel
{

class be_api(SCHEDULER) ICodeLoader : public Resource::ILoader
{
    BE_NOCOPY(ICodeLoader)
protected:
    ICodeLoader();
    ~ICodeLoader();
};

}}

/**************************************************************************************************/
#endif
