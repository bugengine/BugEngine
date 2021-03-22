/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_ARRAY_FACTORY_HH_
#define BE_META_ENGINE_ARRAY_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/helper/method.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/engine/scriptingapi.hh>
#include <bugengine/meta/typeinfo.hh>
#include <bugengine/minitl/array.hh>

namespace BugEngine { namespace Meta {

template < typename T >
struct ClassID< minitl::array< T > >
{
    static const Meta::Type value_type;
    static u32              array_size(const Meta::Value& v);
    static Meta::Value      index(Meta::Value& v, u32 i);
    static Meta::Value      indexConst(const Meta::Value& v, u32 i);

    static Meta::Value                   trampoline_method_array_overload_0(Meta::Value* parameters,
                                                                            u32          parameterCount);
    static const Meta::Method::Parameter s_method_array_overload_0_params[];
    static const Meta::Method::Overload  s_method_array_overloads[];
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

    static BE_EXPORT raw< const Meta::Class > klass();
};

template < typename T >
const Meta::Type ClassID< minitl::array< T > >::value_type = be_type< T >();

template < typename T >
u32 ClassID< minitl::array< T > >::array_size(const Meta::Value& v)
{
    return v.as< const minitl::array< T >& >().size();
}

template < typename T >
Meta::Value ClassID< minitl::array< T > >::index(Meta::Value& v, u32 i)
{
    return Meta::Value(Meta::Value::ByRef(v.as< minitl::array< T >& >().operator[](i)));
}

template < typename T >
Meta::Value ClassID< minitl::array< T > >::indexConst(const Meta::Value& v, u32 i)
{
    return Meta::Value(Meta::Value::ByRef(v.as< const minitl::array< T >& >().operator[](i)));
}

template < typename T >
Meta::Value
ClassID< minitl::array< T > >::trampoline_method_size_overload_0(Meta::Value* parameters,
                                                                 u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return Meta::Value(parameters[0].as< const minitl::array< T >& >().size());
}

template < typename T >
Meta::Value
ClassID< minitl::array< T > >::trampoline_method_Index_overload_0(Meta::Value* parameters,
                                                                  u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return Meta::Value(Meta::Value::ByRef(
        parameters[0].as< const minitl::array< T >& >().operator[](parameters[1].as< u32 >())));
}

template < typename T >
Meta::Value
ClassID< minitl::array< T > >::trampoline_method_Index_overload_1(Meta::Value* parameters,
                                                                  u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return Meta::Value(Meta::Value::ByRef(
        parameters[0].as< minitl::array< T >& >().operator[](parameters[1].as< u32 >())));
}

template < typename T >
Meta::Value
ClassID< minitl::array< T > >::trampoline_method_array_overload_0(Meta::Value* parameters,
                                                                  u32          parameterCount)
{
    T* t = (T*)malloca(sizeof(T) * parameterCount);
    for(u32 i = 0; i < parameterCount; ++i)
        new((void*)&t[i]) T(parameters[i].as< T >());
    minitl::array< T > result(Arena::script(), t, t + parameterCount);
    for(u32 i = 0; i < parameterCount; ++i)
        result[parameterCount - i - 1].~T();
    freea(t);
    return Meta::Value(result);
}

template < typename T >
const Meta::ObjectInfo ClassID< minitl::array< T > >::s_prop_value_type_object_value_type
    = {{0}, {0}, istring(istring("value_type")), Meta::Value(value_type)};

template < typename T >
const Meta::Method::Overload ClassID< minitl::array< T > >::s_method_array_overloads[]
    = {{{0},
        {0, 0},
        be_type< minitl::array< T > >(),
        true,
        {0, 0},
        &trampoline_method_array_overload_0}};

template < typename T >
const Meta::Method::Parameter ClassID< minitl::array< T > >::s_method_size_overload_0_params[]
    = {{{0},
        istring("this"),
        be_type< const minitl::array< T >& >(),
        {&Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const Meta::Method::Overload ClassID< minitl::array< T > >::s_method_size_overloads[]
    = {{{0},
        {1, s_method_size_overload_0_params},
        be_type< u32 >(),
        false,
        {0, 0},
        &trampoline_method_size_overload_0}};

template < typename T >
const Meta::Method::Parameter ClassID< minitl::array< T > >::s_method_Index_overload_0_params[]
    = {{{0},
        istring("this"),
        be_type< const minitl::array< T >& >(),
        {&Meta::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("index"), be_type< u32 >(), {&Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const Meta::Method::Parameter ClassID< minitl::array< T > >::s_method_Index_overload_1_params[]
    = {{{0},
        istring("this"),
        be_type< minitl::array< T >& >(),
        {&Meta::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("index"), be_type< u32 >(), {&Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const Meta::Method::Overload ClassID< minitl::array< T > >::s_method_Index_overloads[]
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

template < typename T >
const Meta::ScriptingArrayAPI ClassID< minitl::array< T > >::scriptingArrayAPI
    = {be_type< T >(), &array_size, &index, &indexConst};

template < typename T >
const Meta::ScriptingAPI ClassID< minitl::array< T > >::scriptingAPI = {{&scriptingArrayAPI}};

template < typename T >
BE_EXPORT raw< const Meta::Class > ClassID< minitl::array< T > >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const Meta::Method s_methods[3]
        = {{istring("array"),
            {1, ClassID< minitl::array< T > >::s_method_array_overloads},
            {&s_methods[0]}},
           {istring("size"),
            {1, ClassID< minitl::array< T > >::s_method_size_overloads},
            {&s_methods[1]}},
           {istring("Index"),
            {2, ClassID< minitl::array< T > >::s_method_Index_overloads},
            {&s_methods[2]}}};
    static const Meta::Class s_class = {"array",
                                        u32(sizeof(minitl::array< T >)),
                                        0,
                                        Meta::ClassType_Array,
                                        {0},
                                        {be_class< void >().m_ptr},
                                        {0},
                                        {0},
                                        {0, 0},
                                        {3, s_methods},
                                        {s_methods},
                                        {&ClassID< minitl::array< T > >::scriptingAPI},
                                        &Meta::wrap< minitl::array< T > >::copy,
                                        &Meta::wrap< minitl::array< T > >::destroy};
    raw< const Meta::Class > result  = {&s_class};
    return result;
}

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
