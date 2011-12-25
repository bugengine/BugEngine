/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/helper/get.hh>
#include    <rtti/engine/helper/set.hh>
#include    <rtti/engine/helper/method.hh>


BE_REGISTER_NAMESPACE_2(game, BugEngine, RTTI);
BE_REGISTER_NAMESPACE_2(game, BugEngine, EditHint);
BE_REGISTER_NAMESPACE_2(game, BugEngine, Documentation);

namespace BugEngine
{

template< >
raw<const RTTI::ClassInfo> be_typeid< void >::klass()
{
    static const RTTI::ClassInfo s_void = { "void", {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, &RTTI::nullconstructor<0>, &RTTI::nulldestructor,  {{ 0, 0, 0, 0 }} };
    raw<const RTTI::ClassInfo> ci = {&s_void};
    return ci;
}

template< >
raw<const RTTI::ClassInfo> be_typeid< minitl::refcountable >::klass()
{
    static const RTTI::ClassInfo s_refcountable = { "refcountable", {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, &RTTI::nullconstructor<0>, &RTTI::nulldestructor, {{ 0, 0, 0, 0 }} };
    raw<const RTTI::ClassInfo> ci = {&s_refcountable};
    return ci;
}

#define BE_MAKE_BUILTIN_NAME(type,name,parent)                          \
    static const RTTI::ClassInfo s_##name =                             \
        {                                                               \
            #name,                                                      \
            be_typeid< parent >::klass(),                               \
            sizeof(#type),                                              \
            0,                                                          \
            {0}, {0}, {0}, {0}, {0}, {0},                               \
            &RTTI::nullconstructor<sizeof(#type)>,                      \
            &RTTI::nulldestructor,                                      \
            {{ 0, 0, 0, 0 }}                                            \
        };                                                              \
    template< > raw<const RTTI::ClassInfo> be_typeid< type >::klass()   \
    {                                                                   \
        raw<const RTTI::ClassInfo> ci = {&s_##name};                    \
        return ci;                                                      \
    }
#define BE_MAKE_BUILTIN(type,parent)                                    \
    BE_MAKE_BUILTIN_NAME(type,type,parent)

BE_MAKE_BUILTIN(bool, void);
BE_MAKE_BUILTIN(u8, void);
BE_MAKE_BUILTIN(u16, void);
BE_MAKE_BUILTIN(u32, void);
BE_MAKE_BUILTIN(u64, void);
BE_MAKE_BUILTIN(i8, void);
BE_MAKE_BUILTIN(i16, void);
BE_MAKE_BUILTIN(i32, void);
BE_MAKE_BUILTIN(i64, void);
BE_MAKE_BUILTIN(float, void);
BE_MAKE_BUILTIN(double, void);

BE_MAKE_BUILTIN(byte2, void);
BE_MAKE_BUILTIN(byte3, void);
BE_MAKE_BUILTIN(byte4, void);

BE_MAKE_BUILTIN(short2, void);
BE_MAKE_BUILTIN(short3, void);
BE_MAKE_BUILTIN(short4, void);

BE_MAKE_BUILTIN(int2, void);
BE_MAKE_BUILTIN(int3, void);
BE_MAKE_BUILTIN(int4, void);

BE_MAKE_BUILTIN(uint2, void);
BE_MAKE_BUILTIN(uint3, void);
BE_MAKE_BUILTIN(uint4, void);

BE_MAKE_BUILTIN(bigint2, void);
BE_MAKE_BUILTIN(bigint3, void);
BE_MAKE_BUILTIN(bigint4, void);

BE_MAKE_BUILTIN(float2, void);
BE_MAKE_BUILTIN(float3, void);
BE_MAKE_BUILTIN(float4, void);

BE_MAKE_BUILTIN(istring, void);
BE_MAKE_BUILTIN(inamespace, void);
BE_MAKE_BUILTIN(ipath, void);
BE_MAKE_BUILTIN(ifilename, void);
BE_MAKE_BUILTIN_NAME(minitl::format<>, format, void);

BE_MAKE_BUILTIN(Value, void);

#undef BE_MAKE_BUILTIN

}
