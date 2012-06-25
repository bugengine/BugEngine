/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_KERNELOBJECT_HH_
#define BE_KERNEL_CPU_KERNELOBJECT_HH_
/*****************************************************************************/


namespace BugEngine
{

class KernelObject : public minitl::refcountable
{
private:
    void*   m_handle;
    void*   m_kernel;
public:
    KernelObject(const inamespace& name);
    ~KernelObject();
};

}


/*****************************************************************************/
#endif
