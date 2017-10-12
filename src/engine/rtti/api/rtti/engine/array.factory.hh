/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_ARRAY_FACTORY_HH_
#define BE_RTTI_ENGINE_ARRAY_FACTORY_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/helper/method.hh>
#include    <rtti/engine/scriptingapi.hh>
#include    <minitl/array.hh>

namespace BugEngine
{

template< typename T >
struct be_typeid< minitl::array<T> >
{
    static BE_EXPORT raw<const RTTI::Class> klass();
    static BE_EXPORT RTTI::Type  type();
};


template< typename T >
struct array_BugHelper
{
    static const RTTI::Type value_type;
    static u32 array_size(const RTTI::Value& v);
    static RTTI::Value index(RTTI::Value& v, u32 i);
    static RTTI::Value indexConst(const RTTI::Value& v, u32 i);

    static RTTI::Value trampoline_method_array_overload_0(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Parameter s_method_array_overload_0_params[];
    static const RTTI::Method::Overload s_method_array_overloads[];
    static RTTI::Value trampoline_method_size_overload_0(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Parameter s_method_size_overload_0_params[];
    static const RTTI::Method::Overload s_method_size_overloads[];
    static RTTI::Value trampoline_method_Index_overload_0(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Parameter s_method_Index_overload_0_params[];
    static RTTI::Value trampoline_method_Index_overload_1(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Parameter s_method_Index_overload_1_params[];
    static const RTTI::Method::Overload s_method_Index_overloads[];
    static const RTTI::Method s_methods[];
    static const RTTI::ObjectInfo s_prop_value_type_object_value_type;
    static const RTTI::ScriptingArrayAPI scriptingArrayAPI;
    static const RTTI::ScriptingAPI scriptingAPI;
};

template< typename T >
const RTTI::Type array_BugHelper<T>::value_type = be_typeid<T>::type();

template< typename T >
u32 array_BugHelper<T>::array_size(const RTTI::Value& v)
{
    return v.as< const minitl::array<T>& >().size();
}

template< typename T >
RTTI::Value array_BugHelper<T>::index(RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< minitl::array<T>& >().operator[](i)));
}

template< typename T >
RTTI::Value array_BugHelper<T>::indexConst(const RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< const minitl::array<T>& >().operator[](i)));
}

template< typename T >
RTTI::Value array_BugHelper<T>::trampoline_method_size_overload_0(RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(parameters[0].as< const minitl::array<T>& >().size());
}

template< typename T >
RTTI::Value array_BugHelper<T>::trampoline_method_Index_overload_0(RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(RTTI::Value::ByRef(parameters[0].as< minitl::array<T>& >().operator[](parameters[1].as< u32  >())));
}

template< typename T >
RTTI::Value array_BugHelper<T>::trampoline_method_Index_overload_1(RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(RTTI::Value::ByRef(parameters[0].as< const minitl::array<T>& >().operator[](parameters[1].as< u32  >())));
}

template< typename T >
RTTI::Value array_BugHelper<T>::trampoline_method_array_overload_0(RTTI::Value* parameters, u32 parameterCount)
{
    T* t = (T*)malloca(sizeof(T)*parameterCount);
    for (u32 i = 0; i < parameterCount; ++i)
        new ((void*)&t[i]) T(parameters[i].as<T>());
    minitl::array<T> result(Arena::script(), t, t+parameterCount);
    for (u32 i = 0; i < parameterCount; ++i)
        result[parameterCount-i-1].~T();
    freea(t);
    return RTTI::Value(result);
}

template< typename T >
const RTTI::ObjectInfo array_BugHelper<T>::s_prop_value_type_object_value_type = {
    {0},
    {0},
    istring(istring("value_type")),
    RTTI::Value(value_type)
};

template< typename T >
const RTTI::Method::Overload array_BugHelper<T>::s_method_array_overloads[] = {
    {
        { 0 },
        { 0, 0 },
        be_typeid< minitl::array<T> >::type(),
        true,
        &trampoline_method_array_overload_0
    }
};

template< typename T >
const RTTI::Method::Parameter array_BugHelper<T>::s_method_size_overload_0_params[] = {
    {
        { 0 },
        istring("this"),
        be_typeid< const minitl::array<T>& >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T >
const RTTI::Method::Overload array_BugHelper<T>::s_method_size_overloads[] = {
    {
        { 0 },
        { 1, s_method_size_overload_0_params },
        be_typeid< u32 >::type(),
        false,
        &trampoline_method_size_overload_0
    }
};

template< typename T >
const RTTI::Method::Parameter array_BugHelper<T>::s_method_Index_overload_0_params[] = {
    {
        { 0 },
        istring("this"),
        be_typeid< const minitl::array<T>& >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        { 0 },
        istring("index"),
        be_typeid< u32  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T >
const RTTI::Method::Parameter array_BugHelper<T>::s_method_Index_overload_1_params[] = {
    {
        { 0 },
        istring("this"),
        be_typeid< minitl::array<T>& >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        { 0 },
        istring("index"),
        be_typeid< u32  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T >
const RTTI::Method::Overload array_BugHelper<T>::s_method_Index_overloads[] = {
    {
        { 0 },
        { 2, s_method_Index_overload_0_params },
        be_typeid< const T & >::type(),
        false,
        &trampoline_method_Index_overload_0
    },
    {
        { 0 },
        { 2, s_method_Index_overload_1_params },
        be_typeid< T & >::type(),
        false,
        &trampoline_method_Index_overload_1
    }
};

template< typename T >
const RTTI::Method array_BugHelper<T>::s_methods[3] = {
    {
        istring("array"),
        { 1, s_method_array_overloads },
        {&s_methods[0]}
    },
    {
        istring("size"),
        { 1, s_method_size_overloads },
        {&s_methods[1]}
    },
    {
        istring("Index"),
        { 2, s_method_Index_overloads },
        {&s_methods[2]}
    }
};

template< typename T >
const RTTI::ScriptingArrayAPI array_BugHelper<T>::scriptingArrayAPI = {
    value_type,
    &array_size,
    &index,
    &indexConst
};

template< typename T >
const RTTI::ScriptingAPI array_BugHelper<T>::scriptingAPI = {
    {&scriptingArrayAPI}
};



template< typename T >
BE_EXPORT
raw<const RTTI::Class> be_typeid< minitl::array<T> >::klass()
{
    static const RTTI::Class s_class = {
        "array",
        u32(sizeof(minitl::array<T>)),
        0,
        RTTI::ClassType_Array,
        {0},
        {be_typeid< void >::klass().m_ptr},
        {0},
        { 0 },
        { 0, 0 },
        { 3, array_BugHelper<T>::s_methods },
        {array_BugHelper<T>::s_methods},
        {&array_BugHelper<T>::scriptingAPI},
        &RTTI::wrapCopy< minitl::array<T> >,
        &RTTI::wrapDestroy< minitl::array<T> >
    };
    raw< const RTTI::Class > result = { &s_class };
    return result;
}

template< typename T >
BE_EXPORT
RTTI::Type be_typeid< minitl::array<T> >::type()
{
    return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
}

}

/**************************************************************************************************/
#endif

