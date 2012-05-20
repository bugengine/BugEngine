/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_ARRAY_HH_
#define BE_MINITL_CONTAINER_ARRAY_HH_
/*****************************************************************************/
#include    <minitl/container/iterator.hh>
#include    <core/memory/malloc.hh>

namespace minitl
{

template< typename T >
class array
{
private:
    T*                      m_array;
    BugEngine::Allocator&   allocator;
    u32                     m_size;
public:
    inline array(BugEngine::Allocator& allocator, u32 size);
    inline ~array();

    inline T* begin();
    inline T* end();
    inline const T* begin() const;
    inline const T* end() const;

    inline T& operator[](u32 index);
    inline const T& operator[](u32 index) const;
};

}

#include <minitl/container/inl/array.inl>

/*****************************************************************************/
#endif
