/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_TUPLE4_HH_
#define BE_RTTI_ENGINE_HELPER_TUPLE4_HH_
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

template< typename T1, typename T2, typename T3, typename T4 >
struct be_typeid< minitl::tuple<T1, T2, T3, T4> >
{
    static RTTI::Value make_tuple(RTTI::Value* v, u32 count);
    static const RTTI::Type first_type;
    static const RTTI::Type second_type;
    static const RTTI::Type third_type;
    static const RTTI::Type fourth_type;
    static inline raw<RTTI::Class> preklass();
    static inline istring name();
    static inline raw<const RTTI::Class> klass();
    static inline RTTI::Type  type();
private:
    static raw<const RTTI::Class> initialisation;
    static raw<const RTTI::Class> registerProperties();
};


template< typename T1, typename T2, typename T3, typename T4 >
RTTI::Value be_typeid< minitl::tuple<T1, T2, T3, T4> >::make_tuple(RTTI::Value* v,
                                                                   u32 count)
{
    be_assert(count == 4, "expected 4 parameter; received %d" | count);
    T1* t1 = static_cast<T1*>(malloca(sizeof(T1)));
    T2* t2 = static_cast<T2*>(malloca(sizeof(T2)));
    T3* t3 = static_cast<T3*>(malloca(sizeof(T3)));
    T4* t4 = static_cast<T4*>(malloca(sizeof(T4)));
    new (t1) T1(v[0].as<T1>());
    new (t2) T2(v[1].as<T2>());
    new (t3) T3(v[2].as<T3>());
    new (t4) T4(v[3].as<T4>());
    minitl::tuple<T1, T2, T3, T4> result(*t1, *t2, *t3, *t4);
    t4->~T4();
    t3->~T3();
    t2->~T2();
    t1->~T1();
    freea(t4);
    freea(t3);
    freea(t2);
    freea(t1);
    return RTTI::Value(result);
}

template< typename T1, typename T2, typename T3, typename T4 >
raw<const RTTI::Class> be_typeid< minitl::tuple<T1, T2, T3, T4> >::initialisation =
            be_typeid< minitl::tuple<T1, T2, T3, T4> >::klass();


template< typename T1, typename T2, typename T3, typename T4 >
raw<RTTI::Class> be_typeid< minitl::tuple<T1, T2, T3, T4> >::preklass()
{
    static RTTI::Class klass = {
        name(),
        {be_game_Namespace().m_ptr},
        {be_typeid< void >::preklass().m_ptr},
        0, 0, RTTI::ClassType_Struct, {0}, {0},
        {0},
        {0},
        {0},
        {0},
        &RTTI::wrapCopy< minitl::tuple<T1, T2, T3, T4> >,
        &RTTI::wrapDestroy< minitl::tuple<T1, T2, T3, T4> >
    };
    raw<RTTI::Class> result = {&klass};
    return result;
}

template< typename T1, typename T2, typename T3, typename T4 >
const RTTI::Type be_typeid< minitl::tuple<T1, T2, T3, T4> >::first_type = be_typeid<T1>::type();
template< typename T1, typename T2, typename T3, typename T4 >
const RTTI::Type be_typeid< minitl::tuple<T1, T2, T3, T4> >::second_type = be_typeid<T2>::type();
template< typename T1, typename T2, typename T3, typename T4 >
const RTTI::Type be_typeid< minitl::tuple<T1, T2, T3, T4> >::third_type = be_typeid<T3>::type();
template< typename T1, typename T2, typename T3, typename T4 >
const RTTI::Type be_typeid< minitl::tuple<T1, T2, T3, T4> >::fourth_type = be_typeid<T4>::type();


template< typename T1, typename T2, typename T3, typename T4 >
raw<const RTTI::Class> be_typeid< minitl::tuple<T1, T2, T3, T4> >::registerProperties()
{
    static raw<RTTI::Class> result = preklass();
    static RTTI::Method::Overload constructor_overload = {
        {0},
        {0},
        { {result.m_ptr}, RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable},
        0,
        {0},
        true,
        &be_typeid< minitl::tuple<T1, T2, T3, T4> >::make_tuple
    };
    static RTTI::Method constructor = {
        "?new",
        {0},
        {&constructor},
        1,
        {&constructor_overload}
    };
    static RTTI::ObjectInfo valueConstructor = {
        {0},
        {0},
        constructor.name,
        RTTI::Value(constructor)
    };
    static RTTI::ObjectInfo fourthTypeObject = {
        {&valueConstructor},
        {0},
        "fourth_type",
        RTTI::Value(RTTI::Value::ByRef(fourth_type))
    };
    static RTTI::ObjectInfo thirdTypeObject = {
        {&fourthTypeObject},
        {0},
        "third_type",
        RTTI::Value(RTTI::Value::ByRef(third_type))
    };
    static RTTI::ObjectInfo secondTypeObject = {
        {&thirdTypeObject},
        {0},
        "second_type",
        RTTI::Value(RTTI::Value::ByRef(second_type))
    };
    static RTTI::ObjectInfo firstTypeObject = {
        {&secondTypeObject},
        {0},
        "first_type",
        RTTI::Value(RTTI::Value::ByRef(first_type))
    };
    static RTTI::Property fourthProperty = {
        {0},
        {0},
        "fourth",
        type(),
        be_typeid<T4>::type(),
        &RTTI::PropertyHelper<T4, minitl::tuple<T1, T2, T3, T4>,
                              &minitl::tuple<T1, T2, T3, T4>::fourth >::get
    };
    static RTTI::Property thirdProperty = {
        {0},
        {&fourthProperty},
        "third",
        type(),
        be_typeid<T3>::type(),
        &RTTI::PropertyHelper<T3, minitl::tuple<T1, T2, T3, T4>,
                              &minitl::tuple<T1, T2, T3, T4>::third >::get
    };
    static RTTI::Property secondProperty = {
        {0},
        {&thirdProperty},
        "second",
        type(),
        be_typeid<T2>::type(),
        &RTTI::PropertyHelper<T2, minitl::tuple<T1, T2, T3, T4>,
                              &minitl::tuple<T1, T2, T3, T4>::second >::get
    };
    static RTTI::Property firstProperty = {
        {0},
        {&secondProperty},
        "first",
        type(),
        be_typeid<T1>::type(),
        &RTTI::PropertyHelper<T1, minitl::tuple<T1, T2, T3, T4>,
                              &minitl::tuple<T1, T2, T3, T4>::first>::get
    };


    static const RTTI::Method* constructors = result->constructor.set(&constructor);
    be_forceuse(constructors);
    static const RTTI::ObjectInfo* objects = result->objects.set(&firstTypeObject);
    be_forceuse(objects);
    static const RTTI::Property* props = result->properties.set(&firstProperty);
    be_forceuse(props);
    return result;
}

template< typename T1, typename T2, typename T3, typename T4 >
istring be_typeid< minitl::tuple<T1, T2, T3, T4> >::name()
{
    static istring s_name(minitl::format<512u>("tuple<%s,%s,%s,%s>")
                        | be_typeid<T1>::name()
                        | be_typeid<T2>::name()
                        | be_typeid<T3>::name()
                        | be_typeid<T4>::name());
    return s_name;
}

template< typename T1, typename T2, typename T3, typename T4 >
raw<const RTTI::Class> be_typeid< minitl::tuple<T1, T2, T3, T4> >::klass()
{
    static raw<const RTTI::Class> result = registerProperties();
    return result;
}

template< typename T1, typename T2, typename T3, typename T4 >
RTTI::Type be_typeid< minitl::tuple<T1, T2, T3, T4> >::type()
{
    return RTTI::Type::makeType(klass(), RTTI::Type::Value,
                                RTTI::Type::Mutable, RTTI::Type::Mutable);
}

}

/**************************************************************************************************/
#endif

