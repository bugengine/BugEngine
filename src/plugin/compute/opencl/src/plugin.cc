/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin.compute.opencl/scheduler.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <platform.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

class PlatformLoader : public minitl::refcountable
{
private:
    minitl::vector< ref< Platform > >  m_platforms;
    minitl::vector< ref< Scheduler > > m_schedulers;

public:
    PlatformLoader(const Plugin::Context& context);
    ~PlatformLoader();
};

PlatformLoader::PlatformLoader(const Plugin::Context& context)
    : m_platforms(Platform::loadPlatforms())
    , m_schedulers(Arena::task())
{
    for(minitl::vector< ref< Platform > >::const_iterator it = m_platforms.begin();
        it != m_platforms.end(); ++it)
    {
        for(minitl::vector< ref< Context > >::const_iterator ctxIt = (*it)->contextBegin();
            ctxIt != (*it)->contextEnd(); ++ctxIt)
        {
            m_schedulers.push_back(ref< Scheduler >::create(Arena::task(), context, *ctxIt));
        }
    }
}

PlatformLoader::~PlatformLoader()
{
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL

BE_PLUGIN_REGISTER(BugEngine::KernelScheduler::OpenCL::PlatformLoader);
