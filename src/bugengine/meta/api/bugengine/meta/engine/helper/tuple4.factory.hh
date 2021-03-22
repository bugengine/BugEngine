/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_HELPER_TUPLE4_HH_
#define BE_META_ENGINE_HELPER_TUPLE4_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/helper/get.hh>
#include <bugengine/meta/engine/helper/method.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>
#include <bugengine/meta/typeinfo.hh>
#include <bugengine/minitl/tuple.hh>

namespace BugEngine { namespace Meta {

template < typename T1, typename T2, typename T3, typename T4 >
struct ClassID< minitl::tuple< T1, T2, T3, T4 > >
{
    static Meta::Value                   trampoline_method_tuple_overload_0(Meta::Value* parameters,
                                                                            u32          parameterCount);
    static Meta::Value                   trampoline_method_tuple_overload_1(Meta::Value* parameters,
                                                                            u32          parameterCount);
    static const Meta::Method::Parameter s_method_tuple_overload_1_params[];
    static const Meta::Method::Overload  s_method_tuple_overloads[];
    static const Meta::Method            s_methods[];
    static const Meta::Property          s_properties[];
    static const Meta::ObjectInfo        s_first_type_object_first_type;
    static const Meta::ObjectInfo        s_second_type_object_second_type;
    static const Meta::ObjectInfo        s_third_type_object_third_type;
    static const Meta::ObjectInfo        s_fourth_type_object_fourth_type;

    static BE_EXPORT raw< const Meta::Class > klass();
};

template < typename T1, typename T2, typename T3, typename T4 >
Meta::Value ClassID< minitl::tuple< T1, T2, T3, T4 > >::trampoline_method_tuple_overload_0(
    Meta::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return Meta::Value(minitl::tuple< T1, T2, T3, T4 >());
}

template < typename T1, typename T2, typename T3, typename T4 >
Meta::Value ClassID< minitl::tuple< T1, T2, T3, T4 > >::trampoline_method_tuple_overload_1(
    Meta::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return Meta::Value(minitl::tuple< T1, T2, T3, T4 >(
        parameters[0].as< const T1& >(), parameters[1].as< const T2& >(),
        parameters[2].as< const T3& >(), parameters[3].as< const T4& >()));
}

template < typename T1, typename T2, typename T3, typename T4 >
const Meta::ObjectInfo ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_fourth_type_object_fourth_type
    = {{0}, {0}, istring(istring("fourth_type")), Meta::Value(be_type< T4 >())};

template < typename T1, typename T2, typename T3, typename T4 >
const Meta::ObjectInfo ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_third_type_object_third_type
    = {{&s_fourth_type_object_fourth_type},
       {0},
       istring(istring("third_type")),
       Meta::Value(be_type< T3 >())};

template < typename T1, typename T2, typename T3, typename T4 >
const Meta::ObjectInfo ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_second_type_object_second_type
    = {{&s_third_type_object_third_type},
       {0},
       istring(istring("second_type")),
       Meta::Value(be_type< T2 >())};

template < typename T1, typename T2, typename T3, typename T4 >
const Meta::ObjectInfo ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_first_type_object_first_type
    = {{&s_second_type_object_second_type},
       {0},
       istring(istring("first_type")),
       Meta::Value(be_type< T1 >())};

template < typename T1, typename T2, typename T3, typename T4 >
const Meta::Method::Parameter
    ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_method_tuple_overload_1_params[]
    = {{{0}, istring("first"), be_type< T1 >(), {&Meta::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("second"), be_type< T2 >(), {&Meta::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("third"), be_type< T3 >(), {&Meta::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("fourth"), be_type< T4 >(), {&Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T1, typename T2, typename T3, typename T4 >
const Meta::Method::Overload ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_method_tuple_overloads[]
    = {{{0},
        {0, 0},
        be_type< minitl::tuple< T1, T2, T3, T4 > >(),
        false,
        {0, 0},
        &trampoline_method_tuple_overload_0},
       {{0},
        {4, s_method_tuple_overload_1_params},
        be_type< minitl::tuple< T1, T2, T3, T4 > >(),
        false,
        {0, 0},
        &trampoline_method_tuple_overload_1}};

template < typename T1, typename T2, typename T3, typename T4 >
const Meta::Method ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_methods[1]
    = {{istring("tuple"), {2, s_method_tuple_overloads}, {&s_methods[0]}}};

template < typename T1, typename T2, typename T3, typename T4 >
const Meta::Property ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_properties[4]
    = {{{0},
        istring("first"),
        be_type< minitl::tuple< T1, T2, T3, T4 > >(),
        be_type< T1 >(),
        &Meta::PropertyHelper< T1, minitl::tuple< T1, T2, T3, T4 >,
                               &minitl::tuple< T1, T2, T3, T4 >::first >::get},
       {{0},
        istring("second"),
        be_type< minitl::tuple< T1, T2, T3, T4 > >(),
        be_type< T2 >(),
        &Meta::PropertyHelper< T2, minitl::tuple< T1, T2, T3, T4 >,
                               &minitl::tuple< T1, T2, T3, T4 >::second >::get},
       {{0},
        istring("third"),
        be_type< minitl::tuple< T1, T2, T3, T4 > >(),
        be_type< T3 >(),
        &Meta::PropertyHelper< T3, minitl::tuple< T1, T2, T3, T4 >,
                               &minitl::tuple< T1, T2, T3, T4 >::third >::get},
       {{0},
        istring("fourth"),
        be_type< minitl::tuple< T1, T2, T3, T4 > >(),
        be_type< T4 >(),
        &Meta::PropertyHelper< T4, minitl::tuple< T1, T2, T3, T4 >,
                               &minitl::tuple< T1, T2, T3, T4 >::fourth >::get}};

template < typename T1, typename T2, typename T3, typename T4 >
BE_EXPORT raw< const Meta::Class > ClassID< minitl::tuple< T1, T2, T3, T4 > >::klass()
{
    static const Meta::Class s_class
        = {/* .name */ istring(minitl::format< 1024u >("tuple<%s,%s,%s,%s>")
                               | be_type< T1 >().name() | be_type< T2 >().name()
                               | be_type< T3 >().name() | be_type< T4 >().name()),
           /* .size */ u32(sizeof(minitl::tuple< T1, T2, T3, T4 >)),
           /* .offset */ 0,
           /* .id */ Meta::ClassType_Struct,
           /* .owner */ {be_bugengine_Namespace().m_ptr},
           /* .parent */ {be_class< void >().m_ptr},
           /* .objects */
           {&ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_first_type_object_first_type},
           /* .tags */ {0},
           /* .properties */ {4, ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_properties},
           /* .methods */ {1, ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_methods},
           /* .constructor */ {ClassID< minitl::tuple< T1, T2, T3, T4 > >::s_methods},
           /* .apiMethods */ {0},
           /* .copyconstructor */ &Meta::wrap< minitl::tuple< T1, T2, T3, T4 > >::copy,
           /* .destructor */ &Meta::wrap< minitl::tuple< T1, T2, T3, T4 > >::destroy};

    raw< const Meta::Class > result = {&s_class};
    return result;
}

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
