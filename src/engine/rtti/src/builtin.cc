/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.hh>

namespace BugEngine
{

#define BE_MAKE_BUILTIN_TYPE(type)  \
    template< > const char *be_typeid<type>::name = #type;                                          \
    template< > const RTTI::ClassInfo be_typeid<type>::klass = { name, 0, sizeof(type), 0, 0, 0, 0, 0, 0, 0, 0 };

BE_MAKE_BUILTIN_TYPE(u8);
BE_MAKE_BUILTIN_TYPE(u16);
BE_MAKE_BUILTIN_TYPE(u32);
BE_MAKE_BUILTIN_TYPE(u64);
BE_MAKE_BUILTIN_TYPE(i8);
BE_MAKE_BUILTIN_TYPE(i16);
BE_MAKE_BUILTIN_TYPE(i32);
BE_MAKE_BUILTIN_TYPE(i64);

}
