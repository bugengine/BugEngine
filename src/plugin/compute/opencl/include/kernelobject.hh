/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_COMPUTE_OPENCL_KERNELOBJECT_HH_
#define BE_COMPUTE_OPENCL_KERNELOBJECT_HH_
/**************************************************************************************************/
#include    <opencl/stdafx.h>
#include    <opencl/scheduler.hh>
#include    <plugin/dynobject.hh>
#include    <scheduler/task/task.hh>


namespace BugEngine { namespace KernelScheduler { namespace OpenCL
{

class KernelObject : public minitl::refcountable
{
    friend class Scheduler;
private:
    typedef const unsigned char* KernelBlob;
private:
    Plugin::DynamicObject   m_kernel;
    KernelBlob              m_kernelBlob;
    Scheduler::ClKernel     m_program;
public:
    KernelObject(weak<const Scheduler> scheduler, const inamespace& name);
    ~KernelObject();

    void run(const minitl::array< weak<const IMemoryBuffer> >& params);
};

}}}


/**************************************************************************************************/
#endif
