/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/engine/classinfo.script.hh>

namespace BugEngine
{

namespace Builtin
{
    static const char *const s_voidName = "void";
    static const RTTI::ClassInfo s_voidClass =  { s_voidName, &s_voidClass, 0, 0 };
};
template< > const RTTI::ClassInfo* const be_typeid<void>::type = &Builtin::s_voidClass;

#define BE_MAKE_BUILTIN_TYPE(type_)                     \
namespace Builtin                                       \
{                                                       \
    static const char *const s_##type_##Name = #type_;  \
    static const RTTI::ClassInfo s_##type_##Class =     \
        { s_##type_##Name, &s_voidClass, 0,             \
            sizeof(type_) };                            \
};                                                      \
    template< > const RTTI::ClassInfo* const be_typeid<type_>::type = &Builtin::s_##type_##Class;

BE_MAKE_BUILTIN_TYPE(u8);
BE_MAKE_BUILTIN_TYPE(u16);
BE_MAKE_BUILTIN_TYPE(u32);
BE_MAKE_BUILTIN_TYPE(u64);
BE_MAKE_BUILTIN_TYPE(i8);
BE_MAKE_BUILTIN_TYPE(i16);
BE_MAKE_BUILTIN_TYPE(i32);
BE_MAKE_BUILTIN_TYPE(i64);

}
