/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_REFPTR_HH_
#define BE_MINITL_PTR_REFPTR_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>


namespace minitl
{

template< typename T >
class ref
{
    template< typename U, typename V > friend ref<U> be_checked_cast(ref<V> v);
    template< typename U, typename V > friend ref<U> be_const_cast(ref<V> v);
private:
    T*          m_ptr;
private:
    inline void swap(ref& other);
private:
    inline ref(T* value, BugEngine::Allocator& deleter);
public:
    inline ref();
    inline ref(const ref& other);
    template< typename U > inline ref(const ref<U> other);
    template< typename U > inline ref(const scoped<U> other);

    inline ref& operator=(const ref& other);
    template< typename U > inline ref& operator=(const ref<U>& other);

    inline ~ref();

    inline T* operator->() const;
    inline operator const void*() const;
    inline bool operator!() const;
    inline T& operator*();

    inline void clear();

#define be_pointer_ ref
#include "factory0.inl"
#include "factory1.inl"
#include "factory2.inl"
#include "factory3.inl"
#include "factory4.inl"
#include "factory5.inl"
#include "factory6.inl"
#undef be_pointer_
};

}
#include   <minitl/ptr/refptr.inl>

/*****************************************************************************/
#endif
