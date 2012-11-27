/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelobject.hh>


namespace BugEngine
{

KernelObject::KernelObject(const inamespace& name)
    :   m_kernel(name, "kernels")
    ,   m_entryPoint(m_kernel.getSymbol<KernelMain>("_kmain"))
{
    be_debug("kernel entry point: %p"|m_entryPoint);
}

KernelObject::~KernelObject()
{
}

void KernelObject::run(const u32 index, const u32 total, KernelObjectParameter params[])
{
    (*m_entryPoint)(index, total, params);
}

}
