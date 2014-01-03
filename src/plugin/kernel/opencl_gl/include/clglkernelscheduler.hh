/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_OPENCL_OPENGL_CLGLKERNELSCHEDULER_HH_
#define BE_KERNEL_OPENCL_OPENGL_CLGLKERNELSCHEDULER_HH_
/*****************************************************************************/
#include    <opencl/clkernelscheduler.hh>
#include    <plugin/plugin.hh>

namespace BugEngine
{

class OpenCLOpenGLKernelScheduler : public OpenCLKernelScheduler
{
private:
    static minitl::array<cl_context_properties> createPlatformSpecificContextProperties();
public:
    OpenCLOpenGLKernelScheduler(const Plugin::Context& context);
    ~OpenCLOpenGLKernelScheduler();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}


/*****************************************************************************/
#endif
