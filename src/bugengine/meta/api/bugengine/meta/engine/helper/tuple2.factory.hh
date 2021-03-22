/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_HELPER_TUPLE2_HH_
#define BE_META_ENGINE_HELPER_TUPLE2_HH_
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

template < typename T1, typename T2 >
struct ClassID< minitl::tuple< T1, T2 > >
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

    static BE_EXPORT raw< const Meta::Class > klass();
};

template < typename T1, typename T2 >
Meta::Value
ClassID< minitl::tuple< T1, T2 > >::trampoline_method_tuple_overload_0(Meta::Value* parameters,
                                                                       u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return Meta::Value(minitl::tuple< T1, T2 >());
}

template < typename T1, typename T2 >
Meta::Value
ClassID< minitl::tuple< T1, T2 > >::trampoline_method_tuple_overload_1(Meta::Value* parameters,
                                                                       u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return Meta::Value(
        minitl::tuple< T1, T2 >(parameters[0].as< const T1& >(), parameters[1].as< const T2& >()));
}

template < typename T1, typename T2 >
const Meta::ObjectInfo ClassID< minitl::tuple< T1, T2 > >::s_second_type_object_second_type
    = {{0}, {0}, istring(istring("second_type")), Meta::Value(be_type< T2 >())};

template < typename T1, typename T2 >
const Meta::ObjectInfo ClassID< minitl::tuple< T1, T2 > >::s_first_type_object_first_type
    = {{&s_second_type_object_second_type},
       {0},
       istring(istring("first_type")),
       Meta::Value(be_type< T1 >())};

template < typename T1, typename T2 >
const Meta::Method::Parameter ClassID< minitl::tuple< T1, T2 > >::s_method_tuple_overload_1_params[]
    = {{{0}, istring("first"), be_type< T1 >(), {&Meta::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("second"), be_type< T2 >(), {&Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T1, typename T2 >
const Meta::Method::Overload ClassID< minitl::tuple< T1, T2 > >::s_method_tuple_overloads[]
    = {{{0},
        {0, 0},
        be_type< minitl::tuple< T1, T2 > >(),
        false,
        {0, 0},
        &trampoline_method_tuple_overload_0},
       {{0},
        {2, s_method_tuple_overload_1_params},
        be_type< minitl::tuple< T1, T2 > >(),
        false,
        {0, 0},
        &trampoline_method_tuple_overload_1}};

template < typename T1, typename T2 >
const Meta::Method ClassID< minitl::tuple< T1, T2 > >::s_methods[1]
    = {{istring("tuple"), {2, s_method_tuple_overloads}, {&s_methods[0]}}};

template < typename T1, typename T2 >
const Meta::Property ClassID< minitl::tuple< T1, T2 > >::s_properties[2] = {
    {{0},
     istring("first"),
     be_type< minitl::tuple< T1, T2 > >(),
     be_type< T1 >(),
     &Meta::PropertyHelper< T1, minitl::tuple< T1, T2 >, &minitl::tuple< T1, T2 >::first >::get},
    {{0},
     istring("second"),
     be_type< minitl::tuple< T1, T2 > >(),
     be_type< T2 >(),
     &Meta::PropertyHelper< T2, minitl::tuple< T1, T2 >, &minitl::tuple< T1, T2 >::second >::get}};

template < typename T1, typename T2 >
BE_EXPORT raw< const Meta::Class > ClassID< minitl::tuple< T1, T2 > >::klass()
{
    static const Meta::Class s_class
        = {/* .name */ istring(minitl::format< 1024u >("tuple<%s,%s>") | be_type< T1 >().name()
                               | be_type< T2 >().name()),
           /* .size */ u32(sizeof(minitl::tuple< T1, T2 >)),
           /* .offset */ 0,
           /* .id */ Meta::ClassType_Struct,
           /* .owner */ {be_bugengine_Namespace().m_ptr},
           /* .parent */ {be_class< void >().m_ptr},
           /* .objects */ {&ClassID< minitl::tuple< T1, T2 > >::s_first_type_object_first_type},
           /* .tags */ {0},
           /* .properties */ {2, ClassID< minitl::tuple< T1, T2 > >::s_properties},
           /* .methods */ {1, ClassID< minitl::tuple< T1, T2 > >::s_methods},
           /* .constructor */ {ClassID< minitl::tuple< T1, T2 > >::s_methods},
           /* .apiMethods */ {0},
           /* .copyconstructor */ &Meta::wrap< minitl::tuple< T1, T2 > >::copy,
           /* .destructor */ &Meta::wrap< minitl::tuple< T1, T2 > >::destroy};

    raw< const Meta::Class > result = {&s_class};
    return result;
}

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
