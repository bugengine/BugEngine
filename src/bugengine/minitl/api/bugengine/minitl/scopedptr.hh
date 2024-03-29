/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_SCOPEDPTR_HH_
#define BE_MINITL_SCOPEDPTR_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>

namespace minitl {

template < typename T >
class scoped
{
    template < typename U >
    friend class scoped;
    template < typename U >
    friend class ref;

private:
    mutable T* m_ptr;

private:
    scoped(T* value, Allocator& deleter);
    template < typename U >
    scoped& operator=(const scoped< U >& other);
    scoped& operator=(const scoped& other);

public:
    inline scoped();
    inline ~scoped();
    inline scoped(const scoped& other);
    template < typename U >
    inline scoped(const scoped< U >& other);

    inline T*   operator->() const;
    inline      operator const void*() const;
    inline bool operator!() const;
    inline T&   operator*();

    template < typename U >
    inline void reset(const scoped< U >& other);

    static inline scoped< T > create(Allocator& allocator)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return scoped< T >(new(mem) T(), allocator);
    }

    template < class A1 >
    static inline scoped< T > create(Allocator& allocator, const A1& a1)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return scoped< T >(new(mem) T(a1), allocator);
    }

    template < class A1, class A2 >
    static inline scoped< T > create(Allocator& allocator, const A1& a1, const A2& a2)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return scoped< T >(new(mem) T(a1, a2), allocator);
    }

    template < class A1, class A2, class A3 >
    static inline scoped< T > create(Allocator& allocator, const A1& a1, const A2& a2, const A3& a3)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return scoped< T >(new(mem) T(a1, a2, a3), allocator);
    }

    template < class A1, class A2, class A3, class A4 >
    static inline scoped< T > create(Allocator& allocator, const A1& a1, const A2& a2, const A3& a3,
                                     const A4& a4)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return scoped< T >(new(mem) T(a1, a2, a3, a4), allocator);
    }

    template < class A1, class A2, class A3, class A4, class A5 >
    static inline scoped< T > create(Allocator& allocator, const A1& a1, const A2& a2, const A3& a3,
                                     const A4& a4, const A5& a5)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return scoped< T >(new(mem) T(a1, a2, a3, a4, a5), allocator);
    }

    template < class A1, class A2, class A3, class A4, class A5, class A6 >
    static inline scoped< T > create(Allocator& allocator, const A1& a1, const A2& a2, const A3& a3,
                                     const A4& a4, const A5& a5, const A6& a6)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return scoped< T >(new(mem) T(a1, a2, a3, a4, a5, a6), allocator);
    }
};

template < u16 SIZE >
class format;
template < typename T, u16 SIZE >
const format< SIZE >& operator|(const format< SIZE >& format, const scoped< T >& t)
{
    return format | t.operator->();
}

}  // namespace minitl

#include <bugengine/minitl/inl/scopedptr.inl>

/**************************************************************************************************/
#endif
