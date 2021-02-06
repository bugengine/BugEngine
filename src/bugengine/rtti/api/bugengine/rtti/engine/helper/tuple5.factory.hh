/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_TUPLE5_HH_
#define BE_RTTI_ENGINE_HELPER_TUPLE5_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/minitl/tuple.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/helper/get.hh>
#include <bugengine/rtti/engine/helper/method.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/engine/propertyinfo.script.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine { namespace RTTI {

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
struct ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >
{
    static RTTI::Value                   trampoline_method_tuple_overload_0(RTTI::Value* parameters,
                                                                            u32          parameterCount);
    static RTTI::Value                   trampoline_method_tuple_overload_1(RTTI::Value* parameters,
                                                                            u32          parameterCount);
    static const RTTI::Method::Parameter s_method_tuple_overload_1_params[];
    static const RTTI::Method::Overload  s_method_tuple_overloads[];
    static const RTTI::Method            s_methods[];
    static const RTTI::Property          s_properties[];
    static const RTTI::ObjectInfo        s_first_type_object_first_type;
    static const RTTI::ObjectInfo        s_second_type_object_second_type;
    static const RTTI::ObjectInfo        s_third_type_object_third_type;
    static const RTTI::ObjectInfo        s_fourth_type_object_fourth_type;
    static const RTTI::ObjectInfo        s_fifth_type_object_fifth_type;

    static BE_EXPORT raw< const RTTI::Class > klass();
};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
struct tuple5_BugHelper
{
};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
RTTI::Value ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::trampoline_method_tuple_overload_0(
    RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(minitl::tuple< T1, T2, T3, T4, T5 >());
}

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
RTTI::Value ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::trampoline_method_tuple_overload_1(
    RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(minitl::tuple< T1, T2, T3, T4, T5 >(
        parameters[0].as< const T1& >(), parameters[1].as< const T2& >(),
        parameters[2].as< const T3& >(), parameters[3].as< const T4& >(),
        parameters[4].as< const T5& >()));
}

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo
    ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_fifth_type_object_fifth_type
    = {{0}, {0}, istring(istring("fifth_type")), RTTI::Value(be_type< T5 >())};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo
    ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_fourth_type_object_fourth_type
    = {{&s_fifth_type_object_fifth_type},
       {0},
       istring(istring("fourth_type")),
       RTTI::Value(be_type< T4 >())};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo
    ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_third_type_object_third_type
    = {{&s_fourth_type_object_fourth_type},
       {0},
       istring(istring("third_type")),
       RTTI::Value(be_type< T3 >())};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo
    ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_second_type_object_second_type
    = {{&s_third_type_object_third_type},
       {0},
       istring(istring("second_type")),
       RTTI::Value(be_type< T2 >())};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo
    ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_first_type_object_first_type
    = {{&s_second_type_object_second_type},
       {0},
       istring(istring("first_type")),
       RTTI::Value(be_type< T1 >())};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::Method::Parameter
    ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_method_tuple_overload_1_params[]
    = {{{0}, istring("first"), be_type< T1 >(), {&RTTI::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("second"), be_type< T2 >(), {&RTTI::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("third"), be_type< T3 >(), {&RTTI::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("fourth"), be_type< T4 >(), {&RTTI::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("fifth"), be_type< T5 >(), {&RTTI::Method::Parameter::s_noDefaultValue}}};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::Method::Overload
    ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_method_tuple_overloads[]
    = {{{0},
        {0, 0},
        be_type< minitl::tuple< T1, T2, T3, T4, T5 > >(),
        false,
        {0, 0},
        &trampoline_method_tuple_overload_0},
       {{0},
        {5, s_method_tuple_overload_1_params},
        be_type< minitl::tuple< T1, T2, T3, T4, T5 > >(),
        false,
        {0, 0},
        &trampoline_method_tuple_overload_1}};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::Method ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_methods[1]
    = {{istring("tuple"), {2, s_method_tuple_overloads}, {&s_methods[0]}}};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::Property ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_properties[5]
    = {{{0},
        istring("first"),
        be_type< minitl::tuple< T1, T2, T3, T4, T5 > >(),
        be_type< T1 >(),
        &RTTI::PropertyHelper< T1, minitl::tuple< T1, T2, T3, T4, T5 >,
                               &minitl::tuple< T1, T2, T3, T4, T5 >::first >::get},
       {{0},
        istring("second"),
        be_type< minitl::tuple< T1, T2, T3, T4, T5 > >(),
        be_type< T2 >(),
        &RTTI::PropertyHelper< T2, minitl::tuple< T1, T2, T3, T4, T5 >,
                               &minitl::tuple< T1, T2, T3, T4, T5 >::second >::get},
       {{0},
        istring("third"),
        be_type< minitl::tuple< T1, T2, T3, T4, T5 > >(),
        be_type< T3 >(),
        &RTTI::PropertyHelper< T3, minitl::tuple< T1, T2, T3, T4, T5 >,
                               &minitl::tuple< T1, T2, T3, T4, T5 >::third >::get},
       {{0},
        istring("fourth"),
        be_type< minitl::tuple< T1, T2, T3, T4, T5 > >(),
        be_type< T4 >(),
        &RTTI::PropertyHelper< T4, minitl::tuple< T1, T2, T3, T4, T5 >,
                               &minitl::tuple< T1, T2, T3, T4, T5 >::fourth >::get},
       {{0},
        istring("fifth"),
        be_type< minitl::tuple< T1, T2, T3, T4, T5 > >(),
        be_type< T5 >(),
        &RTTI::PropertyHelper< T5, minitl::tuple< T1, T2, T3, T4, T5 >,
                               &minitl::tuple< T1, T2, T3, T4, T5 >::fifth >::get}};

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
BE_EXPORT raw< const RTTI::Class > ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::klass()
{
    static const RTTI::Class s_class
        = {/* .name */ istring(minitl::format< 1024u >("tuple<%s,%s,%s,%s,%s>")
                               | be_type< T1 >().name() | be_type< T2 >().name()
                               | be_type< T3 >().name() | be_type< T4 >().name()
                               | be_type< T5 >().name()),
           /* .size */ u32(sizeof(minitl::tuple< T1, T2, T3, T4, T5 >)),
           /* .offset */ 0,
           /* .id */ RTTI::ClassType_Struct,
           /* .owner */ {be_bugengine_Namespace().m_ptr},
           /* .parent */ {be_class< void >().m_ptr},
           /* .objects */
           {&ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_first_type_object_first_type},
           /* .tags */ {0},
           /* .properties */ {5, ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_properties},
           /* .methods */ {1, ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_methods},
           /* .constructor */ {ClassID< minitl::tuple< T1, T2, T3, T4, T5 > >::s_methods},
           /* .apiMethods */ {0},
           /* .copyconstructor */ &RTTI::wrap< minitl::tuple< T1, T2, T3, T4, T5 > >::copy,
           /* .destructor */ &RTTI::wrap< minitl::tuple< T1, T2, T3, T4, T5 > >::destroy};

    raw< const RTTI::Class > result = {&s_class};
    return result;
}

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
