/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_KERNELOBJECT_HH_
#define BE_KERNEL_CPU_KERNELOBJECT_HH_
/*****************************************************************************/
#include    <plugin/dynobject.hh>

namespace BugEngine
{

struct KernelObjectParameter
{
    void* begin;
    void* end;
};

class KernelObject : public minitl::refcountable
{
private:
    typedef void(KernelMain)(KernelObjectParameter params[]);
private:
    Plugin::DynamicObject   m_kernel;
    KernelMain*             m_entryPoint;
public:
    KernelObject(const inamespace& name);
    ~KernelObject();

    void run(KernelObjectParameter params[]);
};

}


/*****************************************************************************/
#endif
