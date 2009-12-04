/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_SCOPEDPTR_
#define BE_MINITL_PTR_SCOPEDPTR_
/*****************************************************************************/

namespace minitl
{

template< typename T >
class scoped
{
    template< typename U >
    friend class scoped;
private:
    mutable T* m_ptr;
private:
    scoped(T* value) NOTHROW : m_ptr(value)  { }
public:
#define be_create scoped
#include "factory0.inl"
#include "factory1.inl"
#include "factory2.inl"
#include "factory3.inl"
#include "factory4.inl"
#include "factory5.inl"
#include "factory6.inl"
#undef be_create
public:
    scoped() NOTHROW : m_ptr(0)                         { }
    ~scoped()                                           { checked_delete<T>(m_ptr); }
    scoped(const scoped& other) : m_ptr(other.m_ptr)    { other.m_ptr = 0; }
    template< typename U >
    scoped(const scoped<U>& other) : m_ptr(other.m_ptr) { other.m_ptr = 0; }
    scoped& operator=(const scoped& other)              { checked_delete<T>(m_ptr); m_ptr = other.m_ptr; other.m_ptr = 0; }

    T* operator->() const NOTHROW        { return m_ptr; }
    operator const void*() const NOTHROW { return m_ptr; }
    bool operator!() const NOTHROW       { return m_ptr == 0; }
private:
};

template<typename T, typename U>
bool operator==(const scoped<T>& ref1, const scoped<U>& ref2) { return ref1.operator->() == ref2.operator->(); }  

template<typename T, typename U>
bool operator!=(const scoped<T>& ref1, const scoped<U>& ref2) { return ref1.operator->() != ref2.operator->(); }

template<typename T, typename U>
bool operator<(const scoped<T>& ref1, const scoped<U>& ref2) { return ref1.operator->() < ref2.operator->(); }  

template<typename T, typename U>
bool operator<=(const scoped<T>& ref1, const scoped<U>& ref2) { return ref1.operator->() <= ref2.operator->(); }

template<typename T, typename U>
bool operator>(const scoped<T>& ref1, const scoped<U>& ref2) { return ref1.operator->() > ref2.operator->(); }  

template<typename T, typename U>
bool operator>=(const scoped<T>& ref1, const scoped<U>& ref2) { return ref1.operator->() >= ref2.operator->(); }

}

/*****************************************************************************/
#endif
