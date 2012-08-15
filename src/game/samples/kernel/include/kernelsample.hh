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

namespace BugEngine { namespace Samples
{

class KernelSampleTask;

class KernelSample : public Application
{
private:
    scoped<KernelSampleTask> m_kernelTask;
public:
    KernelSample(const PluginContext& context);
    ~KernelSample();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

/*****************************************************************************/
#endif

