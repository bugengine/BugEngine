/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_REFCOUNTABLE_HH_
#define BE_MINITL_REFCOUNTABLE_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>
#include <bugengine/minitl/pointer.hh>

namespace minitl {

class refcountable;

class refcountable : public pointer
{
    template < typename T >
    friend class ref;
    template < typename T >
    friend class scoped;

private: /* friend ref */
    mutable i_u32 m_refCount;

public:
    refcountable() : m_refCount(i_u32::create(0))
    {
    }
    inline virtual ~refcountable()
    {
        be_assert(m_refCount == 0, "object is destroyed but has %d references" | m_refCount);
    }

private:
    void  operator&() const;
    void* operator new(size_t size)
    {
        return ::operator new(size);
    }
    void* operator new(size_t size, void* where)
    {
        return ::operator new(size, where);
    }

protected:
    void operator delete(void* memory)
    {
        return ::operator delete(memory);
    }
    void operator delete(void* memory, void* where)
    {
        ::operator delete(memory, where);
    }

public:
    inline void addref() const
    {
        ++m_refCount;
    }
    inline void decref() const
    {
        be_assert(m_refCount > 0, "object has no reference; cannot dereference it again");
        if(!--m_refCount)
        {
            checked_delete();
        }
    }
};

}  // namespace minitl

/**************************************************************************************************/
#endif
