/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_REFPTR_
#define BE_MINITL_PTR_REFPTR_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>


namespace minitl
{

template< typename T >
class refptr
{
private:
    T* m_ptr;
private:
    void swap(refptr& other)
    {
        T* tmp = other.m_ptr;
        other.m_ptr = m_ptr;
        m_ptr = tmp; 
    }
public:
    refptr() : m_ptr(0) {}
    refptr(T* value) : m_ptr(value) { minitl::addref(value); }
    refptr(const refptr& other) : m_ptr(other.m_ptr) { minitl::addref(m_ptr); }
    template< typename U >
    refptr(U* other) : m_ptr(other)  { minitl::addref(m_ptr); }
    template< typename U >
    refptr(refptr<U> other) : m_ptr(other.get())  { minitl::addref(m_ptr); }

    refptr& operator=(const refptr& other) { refptr(other).swap(*this); return *this; }
    template< typename U >
    refptr& operator=(const refptr<U>& other) { refptr(other).swap(*this); return *this; }
    template< typename U >
    refptr& operator=(U* other) { refptr(other).swap(*this); return *this; }


    ~refptr() { if(m_ptr) minitl::decref(m_ptr); }

    T& operator*()  const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    T* get(void)    const { return m_ptr; }
    operator const void*() const { return m_ptr; }
    bool operator!() const { return m_ptr == 0; }
};

template<typename T, typename U>
bool operator==(const refptr<T>& ref1, const refptr<U>& ref2) { return ref1.get() == ref2.get(); }  
template<typename T, typename U>
bool operator==(const refptr<T>& ref1, const U* ptr2) { return ref1.get() == ptr2; }  

template<typename T, typename U>
bool operator!=(const refptr<T>& ref1, const refptr<U>& ref2) { return ref1.get() != ref2.get(); }
template<typename T, typename U>
bool operator!=(const refptr<T>& ref1, const U* ptr2) { return ref1.get() != ptr2; }

template<typename T, typename U>
bool operator<(const refptr<T>& ref1, const refptr<U>& ref2) { return ref1.get() < ref2.get(); }  
template<typename T, typename U>
bool operator<(const refptr<T>& ref1, const U* ptr2) { return ref1.get() < ptr2; }
  
template<typename T, typename U>
bool operator<=(const refptr<T>& ref1, const refptr<U>& ref2) { return ref1.get() <= ref2.get(); }
template<typename T, typename U>
bool operator<=(const refptr<T>& ref1, const U* ptr2) { return ref1.get() <= ptr2; }

template<typename T, typename U>
bool operator>(const refptr<T>& ref1, const refptr<U>& ref2) { return ref1.get() > ref2.get(); }  
template<typename T, typename U>
bool operator>(const refptr<T>& ref1, const U* ptr2) { return ref1.get() > ptr2; }  

template<typename T, typename U>
bool operator>=(const refptr<T>& ref1, const refptr<U>& ref2) { return ref1.get() >= ref2.get(); }
template<typename T, typename U>
bool operator>=(const refptr<T>& ref1, const U* ptr2) { return ref1.get() >= ptr2; }

}

/*****************************************************************************/
#endif
