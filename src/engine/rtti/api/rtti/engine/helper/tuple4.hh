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
    static RTTI::Value make_default_tuple(RTTI::Value* v, u32 count);
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
be_section(rtti_text_trampoline)
RTTI::Value be_typeid< minitl::tuple<T1, T2, T3, T4> >::make_tuple(RTTI::Value* v,
                                                                   u32 count)
{
    be_assert(count == 4, "expected 4 parameters; received %d" | count);
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
be_section(rtti_text_trampoline)
RTTI::Value be_typeid< minitl::tuple<T1, T2, T3, T4> >::make_default_tuple(RTTI::Value* v,
                                                                           u32 count)
{
    be_assert(count == 0, "expected no parameter; received %d" | count);
    be_forceuse(v);
    be_forceuse(count);
    return RTTI::Value(minitl::tuple<T1, T2, T3, T4>());
}

template< typename T1, typename T2, typename T3, typename T4 >
raw<const RTTI::Class> be_typeid< minitl::tuple<T1, T2, T3, T4> >::initialisation =
            be_typeid< minitl::tuple<T1, T2, T3, T4> >::klass();


template< typename T1, typename T2, typename T3, typename T4 >
be_section(rtti_text_cls)
raw<RTTI::Class> be_typeid< minitl::tuple<T1, T2, T3, T4> >::preklass()
{
    be_section(rtti_cls)
    static RTTI::Class cls = {
        be_typeid< minitl::tuple<T1, T2, T3, T4> >::name(),
        u32(sizeof(minitl::tuple<T1, T2, T3, T4>)),
        0,
        RTTI::ClassType_Struct,
        {be_game_Namespace().m_ptr},
        {be_typeid< void >::preklass().m_ptr},
        {0},
        {0},
        {0},
        {0},
        {0},
        &RTTI::wrapCopy< minitl::tuple<T1, T2, T3, T4> >,
        &RTTI::wrapDestroy< minitl::tuple<T1, T2, T3, T4> >};
    raw< RTTI::Class > result = { &cls };
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
be_section(rtti_text_cls_props)
raw<const RTTI::Class> be_typeid< minitl::tuple<T1, T2, T3, T4> >::registerProperties()
{
    raw< RTTI::Class > result = preklass();
    be_section(rtti_method)
    static RTTI::staticarray_n< 4, const RTTI::Method::Parameter > s_tuple_0_params = {
        4,
        {
            { {0}, istring("first"), be_typeid< T1 >::type() },
            { {0}, istring("second"), be_typeid< T2 >::type() },
            { {0}, istring("third"), be_typeid< T3 >::type() },
            { {0}, istring("fourth"), be_typeid< T4 >::type() }
        }
    };
    be_section(rtti_method)
    static RTTI::staticarray_n< 2, const RTTI::Method::Overload > s_method_tuple_overloads = {
        2,
        {
            {
                {0},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_tuple_0_params)},
                be_typeid< minitl::tuple<T1, T2, T3, T4> >::type(),
                false,
                &make_tuple
            },
            {
                {0},
                {0},
                be_typeid< minitl::tuple<T1, T2, T3, T4> >::type(),
                false,
                &make_default_tuple
            }
        }
    };
    be_section(rtti_method)
    static RTTI::staticarray_n< 1, const RTTI::Method > s_methods = {
        1,
        {
            {
                istring("tuple"),
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Overload >* >(&s_method_tuple_overloads)},
                {&s_methods.elements[0]}
            }
        }
    };
    result->methods.set(reinterpret_cast< RTTI::staticarray< const RTTI::Method >* >(&s_methods));
    be_section(rtti_prop)
    static RTTI::staticarray_n< 4, const RTTI::Property > s_properties = {
        4,
        {
            {
                {0},
                istring("fourth"),
                be_typeid< minitl::tuple<T1, T2, T3, T4> >::type(),
                be_typeid< T4  >::type(),
                &RTTI::PropertyHelper< T4 , minitl::tuple<T1, T2, T3, T4>, &minitl::tuple<T1, T2, T3, T4>::fourth >::get
            },
            {
                {0},
                istring("third"),
                be_typeid< minitl::tuple<T1, T2, T3, T4> >::type(),
                be_typeid< T3  >::type(),
                &RTTI::PropertyHelper< T3 , minitl::tuple<T1, T2, T3, T4>, &minitl::tuple<T1, T2, T3, T4>::third >::get
            },
            {
                {0},
                istring("second"),
                be_typeid< minitl::tuple<T1, T2, T3, T4> >::type(),
                be_typeid< T2  >::type(),
                &RTTI::PropertyHelper< T2 , minitl::tuple<T1, T2, T3, T4>, &minitl::tuple<T1, T2, T3, T4>::second >::get
            },
            {
                {0},
                istring("first"),
                be_typeid< minitl::tuple<T1, T2, T3, T4> >::type(),
                be_typeid< T1  >::type(),
                &RTTI::PropertyHelper< T1 , minitl::tuple<T1, T2, T3, T4>, &minitl::tuple<T1, T2, T3, T4>::first >::get
            }
        }
    };
    result->properties.set(reinterpret_cast< RTTI::staticarray< const RTTI::Property >* >(&s_properties));
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

