/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_ARRAY_HH_
#define BE_RTTI_ENGINE_ARRAY_HH_
/*****************************************************************************/
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/helper/method.hh>
#include    <minitl/array.hh>

namespace minitl
{

struct iarray
{
};

}

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
struct be_typeid< minitl::array<T> >
{
    static const RTTI::Type value_type;
    static RTTI::Method::Overload constructor_overload;
    static RTTI::Method constructor;
    static RTTI::ObjectInfo valueTypeObject;

    static inline raw<RTTI::Class> preklass();
    static inline raw<const RTTI::Class> klass();
    static inline RTTI::Type  type();
};

template< typename T >
const RTTI::Type be_typeid< minitl::array<T> >::value_type = be_typeid<T>::type();

template< typename T >
RTTI::Method::Overload be_typeid< minitl::array<T> >::constructor_overload = {
    {0},
    {0},
    be_typeid< minitl::array<T> >::type(),
    {0},
    true,
    &make_array<T>
};

template< typename T >
RTTI::Method be_typeid< minitl::array<T> >::constructor = {
    "?new",
    {0},
    {&constructor},
    {&constructor_overload}
};

template< typename T >
RTTI::ObjectInfo be_typeid< minitl::array<T> >::valueTypeObject = {
    {0},
    {0},
    "value_type",
    RTTI::Value(RTTI::Value::ByRef(value_type))
};

template< typename T >
raw<RTTI::Class> be_typeid< minitl::array<T> >::preklass()
{
    static RTTI::Class ci = {
        istring(minitl::format<512u>("array<%s>") | be_typeid<T>::type().name()),
        be_game_Namespace(),
        be_typeid< minitl::iarray >::preklass(),
        0, 0, {0}, {0}, {0},
        {&valueTypeObject},
        {&constructor},
        {0},
        &RTTI::wrapCopy< minitl::array<T> >,
        &RTTI::wrapDestroy< minitl::array<T> >
    };
    raw<RTTI::Class> c = { &ci };
    return c;
}

template< typename T >
raw<const RTTI::Class> be_typeid< minitl::array<T> >::klass()
{
    return preklass();
}

template< typename T >
RTTI::Type be_typeid< minitl::array<T> >::type()
{
    return RTTI::Type::makeType(preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
}

}

/*****************************************************************************/
#endif

