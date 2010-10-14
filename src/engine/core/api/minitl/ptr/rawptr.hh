/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_RAWPTR_HH_
#define BE_MINITL_PTR_RAWPTR_HH_
/*****************************************************************************/

namespace minitl
{

template< typename T >
class raw
{
private:
    union Tstruct
    {
        T t;
    };
public:
    T*  m_ptr;
public:
    inline T* operator->() const;
    inline operator const void*() const;
    inline bool operator!() const;
    inline T& operator*();
};

}

#include   <minitl/ptr/rawptr.inl>

/*****************************************************************************/
#endif
