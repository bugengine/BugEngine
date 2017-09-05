/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
#define BE_RTTI_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/engine/helper/staticarray.hh>
#include    <rtti/value.hh>
#include    <rtti/typeinfo.hh>


namespace BugEngine
{

namespace
{


template< typename T >
be_section(rtti_text_trampoline_factory)
static RTTI::Value callStaticArrayOperatorIndex(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< RTTI::staticarray<T>& >().operator[](params[1].as<u32>()));
}

template< typename T >
be_section(rtti_text_trampoline_factory)
static RTTI::Value callStaticArraySize(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const RTTI::staticarray<T>& >().count);
}

template< typename T >
be_section(rtti_text_trampoline_factory)
static RTTI::Value callStaticArrayOperatorIndexConst(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 2, "expected 2 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const RTTI::staticarray<T>& >().operator[](params[1].as<u32>()));
}

}

template< typename T >
struct be_typeid< RTTI::staticarray<T> >
{
    static const RTTI::Type value_type;
    static inline raw<RTTI::Class> preklass();
    static inline istring name();
    static inline raw<const RTTI::Class> klass();
    static inline RTTI::Type  type();
private:
    static raw<const RTTI::Class> initialisation;
    static raw<const RTTI::Class> registerProperties();
};

template< typename T >
BE_EXPORT
raw<const RTTI::Class> be_typeid< RTTI::staticarray<T> >::initialisation = be_typeid< RTTI::staticarray<T> >::klass();

template< typename T >
be_section(rtti_text_cls_factory)
raw<RTTI::Class> be_typeid< RTTI::staticarray<T> >::preklass()
{
    be_forceuse(initialisation);
    be_section(rtti_cls_factory)
    static ::BugEngine::RTTI::Class cls = {
        be_typeid< RTTI::staticarray<T> >::name(),
        u32(sizeof(RTTI::staticarray<T>)),
        0,
        RTTI::ClassType_Array,
        {be_game_Namespace().m_ptr},
        {be_typeid< void >::preklass().m_ptr},
        {0},
        {&RTTI::staticarray<const RTTI::Tag>::s_null},
        {&RTTI::staticarray<const RTTI::Property>::s_null},
        {&RTTI::staticarray<const RTTI::Method>::s_null},
        {0},
        &RTTI::wrapCopy< RTTI::staticarray<T> >,
        &RTTI::wrapDestroy< RTTI::staticarray<T> >};
    raw< RTTI::Class > result = { &cls };
    return result;
}

template< typename T >
const RTTI::Type be_typeid< RTTI::staticarray<T> >::value_type = be_typeid<T>::type();
template< typename T >
raw<const RTTI::Class> be_typeid< RTTI::staticarray<T> >::registerProperties()
{
    raw< RTTI::Class > result = preklass();
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 2, const RTTI::Method::Parameter > s_Index_0_params = {
        2,
        {
            {
                {&RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("this"),
                be_typeid< RTTI::staticarray<T>& >::type()
            },
            {
                {&RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("index"),
                be_typeid< u32  >::type()
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 2, const RTTI::Method::Parameter > s_Index_1_params = {
        2,
        {
            {
                {&RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("this"),
                be_typeid< const RTTI::staticarray<T>& >::type()
            },
            {
                {&RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("index"),
                be_typeid< u32  >::type()
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 2, const RTTI::Method::Overload > s_method_Index_overloads = {
        2,
        {
            {
                {&RTTI::staticarray<const RTTI::Tag>::s_null},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_Index_0_params)},
                be_typeid< const T & >::type(),
                false,
                &callStaticArrayOperatorIndexConst<T>
            },
            {
                {&RTTI::staticarray<const RTTI::Tag>::s_null},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_Index_1_params)},
                be_typeid< T & >::type(),
                false,
                &callStaticArrayOperatorIndex<T>
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 1, const RTTI::Method::Parameter > s_size_params = {
        1,
        {
            {
                {&RTTI::staticarray<const RTTI::Tag>::s_null},
                istring("this"),
                be_typeid< const RTTI::staticarray<T>& >::type()
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 1, const RTTI::Method::Overload > s_method_size_overloads = {
        1,
        {
            {
                {&RTTI::staticarray<const RTTI::Tag>::s_null},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_size_params)},
                be_typeid< u32 >::type(),
                false,
                &callStaticArraySize<T>
            }
        }
    };
    be_section(rtti_method_factory)
    static RTTI::staticarray_n< 2, const RTTI::Method > s_methods = {
        2,
        {
            {
                RTTI::Class::nameOperatorIndex(),
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Overload >* >(&s_method_Index_overloads)},
                {&s_methods.elements[0]}
            },
            {
                istring("size"),
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Overload >* >(&s_method_size_overloads)},
                {&s_methods.elements[1]}
            }
        }
    };
    result->methods.set(reinterpret_cast< RTTI::staticarray< const RTTI::Method >* >(&s_methods));
    static RTTI::ObjectInfo valueType = {
        {0},
        {&RTTI::staticarray<const RTTI::Tag>::s_null},
        "value_type",
        RTTI::Value(RTTI::Value::ByRef(value_type))
    };
    raw<const RTTI::ObjectInfo> objects = {&valueType};
    result->objects.set(objects.operator->());
    return result;
}

template< typename T >
istring be_typeid< RTTI::staticarray<T> >::name()
{
    static istring s_name(minitl::format<512u>("staticarray<%s>") | be_typeid<T>::name());
    return s_name;
}
template< typename T >
raw<const RTTI::Class> be_typeid< RTTI::staticarray<T> >::klass()
{
    static raw<const RTTI::Class> result = registerProperties();
    return result;
}

template< typename T >
RTTI::Type be_typeid< RTTI::staticarray<T> >::type()
{
    return RTTI::Type::makeType(preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
}

}

/**************************************************************************************************/
#endif
