/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_CPU_CODELOADER_HH_
#define BE_COMPUTE_CPU_CODELOADER_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <scheduler/kernel/icodeloader.hh>


namespace BugEngine { namespace KernelScheduler { namespace CPU
{

class CodeLoader : public ICodeLoader
{
private:
    const inamespace m_cpuVariant;
public:
    CodeLoader(const inamespace& cpuVariant);
    ~CodeLoader();

    virtual void load(weak<const Resource::Description> kernelDescription, Resource::Resource& resource) override;
    virtual void reload(weak<const Resource::Description> oldKernelDescription,
                        weak<const Resource::Description> newKernelDescription,
                         Resource::Resource& resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}}}


/**************************************************************************************************/
#endif
