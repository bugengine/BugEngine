/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_CODELOADER_HH_
#define BE_COMPUTE_OPENCL_CODELOADER_HH_
/**************************************************************************************************/
#include    <opencl/stdafx.h>
#include    <scheduler/kernel/icodeloader.hh>


namespace BugEngine { namespace Kernel { namespace OpenCL
{

class CodeLoader : public ICodeLoader
{
public:
    CodeLoader();
    ~CodeLoader();

    virtual void load(weak<const Resource::Description> kernelDescription,
                      Resource::Resource& resource) override;
    virtual void reload(weak<const Resource::Description> oldKernelDescription,
                        weak<const Resource::Description> newKernelDescription,
                        Resource::Resource& resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}}}


/**************************************************************************************************/
#endif
