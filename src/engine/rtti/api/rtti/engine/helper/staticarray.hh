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
    u32 const   count;
    T           elements[1];

    T& operator[](const u32 index)
    {
        be_assert(index < count, "index %d out of range (0, %d)" | index | (count-1));
        return elements[index];
    }
    const T& operator[](const u32 index) const
    {
        be_assert(index < count, "index %d out of range (0, %d)" | index | (count-1));
        return elements[index];
    }
    T* begin()              { return &elements[0]; }
    const T* begin() const  { return &elements[0]; }
    T* end()                { return &elements[count]; }
    const T* end() const    { return &elements[count]; }

    static staticarray<T>* s_null;
};


template< u32 COUNT, typename T >
struct staticarray_n
{
    u32 const   count;
    T           elements[COUNT];
};

template< typename T >
struct staticarray_n<0, T>
{
    u32 const   count;
    static staticarray_n<0, T> s_null;
};

template< typename T >
staticarray<T>* staticarray<T>::s_null = reinterpret_cast< staticarray<T>* >(&staticarray_n<0, T>::s_null);
template< typename T >
staticarray_n<0, T>  staticarray_n<0, T>::s_null = {0};

}}

/**************************************************************************************************/
#endif
