/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_TUPLE2_HH_
#define BE_RTTI_ENGINE_HELPER_TUPLE2_HH_
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

template < typename T1, typename T2 >
struct ClassID< minitl::tuple< T1, T2 > >
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

    static BE_EXPORT raw< const RTTI::Class > klass();
};

template < typename T1, typename T2 >
RTTI::Value
ClassID< minitl::tuple< T1, T2 > >::trampoline_method_tuple_overload_0(RTTI::Value* parameters,
                                                                       u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(minitl::tuple< T1, T2 >());
}

template < typename T1, typename T2 >
RTTI::Value
ClassID< minitl::tuple< T1, T2 > >::trampoline_method_tuple_overload_1(RTTI::Value* parameters,
                                                                       u32          parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(
        minitl::tuple< T1, T2 >(parameters[0].as< const T1& >(), parameters[1].as< const T2& >()));
}

template < typename T1, typename T2 >
const RTTI::ObjectInfo ClassID< minitl::tuple< T1, T2 > >::s_second_type_object_second_type
    = {{0}, {0}, istring(istring("second_type")), RTTI::Value(be_type< T2 >())};

template < typename T1, typename T2 >
const RTTI::ObjectInfo ClassID< minitl::tuple< T1, T2 > >::s_first_type_object_first_type
    = {{&s_second_type_object_second_type},
       {0},
       istring(istring("first_type")),
       RTTI::Value(be_type< T1 >())};

template < typename T1, typename T2 >
const RTTI::Method::Parameter ClassID< minitl::tuple< T1, T2 > >::s_method_tuple_overload_1_params[]
    = {{{0}, istring("first"), be_type< T1 >(), {&RTTI::Method::Parameter::s_noDefaultValue}},
       {{0}, istring("second"), be_type< T2 >(), {&RTTI::Method::Parameter::s_noDefaultValue}}};

template < typename T1, typename T2 >
const RTTI::Method::Overload ClassID< minitl::tuple< T1, T2 > >::s_method_tuple_overloads[] = {
    {{0},
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
const RTTI::Method ClassID< minitl::tuple< T1, T2 > >::s_methods[1]
    = {{istring("tuple"), {2, s_method_tuple_overloads}, {&s_methods[0]}}};

template < typename T1, typename T2 >
const RTTI::Property ClassID< minitl::tuple< T1, T2 > >::s_properties[2] = {
    {{0},
     istring("first"),
     be_type< minitl::tuple< T1, T2 > >(),
     be_type< T1 >(),
     &RTTI::PropertyHelper< T1, minitl::tuple< T1, T2 >, &minitl::tuple< T1, T2 >::first >::get},
    {{0},
     istring("second"),
     be_type< minitl::tuple< T1, T2 > >(),
     be_type< T2 >(),
     &RTTI::PropertyHelper< T2, minitl::tuple< T1, T2 >, &minitl::tuple< T1, T2 >::second >::get}};

template < typename T1, typename T2 >
BE_EXPORT raw< const RTTI::Class > ClassID< minitl::tuple< T1, T2 > >::klass()
{
    static const RTTI::Class s_class
        = {/* .name */ istring(minitl::format< 1024u >("tuple<%s,%s>") | be_type< T1 >().name()
                               | be_type< T2 >().name()),
           /* .size */ u32(sizeof(minitl::tuple< T1, T2 >)),
           /* .offset */ 0,
           /* .id */ RTTI::ClassType_Struct,
           /* .owner */ {be_bugengine_Namespace().m_ptr},
           /* .parent */ {be_class< void >().m_ptr},
           /* .objects */ {&ClassID< minitl::tuple< T1, T2 > >::s_first_type_object_first_type},
           /* .tags */ {0},
           /* .properties */ {2, ClassID< minitl::tuple< T1, T2 > >::s_properties},
           /* .methods */ {1, ClassID< minitl::tuple< T1, T2 > >::s_methods},
           /* .constructor */ {ClassID< minitl::tuple< T1, T2 > >::s_methods},
           /* .apiMethods */ {0},
           /* .copyconstructor */ &RTTI::wrap< minitl::tuple< T1, T2 > >::copy,
           /* .destructor */ &RTTI::wrap< minitl::tuple< T1, T2 > >::destroy};

    raw< const RTTI::Class > result = {&s_class};
    return result;
}

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
