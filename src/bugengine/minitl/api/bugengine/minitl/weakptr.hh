/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_WEAKPTR_HH_
#define BE_MINITL_WEAKPTR_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>
#include <bugengine/minitl/hash.hh>
#include <bugengine/minitl/refcountable.hh>
#include <bugengine/minitl/refptr.hh>
#include <bugengine/minitl/scopedptr.hh>

namespace minitl {

template < typename T >
class weak
{
    template < typename U, typename V >
    friend weak< U > be_checked_cast(weak< V > v);
    template < typename U, typename V >
    friend weak< U > be_const_cast(weak< V > v);

private:
    template < typename OBJECT >
    struct ptr
    {
        typedef minitl::pointer* pointer;
    };
    template < typename OBJECT >
    struct ptr< const OBJECT >
    {
        typedef const minitl::pointer* pointer;
    };
    typedef typename ptr< T >::pointer pointer;

private:
    pointer m_ptr;

private:
    inline void swap(weak& other);

public:
    inline weak();
    inline weak(T* p);
    template < typename U >
    inline weak(ref< U > other);
    template < typename U >
    inline weak(const scoped< U >& other);
    inline weak(const weak& other);
    template < typename U >
    inline weak(const weak< U >& other);
    inline ~weak();

    inline weak& operator=(const weak& other);
    template < typename U >
    inline weak& operator=(const weak< U >& other);
    template < typename U >
    inline weak& operator=(U* other);

    inline T*   operator->() const;
    inline      operator const void*() const;
    inline bool operator!() const;
    inline T&   operator*();

    inline void clear();
};

template < typename T >
struct hash< weak< T > >
{
    u32 operator()(weak< T > t)
    {
        return hash< T* >()(t.operator->());
    }
    bool operator()(weak< T > t1, weak< T > t2)
    {
        return hash< T* >()(t1.operator->(), t2.operator->());
    }
};

template < u16 SIZE >
class format;
template < typename T, u16 SIZE >
const format< SIZE >& operator|(const format< SIZE >& format, weak< T > t)
{
    return format | t.operator->();
}

}  // namespace minitl

#include <bugengine/minitl/inl/weakptr.inl>

/**************************************************************************************************/
#endif
