/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_REFPTR_
#define BE_MINITL_PTR_REFPTR_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>


namespace minitl
{

template< typename T >
class ref
{
    template< typename U, typename V >
    friend ref<U> be_checked_cast(ref<V> v);
    template< typename U, typename V >
    friend ref<U> be_const_cast(ref<V> v);
private:
    T* m_ptr;
private:
    void swap(ref& other)
    {
        T* tmp = other.m_ptr;
        other.m_ptr = m_ptr;
        m_ptr = tmp; 
    }
private:
    ref(T* value) : m_ptr(value) { minitl::addref(value); }
public:
#define be_create ref
#include "factory0.inl"
#include "factory1.inl"
#include "factory2.inl"
#include "factory3.inl"
#include "factory4.inl"
#include "factory5.inl"
#include "factory6.inl"
#undef be_create
public:
    ref() : m_ptr(0) {}
    ref(const ref& other) : m_ptr(other.m_ptr) { addref(m_ptr); }
    template< typename U >
    ref(const ref<U> other) : m_ptr(other.operator->())  { addref(m_ptr); }
    template< typename U >
    ref(const scoped<U> other) : m_ptr(other.operator->())  { addref(m_ptr); other.m_ptr = 0; }

    ref& operator=(const ref& other) { ref(other).swap(*this); return *this; }
    template< typename U >
    ref& operator=(const ref<U>& other) { ref(other).swap(*this); return *this; }

    ~ref() { decref(m_ptr); }

    T* operator->() const { return m_ptr; }
    operator const void*() const { return m_ptr; }
    bool operator!() const { return m_ptr == 0; }
    T& operator*() { return *m_ptr; }

    void clear() { decref(m_ptr); m_ptr = 0; }
};

template<typename T, typename U>
bool operator==(const ref<T>& ref1, const ref<U>& ref2) { return ref1.operator->() == ref2.operator->(); }  

template<typename T, typename U>
bool operator!=(const ref<T>& ref1, const ref<U>& ref2) { return ref1.operator->() != ref2.operator->(); }

template<typename T, typename U>
bool operator<(const ref<T>& ref1, const ref<U>& ref2) { return ref1.operator->() < ref2.operator->(); }  

template<typename T, typename U>
bool operator<=(const ref<T>& ref1, const ref<U>& ref2) { return ref1.operator->() <= ref2.operator->(); }

template<typename T, typename U>
bool operator>(const ref<T>& ref1, const ref<U>& ref2) { return ref1.operator->() > ref2.operator->(); }  

template<typename T, typename U>
bool operator>=(const ref<T>& ref1, const ref<U>& ref2) { return ref1.operator->() >= ref2.operator->(); }

}

/*****************************************************************************/
#endif
