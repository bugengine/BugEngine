/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_SCOPEDPTR_HH_
#define BE_MINITL_PTR_SCOPEDPTR_HH_
/*****************************************************************************/

namespace minitl
{

template< typename T >
class scoped
{
    template< typename U >
    friend class scoped;
    template< typename U >
    friend class ref;
private:
    mutable T*  m_ptr;
private:
    scoped(T* value, BugEngine::Allocator& deleter);
public:
    inline scoped();
    inline ~scoped();
    inline scoped(const scoped& other);
    template< typename U > inline scoped(const scoped<U>& other);
    inline scoped& operator=(const scoped& other);

    inline T* operator->() const;
    inline operator const void*() const;
    inline bool operator!() const;
    inline T& operator*();

#define be_pointer_ scoped
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

#include   <minitl/ptr/scopedptr.inl>

/*****************************************************************************/
#endif
