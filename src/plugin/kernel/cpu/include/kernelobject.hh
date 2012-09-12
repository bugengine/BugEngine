/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_KERNELOBJECT_HH_
#define BE_KERNEL_CPU_KERNELOBJECT_HH_
/*****************************************************************************/
#include    <plugin/dynobject.hh>

namespace BugEngine
{

class KernelObject : public minitl::refcountable
{
private:
    Plugin::DynamicObject   m_kernel;
public:
    KernelObject(const inamespace& name);
    ~KernelObject();
};

}


/*****************************************************************************/
#endif
