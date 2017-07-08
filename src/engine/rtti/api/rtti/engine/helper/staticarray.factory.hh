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

template< typename T >
be_section(rtti_text_trampoline)
static RTTI::Value callStaticArrayOperatorIndex(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< RTTI::staticarray<T>& >().operator[](params[1].as<u32>()));
}

template< typename T >
be_section(rtti_text_trampoline)
static RTTI::Value callStaticArrayOperatorIndexConst(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const RTTI::staticarray<T>& >().operator[](params[1].as<u32>()));
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
raw<const RTTI::Class> be_typeid< RTTI::staticarray<T> >::initialisation = be_typeid< RTTI::staticarray<T> >::klass();

template< typename T >
be_section(rtti_text_cls)
raw<RTTI::Class> be_typeid< RTTI::staticarray<T> >::preklass()
{
    be_section(rtti_cls)
    static RTTI::Class cls = {
        be_typeid< RTTI::staticarray<T> >::name(),
        u32(sizeof(RTTI::staticarray<T>)),
        0,
        RTTI::ClassType_Struct,
        {be_game_Namespace().m_ptr},
        {be_typeid< void >::preklass().m_ptr},
        {0},
        {0},
        {0},
        {0},
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
    be_section(rtti_method)
    static RTTI::staticarray_n< 1, const RTTI::Method::Parameter > s_Index_0_params = {
        1,
        {
            {
                {0},
                istring("index"),
                be_typeid< u32  >::type()
            }
        }
    };
    be_section(rtti_method)
    static RTTI::staticarray_n< 2, const RTTI::Method::Overload > s_method_Index_overloads = {
        2,
        {
            {
                {0},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_Index_0_params)},
                be_typeid< const T & >::type(),
                false,
                &callStaticArrayOperatorIndexConst<T>
            },
            {
                {0},
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Parameter >* >(&s_Index_0_params)},
                be_typeid< T & >::type(),
                false,
                &callStaticArrayOperatorIndex<T>
            }
        }
    };
    be_section(rtti_method)
    static RTTI::staticarray_n< 1, const RTTI::Method > s_methods = {
        1,
        {
            {
                istring("Index"),
                {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Overload >* >(&s_method_Index_overloads)},
                {&s_methods.elements[1]}
            }
        }
    };
    result->methods.set(reinterpret_cast< RTTI::staticarray< const RTTI::Method >* >(&s_methods));
    raw<const RTTI::ObjectInfo> objects = {0};
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
