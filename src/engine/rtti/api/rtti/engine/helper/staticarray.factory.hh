/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
#define BE_RTTI_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/engine/helper/staticarray.hh>
#include    <rtti/value.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/typeinfo.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/scriptingapi.hh>
#include    <rtti/engine/helper/method.hh>


namespace BugEngine
{

template< typename T >
struct be_typeid< RTTI::staticarray<T> >
{
    static BE_EXPORT raw<const RTTI::Class> klass();
    static BE_EXPORT RTTI::Type  type();
};

template< typename T >
struct staticarray_BugHelper
{
    static const RTTI::Type value_type;
    static u32 array_size(const RTTI::Value& v);
    static RTTI::Value index(RTTI::Value& v, u32 i);
    static RTTI::Value indexConst(const RTTI::Value& v, u32 i);

    static RTTI::Value callStaticArrayOperatorIndex(RTTI::Value* params, u32 paramCount);
    static RTTI::Value callStaticArraySize(RTTI::Value* params, u32 paramCount);
    static RTTI::Value callStaticArrayOperatorIndexConst(RTTI::Value* params, u32 paramCount);

    static const RTTI::Method::Parameter s_index_0_params[2];
    static const RTTI::Method::Parameter s_index_1_params[2];
    static const RTTI::Method::Overload s_method_index_overloads[2];
    static const RTTI::Method::Parameter s_size_params[1];
    static const RTTI::Method::Overload s_method_size_overloads[1];
    static const RTTI::ScriptingArrayAPI scriptingArrayAPI;
    static const RTTI::ScriptingAPI scriptingAPI;
};

template< typename T >
const RTTI::Type staticarray_BugHelper<T>::value_type = be_typeid<T>::type();

template< typename T >
u32 staticarray_BugHelper<T>::array_size(const RTTI::Value& v)
{
    return v.as< const RTTI::staticarray<T>& >().count;
}

template< typename T >
RTTI::Value staticarray_BugHelper<T>::index(RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< RTTI::staticarray<T>& >().operator[](i)));
}

template< typename T >
RTTI::Value staticarray_BugHelper<T>::indexConst(const RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< const RTTI::staticarray<T>& >().operator[](i)));
}

template< typename T >
RTTI::Value staticarray_BugHelper<T>::callStaticArrayOperatorIndex(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< RTTI::staticarray<T>& >().operator[](params[1].as<u32>()));
}

template< typename T >
RTTI::Value staticarray_BugHelper<T>::callStaticArraySize(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const RTTI::staticarray<T>& >().count);
}

template< typename T >
RTTI::Value staticarray_BugHelper<T>::callStaticArrayOperatorIndexConst(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 2, "expected 2 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const RTTI::staticarray<T>& >().operator[](params[1].as<u32>()));
}

template< typename T >
const RTTI::Method::Parameter staticarray_BugHelper<T>::s_index_0_params[2] = {
    {
        {0},
        istring("this"),
        be_typeid< RTTI::staticarray<T>& >::type(),
        {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        {0},
        istring("index"),
        be_typeid< u32  >::type(),
        {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T >
const RTTI::Method::Parameter staticarray_BugHelper<T>::s_index_1_params[2] = {
    {
        {0},
        istring("this"),
        be_typeid< const RTTI::staticarray<T>& >::type(),
        {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        {0},
        istring("index"),
        be_typeid< u32  >::type(),
        {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T >
const RTTI::Method::Overload staticarray_BugHelper<T>::s_method_index_overloads[2] = {
    {
        {0},
        {2, s_index_0_params},
        be_typeid< const T & >::type(),
        false,
        &callStaticArrayOperatorIndexConst
    },
    {
        {0},
        {2, s_index_1_params},
        be_typeid< T & >::type(),
        false,
        &callStaticArrayOperatorIndex
    }
};

template< typename T >
const RTTI::Method::Parameter staticarray_BugHelper<T>::s_size_params[1] = {
    {
        {0},
        istring("this"),
        be_typeid< const RTTI::staticarray<T>& >::type(),
        {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T >
const RTTI::Method::Overload staticarray_BugHelper<T>::s_method_size_overloads[1] = {
    {
        {0},
        {1, s_size_params},
        be_typeid< u32 >::type(),
        false,
        &callStaticArraySize
    }
};


template< typename T >
const RTTI::ScriptingArrayAPI staticarray_BugHelper<T>::scriptingArrayAPI = {
    value_type,
    &array_size,
    &index,
    &indexConst
};

template< typename T >
const RTTI::ScriptingAPI staticarray_BugHelper<T>::scriptingAPI = {
    {&scriptingArrayAPI}
};

template< typename T >
BE_EXPORT
raw<const RTTI::Class> be_typeid< RTTI::staticarray<T> >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const RTTI::Method s_methods[2] = {
        {
            RTTI::Class::nameOperatorIndex(),
            {2, staticarray_BugHelper<T>::s_method_index_overloads},
            {&s_methods[0]}
        },
        {
            istring("size"),
            {1, staticarray_BugHelper<T>::s_method_size_overloads},
            {&s_methods[1]}
        }
    };
    static const ::BugEngine::RTTI::Class s_class = {
        istring(minitl::format<1024u>("staticarray<%s>") | be_typeid<T>::klass()->name),
        u32(sizeof(RTTI::staticarray<T>)),
        0,
        RTTI::ClassType_Array,
        {be_bugengine_Namespace().m_ptr},
        {be_typeid< void >::klass().m_ptr},
        {0},
        {0},
        {0, 0},
        {2, s_methods},
        {0},
        {&staticarray_BugHelper<T>::scriptingAPI},
        &RTTI::wrap< RTTI::staticarray<T> >::copy,
        &RTTI::wrap< RTTI::staticarray<T> >::destroy
    };
    raw< const RTTI::Class > result = { &s_class };
    return result;
}

template< typename T >
BE_EXPORT
RTTI::Type be_typeid< RTTI::staticarray<T> >::type()
{
    return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
}

}

/**************************************************************************************************/
#endif
