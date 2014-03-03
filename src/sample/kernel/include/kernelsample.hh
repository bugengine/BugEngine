/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_KERNELSAMPLE_HH_
#define BE_SAMPLES_KERNEL_KERNELSAMPLE_HH_
/**************************************************************************************************/
#include    <filesystem/folder.script.hh>
#include    <bugengine/application.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <plugin/plugin.hh>
#include    <package/package.script.hh>

namespace BugEngine
{

class KernelSampleTask;

class KernelSample : public Application
{
private:
    Plugin::Plugin<Resource::ILoader>   const   m_packageManager;
    Plugin::Plugin<Resource::ILoader>   const   m_lua;
    ref<const Package>                  const   m_mainPackage;
public:
    KernelSample(const Plugin::Context& context);
    ~KernelSample();
};

}

/**************************************************************************************************/
#endif

