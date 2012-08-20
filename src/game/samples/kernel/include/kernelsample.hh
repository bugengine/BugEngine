/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_KERNELSAMPLE_HH_
#define BE_SAMPLES_KERNEL_KERNELSAMPLE_HH_
/*****************************************************************************/
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
    Plugin<IResourceLoader>     const   m_packageManager;
    Plugin<IResourceLoader>     const   m_lua;
    ref<const Package>          const   m_mainPackage;
public:
    KernelSample(const PluginContext& context);
    ~KernelSample();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}

/*****************************************************************************/
#endif

