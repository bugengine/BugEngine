/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_ARRAY_FACTORY_HH_
#define BE_RTTI_ENGINE_ARRAY_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/minitl/array.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/helper/method.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/engine/scriptingapi.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine { namespace RTTI {

template < typename T >
struct ClassID< minitl::array< T > >
{
    static const RTTI::Type value_type;
    static u32              array_size(const RTTI::Value& v);
    static RTTI::Value      index(RTTI::Value& v, u32 i);
    static RTTI::Value      indexConst(const RTTI::Value& v, u32 i);

    static RTTI::Value                   trampoline_method_array_overload_0(RTTI::Value* parameters,
                                                                            u32          parameterCount);
    static const RTTI::Method::Parameter s_method_array_overload_0_params[];
    static const RTTI::Method::Overload  s_method_array_overloads[];
    static RTTI::Value                   trampoline_method_size_overload_0(RTTI::Value* parameters,
                                                                           u32          parameterCount);
    static const RTTI::Method::Parameter s_method_size_overload_0_params[];
    static const RTTI::Method::Overload  s_method_size_overloads[];
    static RTTI::Value                   trampoline_method_Index_overload_0(RTTI::Value* parameters,
                                                                            u32          parameterCount);
    static const RTTI::Method::Parameter s_method_Index_overload_0_params[];
    static RTTI::Value                   trampoline_method_Index_overload_1(RTTI::Value* parameters,
                                                                            u32          parameterCount);
    static const RTTI::Method::Parameter s_method_Index_overload_1_params[];
    static const RTTI::Method::Overload  s_method_Index_overloads[];
    static const RTTI::ObjectInfo        s_prop_value_type_object_value_type;
    static const RTTI::ScriptingArrayAPI scriptingArrayAPI;
    static const RTTI::ScriptingAPI      scriptingAPI;

    static BE_EXPORT raw< const RTTI::Class > klass();
};

template < typename T >
const RTTI::Type ClassID< minitl::array< T > >::value_type = be_type< T >();

template < typename T >
u32 ClassID< minitl::array< T > >::array_size(const RTTI::Value& v)
{
    return v.as< const minitl::array< T >& >().size();
}

template < typename T >
RTTI::Value ClassID< minitl::array< T > >::index(RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< minitl::array< T >& >().operator[](i)));
}

template < typename T >
RTTI::Value ClassID< minitl::array< T > >::indexConst(const RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< const minitl::array< T >& >().operator[](i)));
}

template < typename T >
RTTI::Value
ClassID< minitl::array< T > >::trampoline_method_size_overload_0(RTTI::Value* parameters,
                                                                 u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(parameters[0].as< const minitl::array< T >& >().size());
}

template < typename T >
RTTI::Value
ClassID< minitl::array< T > >::trampoline_method_Index_overload_0(RTTI::Value* parameters,
                                                                  u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(RTTI::Value::ByRef(
       parameters[0].as< const minitl::array< T >& >().operator[](parameters[1].as< u32 >())));
}

template < typename T >
RTTI::Value
ClassID< minitl::array< T > >::trampoline_method_Index_overload_1(RTTI::Value* parameters,
                                                                  u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(RTTI::Value::ByRef(
       parameters[0].as< minitl::array< T >& >().operator[](parameters[1].as< u32 >())));
}

template < typename T >
RTTI::Value
ClassID< minitl::array< T > >::trampoline_method_array_overload_0(RTTI::Value* parameters,
                                                                  u32          parameterCount)
{
    T* t = (T*)malloca(sizeof(T) * parameterCount);
    for(u32 i = 0; i < parameterCount; ++i)
        new((void*)&t[i]) T(parameters[i].as< T >());
    minitl::array< T > result(Arena::script(), t, t + parameterCount);
    for(u32 i = 0; i < parameterCount; ++i)
        result[parameterCount - i - 1].~T();
    freea(t);
    return RTTI::Value(result);
}

template < typename T >
const RTTI::ObjectInfo ClassID< minitl::array< T > >::s_prop_value_type_object_value_type
   = {{0}, {0}, istring(istring("value_type")), RTTI::Value(value_type)};

template < typename T >
const RTTI::Method::Overload ClassID< minitl::array< T > >::s_method_array_overloads[]
   = {{{0}, {0, 0}, be_type< minitl::array< T > >(), true, &trampoline_method_array_overload_0}};

template < typename T >
const RTTI::Method::Parameter ClassID< minitl::array< T > >::s_method_size_overload_0_params[]
   = {{{0},
       istring("this"),
       be_type< const minitl::array< T >& >(),
       {&RTTI::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const RTTI::Method::Overload ClassID< minitl::array< T > >::s_method_size_overloads[]
   = {{{0},
       {1, s_method_size_overload_0_params},
       be_type< u32 >(),
       false,
       &trampoline_method_size_overload_0}};

template < typename T >
const RTTI::Method::Parameter ClassID< minitl::array< T > >::s_method_Index_overload_0_params[]
   = {{{0},
       istring("this"),
       be_type< const minitl::array< T >& >(),
       {&RTTI::Method::Parameter::s_noDefaultValue}},
      {{0}, istring("index"), be_type< u32 >(), {&RTTI::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const RTTI::Method::Parameter ClassID< minitl::array< T > >::s_method_Index_overload_1_params[]
   = {{{0},
       istring("this"),
       be_type< minitl::array< T >& >(),
       {&RTTI::Method::Parameter::s_noDefaultValue}},
      {{0}, istring("index"), be_type< u32 >(), {&RTTI::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const RTTI::Method::Overload ClassID< minitl::array< T > >::s_method_Index_overloads[]
   = {{{0},
       {2, s_method_Index_overload_0_params},
       be_type< const T& >(),
       false,
       &trampoline_method_Index_overload_0},
      {{0},
       {2, s_method_Index_overload_1_params},
       be_type< T& >(),
       false,
       &trampoline_method_Index_overload_1}};

template < typename T >
const RTTI::ScriptingArrayAPI ClassID< minitl::array< T > >::scriptingArrayAPI
   = {be_type< T >(), &array_size, &index, &indexConst};

template < typename T >
const RTTI::ScriptingAPI ClassID< minitl::array< T > >::scriptingAPI = {{&scriptingArrayAPI}};

template < typename T >
BE_EXPORT raw< const RTTI::Class > ClassID< minitl::array< T > >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const RTTI::Method s_methods[3]
       = {{istring("array"),
           {1, ClassID< minitl::array< T > >::s_method_array_overloads},
           {&s_methods[0]}},
          {istring("size"),
           {1, ClassID< minitl::array< T > >::s_method_size_overloads},
           {&s_methods[1]}},
          {istring("Index"),
           {2, ClassID< minitl::array< T > >::s_method_Index_overloads},
           {&s_methods[2]}}};
    static const RTTI::Class s_class = {"array",
                                        u32(sizeof(minitl::array< T >)),
                                        0,
                                        RTTI::ClassType_Array,
                                        {0},
                                        {be_class< void >().m_ptr},
                                        {0},
                                        {0},
                                        {0, 0},
                                        {3, s_methods},
                                        {s_methods},
                                        {&ClassID< minitl::array< T > >::scriptingAPI},
                                        &RTTI::wrap< minitl::array< T > >::copy,
                                        &RTTI::wrap< minitl::array< T > >::destroy};
    raw< const RTTI::Class > result  = {&s_class};
    return result;
}

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
