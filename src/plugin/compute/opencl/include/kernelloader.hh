/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_KERNELLOADER_HH_
#define BE_COMPUTE_OPENCL_KERNELLOADER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/scheduler/kernel/ikernelloader.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

class CodeLoader;

class KernelLoader : public IKernelLoader
{
public:
    KernelLoader(ref< CodeLoader > codeLoader);
    ~KernelLoader();

    virtual void load(weak< const Resource::Description > kernelDescription,
                      Resource::Resource&                 resource) override;
    virtual void reload(weak< const Resource::Description > oldKernelDescription,
                        weak< const Resource::Description > newKernelDescription,
                        Resource::Resource&                 resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}}}  // namespace BugEngine::KernelScheduler::OpenCL

/**************************************************************************************************/
#endif
