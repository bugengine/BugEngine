/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_CODELOADER_HH_
#define BE_COMPUTE_CPU_CODELOADER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cpu/stdafx.h>
#include <bugengine/scheduler/kernel/icodeloader.hh>

namespace BugEngine { namespace KernelScheduler { namespace CPU {

class CodeLoader : public ICodeLoader
{
private:
    const inamespace m_cpuVariant;

public:
    CodeLoader(const inamespace& cpuVariant);
    ~CodeLoader();

    virtual void load(weak< const Resource::Description > codeDescription,
                      Resource::Resource&                 resource) override;
    virtual void reload(weak< const Resource::Description > codeOldDescription,
                        weak< const Resource::Description > codeNewDescription,
                        Resource::Resource&                 resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}}}  // namespace BugEngine::KernelScheduler::CPU

/**************************************************************************************************/
#endif
