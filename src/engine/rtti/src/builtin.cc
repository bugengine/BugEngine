/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.hh>

namespace BugEngine
{

namespace Builtin
{
    static const char *const s_voidName = "void";
    static const RTTI::ClassInfo s_voidClass =  { s_voidName, &s_voidClass, 0, 0 };
};
template< > const RTTI::ClassInfo* const be_typeid<void>::klass = &Builtin::s_voidClass;

#define BE_MAKE_BUILTIN_TYPE(type)  \
namespace Builtin                                       \
{                                                       \
    static const char *const s_##type##Name = #type;    \
    static const RTTI::ClassInfo s_##type##Class =      \
        { s_##type##Name, &s_voidClass, 0,              \
            sizeof(type) };                             \
};                                                      \
    template< > const RTTI::ClassInfo* const be_typeid<type>::klass = &Builtin::s_##type##Class;

BE_MAKE_BUILTIN_TYPE(u8);
BE_MAKE_BUILTIN_TYPE(u16);
BE_MAKE_BUILTIN_TYPE(u32);
BE_MAKE_BUILTIN_TYPE(u64);
BE_MAKE_BUILTIN_TYPE(i8);
BE_MAKE_BUILTIN_TYPE(i16);
BE_MAKE_BUILTIN_TYPE(i32);
BE_MAKE_BUILTIN_TYPE(i64);

}
