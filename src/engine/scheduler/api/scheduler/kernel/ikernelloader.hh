/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IKERNELLOADER_HH_
#define BE_SCHEDULER_KERNEL_IKERNELLOADER_HH_
/*****************************************************************************/
#include    <resource/loader.hh>

namespace BugEngine { namespace Kernel
{

class be_api(SCHEDULER) IKernelLoader : public Resource::ILoader
{
    BE_NOCOPY(IKernelLoader);
protected:
    IKernelLoader();
    ~IKernelLoader();
};

}}

/*****************************************************************************/
#endif
