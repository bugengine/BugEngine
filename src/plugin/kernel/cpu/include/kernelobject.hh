/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_KERNELOBJECT_HH_
#define BE_KERNEL_CPU_KERNELOBJECT_HH_
/*****************************************************************************/
#include    <plugin/dynobject.hh>

namespace BugEngine
{

namespace Kernel
{
struct KernelParameter;
}

class KernelObject : public minitl::refcountable
{
private:
    typedef void(KernelMain)(Kernel::KernelParameter params[]);
private:
    Plugin::DynamicObject   m_kernel;
    KernelMain*             m_entryPoint;
public:
    KernelObject(const inamespace& name);
    ~KernelObject();

    void run(Kernel::KernelParameter params[]);
};

}


/*****************************************************************************/
#endif
