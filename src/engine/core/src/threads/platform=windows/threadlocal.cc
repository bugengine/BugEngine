/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/threadlocal.hh>

namespace BugEngine
{

void* ThreadLocal::tlsAlloc()
{
    DWORD key = TlsAlloc();
    return reinterpret_cast<void*>((uintptr_t)(key));
}


void ThreadLocal::tlsFree(void* key)
{
    DWORD k = static_cast<DWORD>((uintptr_t)key);
    TlsFree(k);
}

void* ThreadLocal::tlsGet(void* key)
{
    DWORD k = static_cast<DWORD>((uintptr_t)key);
    return TlsGetValue(k);
}

void ThreadLocal::tlsSet(void* key, void* value)
{
    DWORD k = static_cast<DWORD>((uintptr_t)key);
    TlsSetValue(k, value);
}

}
