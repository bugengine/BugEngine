/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/namespace.script.hh>
#include    <rtti/engine/helper/get.hh>
#include    <rtti/engine/helper/set.hh>
#include    <rtti/engine/helper/method.hh>

namespace BugEngine
{

template< > ref<RTTI::ClassInfo> be_typeid< void >::klassBuilder()
{
    static ref<RTTI::ClassInfo> klass = ref<RTTI::ClassInfo>::create(rttiArena(), inamespace("void"), ref<RTTI::ClassInfo>(), 0, 0);
    klass->copyconstructor = &RTTI::nullconstructor<0>;
    klass->destructor = &RTTI::nulldestructor;
    return klass;
}

template< > ref<RTTI::ClassInfo> be_typeid< ::minitl::refcountable >::klassBuilder()
{
    return be_typeid< void >::klass();
}

#define BE_MAKE_BUILTIN(type,parent)                                                            \
template< > ref<RTTI::ClassInfo> be_typeid< type >::klassBuilder()                              \
{                                                                                               \
    static ref<RTTI::ClassInfo> klass = ref<RTTI::ClassInfo>::create(rttiArena(),               \
                                                                     inamespace(#type),         \
                                                                     ref<RTTI::ClassInfo>(),    \
                                                                     (u32)sizeof(type),         \
                                                                     0);                        \
    klass->copyconstructor = &RTTI::nullconstructor<sizeof(type)>;                              \
    klass->destructor = &RTTI::nulldestructor;                                                  \
    weak<RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();                                     \
    ns->add(inamespace(#type), Value(minitl::ref< const ::BugEngine::RTTI::ClassInfo >(klass)));\
    return klass;                                                                               \
}

BE_MAKE_BUILTIN(bool, void);
BE_MAKE_BUILTIN(u8, void);
BE_MAKE_BUILTIN(u16, void);
BE_MAKE_BUILTIN(u32, void);
BE_MAKE_BUILTIN(u64, void);
BE_MAKE_BUILTIN(i8, void);
BE_MAKE_BUILTIN(i16, void);
BE_MAKE_BUILTIN(i32, void);
BE_MAKE_BUILTIN(i64, void);
BE_MAKE_BUILTIN(istring, void);
BE_MAKE_BUILTIN(inamespace, void);
BE_MAKE_BUILTIN(ifilename, void);
BE_MAKE_BUILTIN(minitl::format<>, void);
BE_MAKE_BUILTIN(Value, void);

#undef BE_MAKE_BUILTIN

}
