/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_ARRAY_HH_
#define BE_MINITL_ARRAY_HH_
/*****************************************************************************/
#include    <minitl/iterator.hh>
#include    <kernel/interlocked.hh>
#include    <minitl/allocator.hh>

namespace minitl
{

template< typename T >
class array
{
private:
    mutable minitl::Allocator::Block<T> m_array;
private:
    array& operator=(const array& other);
public:
    typedef const T* const_iterator;
    typedef T* iterator;
public:
    inline array(Allocator& allocator, u32 size);
    template< typename ITERATOR >
    inline array(Allocator& allocator, ITERATOR begin, ITERATOR end);
    inline array(const array& other);
    inline ~array();

    inline iterator begin();
    inline iterator end();
    inline const_iterator begin() const;
    inline const_iterator end() const;

    inline T& operator[](u32 index);
    inline const T& operator[](u32 index) const;

    inline u32 size() const;
};

}

#include <minitl/inl/array.inl>

/*****************************************************************************/
#endif
