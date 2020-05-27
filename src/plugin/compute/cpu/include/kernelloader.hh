/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_KERNELLOADER_HH_
#define BE_COMPUTE_CPU_KERNELLOADER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/scheduler/kernel/ikernelloader.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

class CodeLoader;

class KernelLoader : public IKernelLoader
{
public:
    KernelLoader(ref< const CodeLoader > codeLoader);
    ~KernelLoader();

    virtual void load(weak< const Resource::Description > kernelDescription,
                      Resource::Resource&                 resource) override;
    virtual void reload(weak< const Resource::Description > oldKernelDescription,
                        weak< const Resource::Description > newKernelDescription,
                        Resource::Resource&                 resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}}}  // namespace BugEngine::KernelScheduler::CPU

/**************************************************************************************************/
#endif
