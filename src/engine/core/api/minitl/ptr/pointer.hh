/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_POINTER_HH_
#define BE_MINITL_PTR_POINTER_HH_
/*****************************************************************************/

namespace minitl
{

template< typename T > class ref;
template< typename T > class weak;
template< typename T > class scoped;
typedef void (Deleter)(const void* ptr);

template< typename T >
static void checked_destroy(const T*);
template< typename T >
static void checked_delete(const T*);

class pointer
{
                            friend inline void addweak(const pointer* ptr);
                            friend inline void decweak(const pointer* ptr);
    template< typename T >  friend class ref;
    template< typename T >  friend class scoped;
    template< typename T >  friend void checked_delete(const T*);
private:
    Deleter*    m_deleter;
#ifdef BE_ENABLE_WEAKCHECK
    mutable i_u32 m_weakCount;
#endif
public:
    pointer()
    :   m_deleter(0)
#ifdef BE_ENABLE_WEAKCHECK
    ,   m_weakCount(0)
#endif
    {}
    inline virtual ~pointer()
    {
#ifdef BE_ENABLE_WEAKCHECK
        be_assert(m_weakCount == 0, "object is destroyed but has %d weak references" | m_weakCount);
#endif
    }
private:
    void  operator&() const;
    void* operator new(size_t size)                  { return ::operator new(size); }
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
protected:
    void  operator delete(void* memory)              { return ::operator delete(memory); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
};


inline void addweak(const pointer* ptr)
{
#ifdef BE_ENABLE_WEAKCHECK
    if(ptr)
        ++ptr->m_weakCount;
#endif
}

inline void decweak(const pointer* ptr)
{
#ifdef BE_ENABLE_WEAKCHECK
    if(!ptr)
        return;
    be_assert(ptr->m_weakCount, "object has no weak reference; cannot dereference it again");
    --ptr->m_weakCount;
#endif
}

}

/*****************************************************************************/
#endif
