/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_THREADLOCAL_HH_
#define BE_CORE_THREADS_THREADLOCAL_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>

template < typename T >
struct tls;

namespace BugEngine {

class be_api(CORE) ThreadLocal
{
    template < typename T >
    friend struct ::tls;

private:
    static void* tlsAlloc();
    static void  tlsFree(void* key);
    static void* tlsGet(void* key);
    static void  tlsSet(void* key, void* data);
};

}  // namespace BugEngine

template < typename T >
struct tls
{
private:
    void* m_tlsKey;

public:
    tls() : m_tlsKey(BugEngine::ThreadLocal::tlsAlloc())
    {
    }

    tls(const tls& other) : m_tlsKey(BugEngine::ThreadLocal::tlsAlloc())
    {
        *this = other;
    }

    ~tls()
    {
        BugEngine::ThreadLocal::tlsFree(m_tlsKey);
    }

    tls& operator=(T* t)
    {
        BugEngine::ThreadLocal::tlsSet(m_tlsKey, reinterpret_cast< void* >(t));
        return *this;
    }

    tls& operator=(weak< T > t)
    {
        BugEngine::ThreadLocal::tlsSet(m_tlsKey, reinterpret_cast< void* >(t.operator->()));
        return *this;
    }

    tls& operator=(ref< T > t)
    {
        BugEngine::ThreadLocal::tlsSet(m_tlsKey, reinterpret_cast< void* >(t.operator->()));
        return *this;
    }

    tls& operator=(scoped< T > t)
    {
        BugEngine::ThreadLocal::tlsSet(m_tlsKey, reinterpret_cast< void* >(t.operator->()));
        return *this;
    }

    operator T*()
    {
        return reinterpret_cast< T* >(BugEngine::ThreadLocal::tlsGet(m_tlsKey));
    }

    operator const T*() const
    {
        return reinterpret_cast< const T* >(BugEngine::ThreadLocal::tlsGet(m_tlsKey));
    }

    T* operator->()
    {
        return reinterpret_cast< T* >(BugEngine::ThreadLocal::tlsGet(m_tlsKey));
    }

    const T* operator->() const
    {
        return reinterpret_cast< const T* >(BugEngine::ThreadLocal::tlsGet(m_tlsKey));
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
