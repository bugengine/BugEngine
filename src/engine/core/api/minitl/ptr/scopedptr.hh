/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_SCOPEDPTR_
#define BE_MINITL_PTR_SCOPEDPTR_
/*****************************************************************************/

namespace minitl
{

template< typename T >
class scopedptr
{
private:
    T* m_ptr;
public:
    scopedptr() NOTHROW : m_ptr(0)              { }
    scopedptr(T* value) NOTHROW : m_ptr(value)  { }
    ~scopedptr()                                { if(m_ptr) checked_delete<T>(m_ptr); }

    void reset(T* value) NOTHROW                { if(m_ptr) checked_delete<T>(m_ptr); m_ptr = value; }
    

    T& operator*() NOTHROW                      { return *m_ptr; }
    const T& operator*()  const NOTHROW         { return *m_ptr; }
    T* operator->() NOTHROW                     { return m_ptr; }
    const T* operator->() const NOTHROW         { return m_ptr; }
    T* get(void) NOTHROW                        { return m_ptr; }
    const T* get(void) const NOTHROW            { return m_ptr; }
    operator bool() const NOTHROW               { return m_ptr != 0; }
    bool operator!() const NOTHROW              { return m_ptr == 0; }
private:
    scopedptr(const scopedptr& other);
    scopedptr& operator=(const scopedptr& other);
};

template<typename T, typename U>
bool operator==(const scopedptr<T>& ref1, const scopedptr<U>& ref2) { return ref1.get() == ref2.get(); }  
template<typename T, typename U>
bool operator==(const scopedptr<T>& ref1, const U* ptr2) { return ref1.get() == ptr2; }  

template<typename T, typename U>
bool operator!=(const scopedptr<T>& ref1, const scopedptr<U>& ref2) { return ref1.get() != ref2.get(); }
template<typename T, typename U>
bool operator!=(const scopedptr<T>& ref1, const U* ptr2) { return ref1.get() != ptr2; }

template<typename T, typename U>
bool operator<(const scopedptr<T>& ref1, const scopedptr<U>& ref2) { return ref1.get() < ref2.get(); }  
template<typename T, typename U>
bool operator<(const scopedptr<T>& ref1, const U* ptr2) { return ref1.get() < ptr2; }
  
template<typename T, typename U>
bool operator<=(const scopedptr<T>& ref1, const scopedptr<U>& ref2) { return ref1.get() <= ref2.get(); }
template<typename T, typename U>
bool operator<=(const scopedptr<T>& ref1, const U* ptr2) { return ref1.get() <= ptr2; }

template<typename T, typename U>
bool operator>(const scopedptr<T>& ref1, const scopedptr<U>& ref2) { return ref1.get() > ref2.get(); }  
template<typename T, typename U>
bool operator>(const scopedptr<T>& ref1, const U* ptr2) { return ref1.get() > ptr2; }  

template<typename T, typename U>
bool operator>=(const scopedptr<T>& ref1, const scopedptr<U>& ref2) { return ref1.get() >= ref2.get(); }
template<typename T, typename U>
bool operator>=(const scopedptr<T>& ref1, const U* ptr2) { return ref1.get() >= ptr2; }

}

/*****************************************************************************/
#endif
