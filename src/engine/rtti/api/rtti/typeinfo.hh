/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/
#include    <rtti/engine/classinfo.script.hh>

namespace BugEngine
{

template< typename T >
struct be_typeid
{
    static const RTTI::ClassInfo* const klass;
    inline static RTTI::ClassInfo type()
    {
        return *klass;
    }
};

}

#include <rtti/typeinfo.inl>

/*****************************************************************************/
#endif
