/* BugEngine <bugengine.devel@gmail.com>
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
    const ref< ICodeLoader > m_codeLoader;

protected:
    IKernelLoader(ref< ICodeLoader > codeLoader);
    ~IKernelLoader();

public:
    weak< const ICodeLoader > codeLoader() const
    {
        return m_codeLoader;
    }
    weak< ICodeLoader > codeLoader()
    {
        return m_codeLoader;
    }
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
