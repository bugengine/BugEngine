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
    T* const    elements;

    inline T& operator[](const u32 index);
    inline const T& operator[](const u32 index) const;

    inline T* begin();
    inline const T* begin() const;
    inline T* end();
    inline const T* end() const;
};

template< typename T >
T& staticarray<T>::operator[](const u32 index)
{
    be_assert(index < count, "index %d out of range (0, %d)" | index | (count-1));
    return *(begin()+index);
}

template< typename T >
const T& staticarray<T>::operator[](const u32 index) const
{
    be_assert(index < count, "index %d out of range (0, %d)" | index | (count-1));
    return *(begin()+index);
}

template< typename T >
T* staticarray<T>::begin()
{
    return elements;
}

template< typename T >
const T* staticarray<T>::begin() const
{
    return elements;
}

template< typename T >
T* staticarray<T>::end()
{
    return begin() + count;
}

template< typename T >
const T* staticarray<T>::end() const
{
    return begin() + count;
}

}}

/**************************************************************************************************/
#endif
