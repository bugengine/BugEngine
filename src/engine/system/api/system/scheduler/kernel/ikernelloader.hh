/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_KERNEL_IKERNELLOADER_HH_
#define BE_SYSTEM_SCHEDULER_KERNEL_IKERNELLOADER_HH_
/*****************************************************************************/
#include    <system/resource/resourceloader.hh>

namespace BugEngine { namespace Kernel
{

class be_api(SYSTEM) IKernelLoader : public IResourceLoader
{
    BE_NOCOPY(IKernelLoader);
protected:
    IKernelLoader();
    ~IKernelLoader();
};

}}

/*****************************************************************************/
#endif
