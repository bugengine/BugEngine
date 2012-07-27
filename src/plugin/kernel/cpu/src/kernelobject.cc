/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelobject.hh>


namespace BugEngine
{

KernelObject::KernelObject(const inamespace& name)
    :   m_handle(loadKernel(name))
    ,   m_kernel(loadSymbol(m_handle, "kmain"))
{
}

KernelObject::~KernelObject()
{
    unloadKernel(m_handle);
}

#ifdef BE_STATIC

void* KernelObject::loadKernel(const inamespace& kernel)
{
    return 0;
}

void* KernelObject::loadSymbol(void* handle, const char *name)
{
    return 0;
}

void KernelObject::unloadKernel(void* handle)
{
}

#endif

}
