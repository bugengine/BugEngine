/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_CARRAY_FACTORY_HH_
#define BE_META_ENGINE_CARRAY_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/helper/method.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/engine/scriptingapi.hh>
#include <bugengine/meta/typeinfo.hh>

namespace BugEngine { namespace Meta {

template < typename T, u32 Count >
struct ClassID< T[Count] >
{
    static BE_EXPORT raw< const Meta::Class > klass();
};

template < typename T, u32 Count >
struct ClassID< const T[Count] >
{
    static BE_EXPORT raw< const Meta::Class > klass();
};

template < typename T, u32 Count >
struct carray_BugHelper
{
    typedef T ArrayType[Count];

    static const Meta::Type value_type;
    static u32              array_size(const Meta::Value& v);
    static Meta::Value      index(Meta::Value& v, u32 i);
    static Meta::Value      indexConst(const Meta::Value& v, u32 i);

    static Meta::Value                   trampoline_method_size_overload_0(Meta::Value* parameters,
                                                                           u32          parameterCount);
    static const Meta::Method::Parameter s_method_size_overload_0_params[];
    static const Meta::Method::Overload  s_method_size_overloads[];
    static Meta::Value                   trampoline_method_Index_overload_0(Meta::Value* parameters,
                                                                            u32          parameterCount);
    static const Meta::Method::Parameter s_method_Index_overload_0_params[];
    static Meta::Value                   trampoline_method_Index_overload_1(Meta::Value* parameters,
                                                                            u32          parameterCount);
    static const Meta::Method::Parameter s_method_Index_overload_1_params[];
    static const Meta::Method::Overload  s_method_Index_overloads[];
    static const Meta::ObjectInfo        s_prop_value_type_object_value_type;
    static const Meta::ScriptingArrayAPI scriptingArrayAPI;
    static const Meta::ScriptingAPI      scriptingAPI;
};

template < typename T, u32 Count >
const Meta::Type carray_BugHelper< T, Count >::value_type = be_type< T >();

template < typename T, u32 Count >
u32 carray_BugHelper< T, Count >::array_size(const Meta::Value& v)
{
    be_forceuse(v);
    return Count;
}

template < typename T, u32 Count >
Meta::Value carray_BugHelper< T, Count >::index(Meta::Value& v, u32 i)
{
    return Meta::Value(Meta::Value::ByRef(v.as< ArrayType& >()[i]));
}

template < typename T, u32 Count >
Meta::Value carray_BugHelper< T, Count >::indexConst(const Meta::Value& v, u32 i)
{
    return Meta::Value(Meta::Value::ByRef(v.as< const ArrayType& >()[i]));
}

template < typename T, u32 Count >
Meta::Value carray_BugHelper< T, Count >::trampoline_method_size_overload_0(Meta::Value* parameters,
                                                                            u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return Meta::Value(Count);
}

template < typename T, u32 Count >
Meta::Value
carray_BugHelper< T, Count >::trampoline_method_Index_overload_0(Meta::Value* parameters,
                                                                 u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    const ArrayType& array = parameters[0].as< const ArrayType& >();
    u32              index = parameters[1].as< u32 >();
    return Meta::Value(Meta::Value::ByRef(array[index]));
}

template < typename T, u32 Count >
Meta::Value
carray_BugHelper< T, Count >::trampoline_method_Index_overload_1(Meta::Value* parameters,
                                                                 u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    ArrayType& array = parameters[0].as< ArrayType& >();
    u32        index = parameters[1].as< u32 >();
    return Meta::Value(Meta::Value::ByRef(array[index]));
}

template < typename T, u32 Count >
const Meta::ObjectInfo carray_BugHelper< T, Count >::s_prop_value_type_object_value_type
    = {{0}, {0}, istring(istring("value_type")), Meta::Value(value_type)};

template < typename T, u32 Count >
const Meta::Method::Parameter carray_BugHelper< T, Count >::s_method_size_overload_0_params[]
    = {{{0},
        istring("this"),
        be_type< const ArrayType& >(),
        {&Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T, u32 Count >
const Meta::Method::Overload carray_BugHelper< T, Count >::s_method_size_overloads[]
    = {{{0},
        {1, s_method_size_overload_0_params},
        be_type< u32 >(),
        false,
        {0, 0},
        &trampoline_method_size_overload_0}};

template < typename T, u32 Count >
const Meta::Method::Parameter carray_BugHelper< T, Count >::s_method_Index_overload_0_params[]
    = {{{0},
        istring("this"),
        be_type< const ArrayType& >(),
        {&Meta::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("index"), be_type< u32 >(), {&Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T, u32 Count >
const Meta::Method::Parameter carray_BugHelper< T, Count >::s_method_Index_overload_1_params[] = {
    {{0}, istring("this"), be_type< ArrayType& >(), {&Meta::Method::Parameter::s_noDefaultValue}},
    {{0}, istring("index"), be_type< u32 >(), {&Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T, u32 Count >
const Meta::Method::Overload carray_BugHelper< T, Count >::s_method_Index_overloads[]
    = {{{0},
        {2, s_method_Index_overload_0_params},
        be_type< const T& >(),
        false,
        {0, 0},
        &trampoline_method_Index_overload_0},
       {{0},
        {2, s_method_Index_overload_1_params},
        be_type< T& >(),
        false,
        {0, 0},
        &trampoline_method_Index_overload_1}};

template < typename T, u32 Count >
const Meta::ScriptingArrayAPI carray_BugHelper< T, Count >::scriptingArrayAPI
    = {be_type< T >(), &array_size, &index, &indexConst};

template < typename T, u32 Count >
const Meta::ScriptingAPI carray_BugHelper< T, Count >::scriptingAPI = {{&scriptingArrayAPI}};

template < typename T, u32 Count >
BE_EXPORT raw< const Meta::Class > ClassID< T[Count] >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const Meta::Method s_methods[2]
        = {{istring("size"),
            {1, carray_BugHelper< T, Count >::s_method_size_overloads},
            {&s_methods[1]}},
           {istring("Index"),
            {2, carray_BugHelper< T, Count >::s_method_Index_overloads},
            {&s_methods[2]}}};
    static const Meta::Class s_class = {"array",
                                        u32(sizeof(T[Count])),
                                        0,
                                        Meta::ClassType_Array,
                                        {0},
                                        {be_class< void >().m_ptr},
                                        {0},
                                        {0},
                                        {0, 0},
                                        {2, s_methods},
                                        {0},
                                        {&carray_BugHelper< T, Count >::scriptingAPI},
                                        &Meta::wrap< T[Count] >::copy,
                                        &Meta::wrap< T[Count] >::destroy};
    raw< const Meta::Class > result  = {&s_class};
    return result;
}

template < typename T, u32 Count >
BE_EXPORT raw< const Meta::Class > ClassID< const T[Count] >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const Meta::Method s_methods[2]
        = {{istring("size"),
            {1, carray_BugHelper< const T, Count >::s_method_size_overloads},
            {&s_methods[1]}},
           {istring("Index"),
            {2, carray_BugHelper< const T, Count >::s_method_Index_overloads},
            {&s_methods[2]}}};
    static const Meta::Class s_class = {"array",
                                        u32(sizeof(const T[Count])),
                                        0,
                                        Meta::ClassType_Array,
                                        {0},
                                        {be_class< void >().m_ptr},
                                        {0},
                                        {0},
                                        {0, 0},
                                        {2, s_methods},
                                        {0},
                                        {&carray_BugHelper< const T, Count >::scriptingAPI},
                                        &Meta::wrap< T[Count] >::copy,
                                        &Meta::wrap< const T[Count] >::destroy};
    raw< const Meta::Class > result  = {&s_class};
    return result;
}

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
