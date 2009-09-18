/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_WEAKPTR_
#define BE_MINITL_PTR_WEAKPTR_
/*****************************************************************************/
#include <minitl/ptr/refcountable.hh>


namespace minitl
{

template< typename T >
class weakptr
{
private:
    T* m_ptr;
private:
    void swap(weakptr& other)
    {
        T* tmp = other.m_ptr;
        other.m_ptr = m_ptr;
        m_ptr = tmp; 
    }
public:
    weakptr() : m_ptr(0) {}
    template< typename U >
    weakptr(refptr<U> other) : m_ptr(other.get()) { minitl::addweak(m_ptr); }
    weakptr(const weakptr& other) : m_ptr(other.m_ptr) { minitl::addweak(m_ptr); }
    ~weakptr() { minitl::decweak(m_ptr); }

    weakptr& operator=(const weakptr& other) { weakptr(other).swap(*this); return *this; }
    template< typename U >
    weakptr& operator=(const weakptr<U>& other) { weakptr(other).swap(*this); return *this; }
    template< typename U >
    weakptr& operator=(U* other) { weakptr(other).swap(*this); return *this; }

    T& operator*()  const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    T* get(void)    const { return m_ptr; }
    operator void*() const { return m_ptr; }
    bool operator!() const { return m_ptr == 0; }
};

}

/*****************************************************************************/
#endif
