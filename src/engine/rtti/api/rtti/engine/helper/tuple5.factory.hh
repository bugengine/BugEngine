/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_TUPLE5_HH_
#define BE_RTTI_ENGINE_HELPER_TUPLE5_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/helper/get.hh>
#include    <rtti/engine/helper/method.hh>
#include    <minitl/tuple.hh>

namespace BugEngine
{


template< typename T1, typename T2, typename T3, typename T4, typename T5 >
struct be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >
{
    static BE_EXPORT raw<const RTTI::Class> klass();
    static BE_EXPORT RTTI::Type  type();
    static BE_EXPORT istring name();
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
struct tuple5_BugHelper
{
    static RTTI::Value trampoline_method_tuple_overload_0(RTTI::Value* parameters, u32 parameterCount);
    static RTTI::Value trampoline_method_tuple_overload_1(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Parameter s_method_tuple_overload_1_params[];
    static const RTTI::Method::Overload s_method_tuple_overloads[];
    static const RTTI::Method s_methods[];
    static const RTTI::Property s_properties[];
    static const RTTI::ObjectInfo s_first_type_object_first_type;
    static const RTTI::ObjectInfo s_second_type_object_second_type;
    static const RTTI::ObjectInfo s_third_type_object_third_type;
    static const RTTI::ObjectInfo s_fourth_type_object_fourth_type;
    static const RTTI::ObjectInfo s_fifth_type_object_fifth_type;
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
RTTI::Value tuple5_BugHelper<T1, T2, T3, T4, T5>::trampoline_method_tuple_overload_0(RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(minitl::tuple<T1, T2, T3, T4, T5>());
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
RTTI::Value tuple5_BugHelper<T1, T2, T3, T4, T5>::trampoline_method_tuple_overload_1(RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(minitl::tuple<T1, T2, T3, T4, T5>(parameters[0].as< const T1&  >(),
                                                         parameters[1].as< const T2&  >(),
                                                         parameters[2].as< const T3&  >(),
                                                         parameters[3].as< const T4&  >(),
                                                         parameters[4].as< const T5&  >()));
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo tuple5_BugHelper<T1, T2, T3, T4, T5>::s_fifth_type_object_fifth_type = {
    {0},
    { 0 },
    istring(istring("fifth_type")),
    RTTI::Value(
        be_typeid< T5 >::type())
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo tuple5_BugHelper<T1, T2, T3, T4, T5>::s_fourth_type_object_fourth_type = {
    {&s_fifth_type_object_fifth_type},
    { 0 },
    istring(istring("fourth_type")),
    RTTI::Value(
        be_typeid< T4 >::type())
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo tuple5_BugHelper<T1, T2, T3, T4, T5>::s_third_type_object_third_type = {
    {&s_fourth_type_object_fourth_type},
    { 0 },
    istring(istring("third_type")),
    RTTI::Value(
        be_typeid< T3 >::type())
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo tuple5_BugHelper<T1, T2, T3, T4, T5>::s_second_type_object_second_type = {
    {&s_third_type_object_third_type},
    { 0 },
    istring(istring("second_type")),
    RTTI::Value(
        be_typeid< T2 >::type())
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::ObjectInfo tuple5_BugHelper<T1, T2, T3, T4, T5>::s_first_type_object_first_type = {
    {&s_second_type_object_second_type},
    { 0 },
    istring(istring("first_type")),
    RTTI::Value(
        be_typeid< T1 >::type())
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::Method::Parameter tuple5_BugHelper<T1, T2, T3, T4, T5>::s_method_tuple_overload_1_params[] = {
    {
        { 0 },
        istring("first"),
        be_typeid< T1  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        { 0 },
        istring("second"),
        be_typeid< T2  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        { 0 },
        istring("third"),
        be_typeid< T3  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        { 0 },
        istring("fourth"),
        be_typeid< T4  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        { 0 },
        istring("fifth"),
        be_typeid< T5  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::Method::Overload tuple5_BugHelper<T1, T2, T3, T4, T5>::s_method_tuple_overloads[] = {
    {
        { 0 },
        { 0, 0 },
        be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::type(),
        false,
        &trampoline_method_tuple_overload_0
    },
    {
        { 0 },
        { 5, s_method_tuple_overload_1_params },
        be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::type(),
        false,
        &trampoline_method_tuple_overload_1
    }
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::Method tuple5_BugHelper<T1, T2, T3, T4, T5>::s_methods[1] = {
    {
        istring("tuple"),
        { 2, s_method_tuple_overloads },
        {&s_methods[0]}
    }
};

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
const RTTI::Property tuple5_BugHelper<T1, T2, T3, T4, T5>::s_properties[5] = {
    {
        { 0 },
        istring("first"),
        be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::type(),
        be_typeid< T1  >::type(),
        &RTTI::PropertyHelper< T1 , minitl::tuple<T1, T2, T3, T4, T5>, &minitl::tuple<T1, T2, T3, T4, T5>::first >::get
    },
    {
        { 0 },
        istring("second"),
        be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::type(),
        be_typeid< T2  >::type(),
        &RTTI::PropertyHelper< T2 , minitl::tuple<T1, T2, T3, T4, T5>, &minitl::tuple<T1, T2, T3, T4, T5>::second >::get
    },
    {
        { 0 },
        istring("third"),
        be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::type(),
        be_typeid< T3  >::type(),
        &RTTI::PropertyHelper< T3 , minitl::tuple<T1, T2, T3, T4, T5>, &minitl::tuple<T1, T2, T3, T4, T5>::third >::get
    },
    {
        { 0 },
        istring("fourth"),
        be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::type(),
        be_typeid< T4  >::type(),
        &RTTI::PropertyHelper< T4 , minitl::tuple<T1, T2, T3, T4, T5>, &minitl::tuple<T1, T2, T3, T4, T5>::fourth >::get
    },
    {
        { 0 },
        istring("fifth"),
        be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::type(),
        be_typeid< T5  >::type(),
        &RTTI::PropertyHelper< T5 , minitl::tuple<T1, T2, T3, T4, T5>, &minitl::tuple<T1, T2, T3, T4, T5>::fifth >::get
    }
};


template< typename T1, typename T2, typename T3, typename T4, typename T5 >
BE_EXPORT
raw<const RTTI::Class> be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::klass()
{
    static const
    RTTI::Class s_class = {
        /* .name */               name(),
        /* .size */               u32(sizeof(minitl::tuple<T1, T2, T3, T4, T5>)),
        /* .offset */             0,
        /* .id */                 RTTI::ClassType_Struct,
        /* .owner */              {be_bugengine_Namespace().m_ptr},
        /* .parent */             {be_typeid< void >::klass().m_ptr},
        /* .objects */            {&tuple5_BugHelper<T1, T2, T3, T4, T5>::s_first_type_object_first_type},
        /* .tags */               { 0 },
        /* .properties */         { 5, tuple5_BugHelper<T1, T2, T3, T4, T5>::s_properties },
        /* .methods */            { 1, tuple5_BugHelper<T1, T2, T3, T4, T5>::s_methods },
        /* .constructor */        {tuple5_BugHelper<T1, T2, T3, T4, T5>::s_methods},
        /* .apiMethods */         {0},
        /* .copyconstructor */    &RTTI::wrapCopy< minitl::tuple<T1, T2, T3, T4, T5> >,
        /* .destructor */         &RTTI::wrapDestroy< minitl::tuple<T1, T2, T3, T4, T5> >
    };

    raw< const RTTI::Class > result = { &s_class };
    return result;
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
BE_EXPORT
RTTI::Type be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::type()
{
    return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
}

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
BE_EXPORT
istring be_typeid< minitl::tuple<T1, T2, T3, T4, T5> >::name()
{
    static istring s_name(minitl::format<1024u>("tuple<%s,%s,%s,%s,%s>")
                        | be_typeid<T1>::klass()->name
                        | be_typeid<T2>::klass()->name
                        | be_typeid<T3>::klass()->name
                        | be_typeid<T4>::klass()->name
                        | be_typeid<T5>::klass()->name);
    return s_name;
}

}

/**************************************************************************************************/
#endif

