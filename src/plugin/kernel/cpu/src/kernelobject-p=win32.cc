/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelobject.hh>
#include    <system/plugin.hh>


namespace BugEngine
{

#ifndef BE_STATIC

void* KernelObject::loadKernel(const inamespace& )
{
    return 0;
}

void* KernelObject::loadSymbol(void* , const char *)
{
    return 0;
}

void KernelObject::unloadKernel(void* )
{
}

#endif

}
