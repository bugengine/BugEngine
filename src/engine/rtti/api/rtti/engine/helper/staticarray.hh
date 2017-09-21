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
    T* begin()              { return elements; }
    const T* begin() const  { return elements; }
    T* end()                { return begin() + count; }
    const T* end() const    { return begin() + count; }
};

}}

/**************************************************************************************************/
#endif
