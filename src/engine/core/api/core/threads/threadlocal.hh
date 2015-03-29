/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_THREADLOCAL_HH_
#define BE_CORE_THREADS_THREADLOCAL_HH_
/**************************************************************************************************/
#include    <core/stdafx.h>


template< typename T > struct thread;

namespace BugEngine
{

class be_api(CORE) ThreadLocal
{
    template< typename T > friend struct ::thread;
private:
    static void* tlsAlloc();
    static void  tlsFree(void* key);
    static void* tlsGet(void* key);
    static void  tlsSet(void* key, void* data);
};

}

template< typename T >
struct thread
{
private:
    void*   m_tlsKey;

public:
    thread()
        :   m_tlsKey(BugEngine::ThreadLocal::tlsAlloc())
    {
    }

    thread(const thread& other)
        :   m_tlsKey(BugEngine::ThreadLocal::tlsAlloc())
    {
        *this = other;
    }

    ~thread()
    {
        BugEngine::ThreadLocal::tlsFree(m_tlsKey);
    }

    thread& operator=(T* t)
    {
        BugEngine::ThreadLocal::tlsSet(m_tlsKey, reinterpret_cast<void*>(t));
        return *this;
    }

    thread& operator=(weak<T> t)
    {
        BugEngine::ThreadLocal::tlsSet(m_tlsKey, reinterpret_cast<void*>(t.operator->()));
        return *this;
    }

    thread& operator=(ref<T> t)
    {
        BugEngine::ThreadLocal::tlsSet(m_tlsKey, reinterpret_cast<void*>(t.operator->()));
        return *this;
    }

    thread& operator=(scoped<T> t)
    {
        BugEngine::ThreadLocal::tlsSet(m_tlsKey, reinterpret_cast<void*>(t.operator->()));
        return *this;
    }

    operator T*()
    {
        return reinterpret_cast<T*>(BugEngine::ThreadLocal::tlsGet(m_tlsKey));
    }

    operator const T*() const
    {
        return reinterpret_cast<const T*>(BugEngine::ThreadLocal::tlsGet(m_tlsKey));
    }

    T* operator->()
    {
        return reinterpret_cast<T*>(BugEngine::ThreadLocal::tlsGet(m_tlsKey));
    }

    const T* operator->() const
    {
        return reinterpret_cast<const T*>(BugEngine::ThreadLocal::tlsGet(m_tlsKey));
    }

    operator const void*() const
    {
        return BugEngine::ThreadLocal::tlsGet(m_tlsKey);
    }

    bool operator!() const
    {
        return BugEngine::ThreadLocal::tlsGet(m_tlsKey) == 0;
    }
};

/**************************************************************************************************/
#endif
