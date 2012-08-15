/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelobject.hh>
#include    <plugin/plugin.hh>


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
