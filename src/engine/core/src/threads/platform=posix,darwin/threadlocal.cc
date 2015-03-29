/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/threadlocal.hh>

#include    <pthread.h>
#include    <cerrno>
#include    <core/timer.hh>

namespace BugEngine
{

void* ThreadLocal::tlsAlloc()
{
    pthread_key_t key;
    pthread_key_create(&key, 0);
    return reinterpret_cast<void*>(key);
}


void ThreadLocal::tlsFree(void* key)
{
    pthread_key_t k = static_cast<pthread_key_t>((uintptr_t)key);
    pthread_key_delete(k);
}

void* ThreadLocal::tlsGet(void* key)
{
    pthread_key_t k = static_cast<pthread_key_t>((uintptr_t)key);
    return const_cast<void*>(pthread_getspecific(k));
}

void ThreadLocal::tlsSet(void* key, void* value)
{
    pthread_key_t k = static_cast<pthread_key_t>((uintptr_t)key);
    pthread_setspecific(k, value);
}

}
