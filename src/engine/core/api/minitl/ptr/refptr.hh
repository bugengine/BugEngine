/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
    template< typename OBJECT >
    struct ptr
    {
        typedef minitl::refcountable* pointer;
    };
    template< typename OBJECT >
    struct ptr<const OBJECT>
    {
        typedef const minitl::refcountable* pointer;
    };
private:
    typename ptr<T>::pointer m_ptr;
private:
    inline void swap(ref& other);
private:
    inline ref(T* value);
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

    static inline ref<T> create(::BugEngine::Allocator& allocator)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return ref<T>(new(mem) T(), allocator);
    }

    template< class A1 >
    static inline ref<T> create( ::BugEngine::Allocator& allocator,
                                 const A1& a1 )
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return ref<T>(new(mem) T(a1), allocator);
    }

    template< class A1, class A2 >
    static inline ref<T> create( ::BugEngine::Allocator& allocator,
                                 const A1& a1,
                                 const A2& a2 )
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return ref<T>(new(mem) T(a1, a2), allocator);
    }

    template< class A1, class A2, class A3  >
    static inline ref<T> create( ::BugEngine::Allocator& allocator,
                                 const A1& a1,
                                 const A2& a2,
                                 const A3& a3 )
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return ref<T>(new(mem) T(a1, a2, a3), allocator);
    }

    template< class A1, class A2, class A3, class A4 >
    static inline ref<T> create( ::BugEngine::Allocator& allocator,
                                 const A1& a1,
                                 const A2& a2,
                                 const A3& a3,
                                 const A4& a4 )
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return ref<T>(new(mem) T(a1, a2, a3, a4), allocator);
    }

    template< class A1, class A2, class A3, class A4, class A5 >
    static inline ref<T> create( ::BugEngine::Allocator& allocator,
                                 const A1& a1,
                                 const A2& a2,
                                 const A3& a3,
                                 const A4& a4,
                                 const A5& a5 )
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return ref<T>(new(mem) T(a1, a2, a3, a4, a5), allocator);
    }

    template< class A1, class A2, class A3, class A4, class A5, class A6 >
    static inline ref<T> create( ::BugEngine::Allocator& allocator,
                                 const A1& a1,
                                 const A2& a2,
                                 const A3& a3,
                                 const A4& a4,
                                 const A5& a5,
                                 const A6& a6)
    {
        void* mem = allocator.alloc(sizeof(T), be_alignof(T));
        return ref<T>(new(mem) T(a1, a2, a3, a4, a5, a6), allocator);
    }
};

}
#include   <minitl/ptr/refptr.inl>

/*****************************************************************************/
#endif
