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
be_section(rtti_text_trampoline_factory)
static RTTI::Value make_array(RTTI::Value* v, u32 count)
{
    T* t = (T*)malloca(sizeof(T)*count);
    for (u32 i = 0; i < count; ++i)
        new ((void*)&t[i]) T(v[i].as<T>());
    minitl::array<T> result(Arena::script(), t, t+count);
    for (u32 i = 0; i < count; ++i)
        result[count-i-1].~T();
    freea(t);
    return RTTI::Value(result);
}

template< typename T >
be_section(rtti_text_trampoline_factory)
static RTTI::Value callSize(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const minitl::array<T>& >().size());
}

template< typename T >
be_section(rtti_text_trampoline_factory)
static RTTI::Value callOperatorIndex(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< minitl::array<T>& >().operator[](params[1].as<u32>()));
}

template< typename T >
be_section(rtti_text_trampoline_factory)
static RTTI::Value callOperatorIndexConst(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const minitl::array<T>& >().operator[](params[1].as<u32>()));
}

template< typename T >
struct be_typeid< minitl::array<T> >
{
    static const RTTI::Type value_type;
    static inline raw<RTTI::Class> preklass();
    static inline istring name();
    static inline raw<const RTTI::Class> klass();
    static inline RTTI::Type  type();
    static BE_EXPORT raw<const RTTI::Class> s_initialisation;
private:
    static raw<const RTTI::Class> registerProperties();
};

template< typename T >
BE_EXPORT
raw<const RTTI::Class> be_typeid< minitl::array<T> >::s_initialisation = be_typeid< minitl::array<T> >::klass();

template< typename T >
be_section(rtti_text_cls_factory)
raw<RTTI::Class> be_typeid< minitl::array<T> >::preklass()
{
    be_forceuse(s_initialisation);
    be_section(rtti_cls_factory)
    static ::BugEngine::RTTI::Class cls = {
        be_typeid< minitl::array<T> >::name(),
        u32(sizeof(minitl::array<T>)),
        0,
        RTTI::ClassType_Array,
        {be_game_Namespace().m_ptr},
        {be_typeid< void >::preklass().m_ptr},
        {0},
        {RTTI::staticarray<const RTTI::Tag>::s_null},
        {RTTI::staticarray<const RTTI::Property>::s_null},
        {RTTI::staticarray<const RTTI::Method>::s_null},
        {0},
        &RTTI::wrapCopy< minitl::array<T> >,
        &RTTI::wrapDestroy< minitl::array<T> >};
    raw< RTTI::Class > result = { &cls };
    return result;
}

template< typename T >
const RTTI::Type be_typeid< minitl::array<T> >::value_type = be_typeid<T>::type();

template< typename T >
be_section(rtti_text_cls_factory)
raw<const RTTI::Class> be_typeid< minitl::array<T> >::registerProperties()
{
    raw< RTTI::Class > result = preklass();
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 1, const RTTI::Method::Overload > s_method_array_overloads = {
        1,
        {
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                {RTTI::staticarray<const RTTI::Method::Parameter>::s_null},
                be_typeid< minitl::array<T> >::type(),
                true,
                &make_array<T>
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 2, const RTTI::Method::Parameter > s_Index_0_params = {
        2,
        {
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("this"),
                be_typeid< minitl::array<T>& >::type(),
                RTTI::Value()
            },
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("index"),
                be_typeid< u32  >::type(),
                RTTI::Value()
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 2, const RTTI::Method::Parameter > s_Index_1_params = {
        2,
        {
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("this"),
                be_typeid< const minitl::array<T>& >::type(),
                RTTI::Value()
            },
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("index"),
                be_typeid< u32  >::type(),
                RTTI::Value()
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 2, const RTTI::Method::Overload > s_method_Index_overloads = {
        2,
        {
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_Index_0_params)},
                be_typeid< const T & >::type(),
                false,
                &callOperatorIndexConst<T>
            },
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_Index_1_params)},
                be_typeid< T & >::type(),
                false,
                &callOperatorIndex<T>
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 1, const RTTI::Method::Parameter > s_size_params = {
        1,
        {
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("this"),
                be_typeid< const minitl::array<T>& >::type(),
                RTTI::Value()
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 2, const RTTI::Method::Overload > s_method_size_overloads = {
        2,
        {
            {
                {RTTI::staticarray<const RTTI::Tag>::s_null},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_size_params)},
                be_typeid< u32 >::type(),
                false,
                &callSize<T>
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 3, const RTTI::Method > s_methods = {
        3,
        {
            {
                istring("array"),
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Overload >* >(&s_method_array_overloads)},
                {&s_methods.elements[0]}
            },
            {
                RTTI::Class::nameOperatorIndex(),
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Overload >* >(&s_method_Index_overloads)},
                {&s_methods.elements[1]}
            },
            {
                istring("size"),
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Overload >* >(&s_method_size_overloads)},
                {&s_methods.elements[2]}
            }
        }
    };
    result->methods.set(reinterpret_cast< RTTI::staticarray< const RTTI::Method >* >(&s_methods));
    static RTTI::ObjectInfo valueType = {
        {0},
        {RTTI::staticarray<const RTTI::Tag>::s_null},
        "value_type",
        RTTI::Value(RTTI::Value::ByRef(value_type))
    };
    raw<const RTTI::ObjectInfo> objects = {&valueType};
    result->objects.set(objects.operator->());
    return result;
}

template< typename T >
istring be_typeid< minitl::array<T> >::name()
{
    static istring s_name(minitl::format<512u>("array<%s>") | be_typeid<T>::name());
    return s_name;
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
    return RTTI::Type::makeType(preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
}

}

/**************************************************************************************************/
#endif

