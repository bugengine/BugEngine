/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
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

}
