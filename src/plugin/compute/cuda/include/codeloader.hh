/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_CUDA_CODELOADER_HH_
#define BE_COMPUTE_CUDA_CODELOADER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <bugengine/scheduler/kernel/icodeloader.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

class CodeLoader : public ICodeLoader
{
public:
    CodeLoader();
    ~CodeLoader();

    virtual void load(weak< const Resource::Description > kernelDescription,
                      Resource::Resource&                 resource) override;
    virtual void reload(weak< const Resource::Description > oldKernelDescription,
                        weak< const Resource::Description > newKernelDescription,
                        Resource::Resource&                 resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}}}  // namespace BugEngine::KernelScheduler::Cuda

/**************************************************************************************************/
#endif
