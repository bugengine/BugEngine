/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IKERNELLOADER_HH_
#define BE_SCHEDULER_KERNEL_IKERNELLOADER_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/resource/loader.hh>

namespace BugEngine { namespace KernelScheduler {

class ICodeLoader;

class be_api(SCHEDULER) IKernelLoader : public Resource::ILoader
{
    BE_NOCOPY(IKernelLoader);

protected:
    const ref< const ICodeLoader > m_codeLoader;

protected:
    IKernelLoader(ref< const ICodeLoader > codeLoader);
    ~IKernelLoader();

public:
    weak< const ICodeLoader > codeLoader() const
    {
        return m_codeLoader;
    }
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
