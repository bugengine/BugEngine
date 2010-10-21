/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/
#include    <rtti/typeinfo.script.hh>

namespace BugEngine
{

template< typename T >
struct be_typeid
{
    static const RTTI::ClassInfo* const klass;
    static const TypeInfo type()
    {
        TypeInfo i = { klass, TypeInfo::Class, TypeInfo::Mutable };
        return i;
    }
};

}

#include <rtti/typeinfo.inl>

/*****************************************************************************/
#endif
