/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_STATICARRAY_HH_
#define BE_RTTI_ENGINE_HELPER_STATICARRAY_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>

namespace BugEngine { namespace RTTI
{

template< typename T >
struct staticarray
{
    u64 const   count;

    T& operator[](const u32 index)
    {
        be_assert(index < count, "index %d out of range (0, %d)" | index | (count-1));
        return *(begin()+index);
    }
    const T& operator[](const u32 index) const
    {
        be_assert(index < count, "index %d out of range (0, %d)" | index | (count-1));
        return *(begin()+index);
    }
    T* begin()              { return reinterpret_cast<T*>(reinterpret_cast<char*>(this) + sizeof(u64)); }
    const T* begin() const  { return reinterpret_cast<const T*>(reinterpret_cast<const char*>(this) + sizeof(u64)); }
    T* end()                { return begin() + count; }
    const T* end() const    { return begin() + count; }

    static staticarray<T> s_null;
};


template< u32 COUNT, typename T >
struct staticarray_n
{
    staticarray<T>  array;
    T               elements[COUNT];
};

template< typename T >
struct staticarray_n<0, T>
{
    staticarray<T>  array;
};

template< typename T >
staticarray<T> staticarray<T>::s_null = {0};

}}

/**************************************************************************************************/
#endif
