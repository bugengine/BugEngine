/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_RAWPTR_HH_
#define BE_MINITL_RAWPTR_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>

namespace minitl {

template < typename T >
class raw
{
private:
    union Tstruct
    {
        T t;
    };

public:
    T* m_ptr;

public:
    inline T*       operator->() const;
    inline          operator const void*() const;
    inline bool     operator!() const;
    inline T&       operator*();
    inline const T& operator*() const;

    template < typename U >
    operator raw< U >() const
    {
        raw< U > result = {m_ptr};
        return result;
    }
    T* set(T* value)
    {
        return m_ptr = value;
    }
    static inline raw< T > null();
};

}  // namespace minitl

#include <bugengine/minitl/inl/rawptr.inl>

/**************************************************************************************************/
#endif
