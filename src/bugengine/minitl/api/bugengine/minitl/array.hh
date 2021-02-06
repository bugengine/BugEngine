/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_ARRAY_HH_
#define BE_MINITL_ARRAY_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>
#include <bugengine/kernel/interlocked.hh>
#include <bugengine/minitl/allocator.hh>
#include <bugengine/minitl/iterator.hh>

namespace minitl {

template < typename T >
class array
{
private:
    mutable minitl::Allocator::Block< T > m_array;

private:
    array& operator=(const array& other);

public:
    typedef const T* const_iterator;
    typedef T*       iterator;

public:
    inline array(Allocator& allocator, u32 size);
    template < typename ITERATOR >
    inline array(Allocator& allocator, ITERATOR begin, ITERATOR end);
    inline array(const array& other);
    inline ~array();

    inline void swap(array& other);

    inline iterator       begin();
    inline iterator       end();
    inline const_iterator begin() const;
    inline const_iterator end() const;

    inline T&       operator[](u32 index);
    inline const T& operator[](u32 index) const;

    inline u32 size() const;

    T&       first();
    const T& first() const;
    T&       last();
    const T& last() const;
};

}  // namespace minitl

#include <bugengine/minitl/inl/array.inl>

/**************************************************************************************************/
#endif
