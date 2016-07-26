/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_ARRAY_HH_
#define BE_RTTI_ENGINE_ARRAY_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/helper/method.hh>
#include    <minitl/array.hh>

namespace BugEngine
{

template< typename T >
static RTTI::Value make_array(RTTI::Value* v, u32 count)
{
    T* t = (T*)malloca(sizeof(T)*count);
    for (u32 i = 0; i < count; ++i)
        new (&t[i]) T(v[i].as<T>());
    minitl::array<T> result(Arena::script(), t, t+count);
    for (u32 i = 0; i < count; ++i)
        result[count-i-1].~T();
    freea(t);
    return RTTI::Value(result);
}

template< typename T >
RTTI::Value callSize(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const minitl::array<T>& >().size());
}

template< typename T >
RTTI::Value callOperatorIndex(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< minitl::array<T>& >().operator[](params[1].as<u32>()));
}

template< typename T >
RTTI::Value callOperatorIndexConst(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const minitl::array<T>& >().operator[](params[1].as<u32>()));
}

template< typename T >
struct be_typeid< minitl::array<T> >
{
    static const RTTI::Type value_type;
    static inline raw<RTTI::Class> preklass();
    static inline raw<const RTTI::Class> klass();
    static inline RTTI::Type  type();
private:
    static raw<const RTTI::Class> initialisation;
    static raw<const RTTI::Class> registerProperties();
};

template< typename T >
raw<const RTTI::Class> be_typeid< minitl::array<T> >::initialisation = be_typeid< minitl::array<T> >::klass();

template< typename T >
raw<RTTI::Class> be_typeid< minitl::array<T> >::preklass()
{
    static RTTI::Class klass = {
        istring(minitl::format<512u>("array<%s>") | be_typeid<T>::type().name()),
        {be_game_Namespace().m_ptr},
        {be_typeid< void >::preklass().m_ptr},
        0, 0, RTTI::ClassType_Array, {0}, {0},
        {0},
        {0},
        {0},
        {0},
        &RTTI::wrapCopy< minitl::array<T> >,
        &RTTI::wrapDestroy< minitl::array<T> >
    };
    raw<RTTI::Class> result = {&klass};
    return result;
}

template< typename T >
const RTTI::Type be_typeid< minitl::array<T> >::value_type = be_typeid<T>::type();
template< typename T >
raw<const RTTI::Class> be_typeid< minitl::array<T> >::registerProperties()
{
    static raw<RTTI::Class> result = preklass();
    static RTTI::Method::Overload constructor_overload = {
        {0},
        {0},
        { {result.m_ptr}, RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable},
        0,
        {0},
        true,
        &make_array<T>
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
    static RTTI::ObjectInfo valueTypeObject = {
        {&valueConstructor},
        {0},
        "value_type",
        RTTI::Value(RTTI::Value::ByRef(value_type))
    };
    static RTTI::Method::Parameter size_overload_this = {
        {0},
        {0},
        "this",
        be_typeid<const minitl::array<T>&>::type()
    };
    static RTTI::Method::Overload size_overload = {
        {0},
        {0},
        be_typeid<u32>::type(),
        1,
        {&size_overload_this},
        false,
        &callSize<T>
    };
    static RTTI::Method size_method = {
        "size",
        {0},
        {&size_method},
        1,
        {&size_overload}
    };


    static RTTI::Method::Parameter index_overload0_this = {
        {0},
        {0},
        "this",
        be_typeid<minitl::array<T>&>::type()
    };
    static RTTI::Method::Parameter index_overload0_index = {
        {0},
        {&index_overload0_this},
        "index",
        be_typeid<minitl::array<T>&>::type()
    };
    static RTTI::Method::Overload index_overload0 = {
        {0},
        {0},
        be_typeid<T&>::type(),
        1,
        {&index_overload0_index},
        false,
        &callOperatorIndex<T>
    };
    static RTTI::Method::Parameter index_overload1_this = {
        {0},
        {0},
        "this",
        be_typeid<const minitl::array<T>&>::type()
    };
    static RTTI::Method::Parameter index_overload1_index = {
        {0},
        {&index_overload1_this},
        "index",
        be_typeid<minitl::array<T>&>::type()
    };
    static RTTI::Method::Overload index_overload1 = {
        {0},
        {&index_overload0},
        be_typeid<const T&>::type(),
        1,
        {&index_overload1_index},
        false,
        &callOperatorIndexConst<T>
    };
    static RTTI::Method index_method = {
        "?index",
        {&size_method},
        {&index_method},
        1,
        {&index_overload1}
    };
    result->constructor.set(&constructor);
    result->objects.set(&valueTypeObject);
    result->methods.set(&index_method);
    return result;
}

template< typename T >
raw<const RTTI::Class> be_typeid< minitl::array<T> >::klass()
{
    static raw<const RTTI::Class> result = registerProperties();
    return result;
}

template< typename T >
RTTI::Type be_typeid< minitl::array<T> >::type()
{
    return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
}

}

/**************************************************************************************************/
#endif

