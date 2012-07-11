/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
private:
    static void* loadKernel(const inamespace& name);
    static void  unloadKernel(void* handle);
    static void* loadSymbol(void* handle, const char *name);
public:
    KernelObject(const inamespace& name);
    ~KernelObject();
};

}


/*****************************************************************************/
#endif
