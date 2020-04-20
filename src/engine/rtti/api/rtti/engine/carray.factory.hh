/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CARRAY_FACTORY_HH_
#define BE_RTTI_ENGINE_CARRAY_FACTORY_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/helper/method.hh>
#include    <rtti/engine/scriptingapi.hh>


namespace BugEngine
{

template< typename T, u32 Count >
struct be_typeid< T[Count] >
{
    static BE_EXPORT raw<const RTTI::Class> klass();
    static RTTI::Type  type()
    {
        return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
    }
};

template< typename T, u32 Count >
struct be_typeid< const T[Count] >
{
    static BE_EXPORT raw<const RTTI::Class> klass();
    static RTTI::Type  type()
    {
        return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
    }
};


template< typename T, u32 Count >
struct carray_BugHelper
{
    typedef T ArrayType[Count];

    static const RTTI::Type value_type;
    static u32 array_size(const RTTI::Value& v);
    static RTTI::Value index(RTTI::Value& v, u32 i);
    static RTTI::Value indexConst(const RTTI::Value& v, u32 i);

    static RTTI::Value trampoline_method_size_overload_0(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Parameter s_method_size_overload_0_params[];
    static const RTTI::Method::Overload s_method_size_overloads[];
    static RTTI::Value trampoline_method_Index_overload_0(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Parameter s_method_Index_overload_0_params[];
    static RTTI::Value trampoline_method_Index_overload_1(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Parameter s_method_Index_overload_1_params[];
    static const RTTI::Method::Overload s_method_Index_overloads[];
    static const RTTI::ObjectInfo s_prop_value_type_object_value_type;
    static const RTTI::ScriptingArrayAPI scriptingArrayAPI;
    static const RTTI::ScriptingAPI scriptingAPI;
};

template< typename T, u32 Count >
const RTTI::Type carray_BugHelper<T, Count>::value_type = be_typeid<T>::type();

template< typename T, u32 Count >
u32 carray_BugHelper<T, Count>::array_size(const RTTI::Value& v)
{
    be_forceuse(v);
    return Count;
}

template< typename T, u32 Count >
RTTI::Value carray_BugHelper<T, Count>::index(RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< ArrayType& >()[i]));
}

template< typename T, u32 Count >
RTTI::Value carray_BugHelper<T, Count>::indexConst(const RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< const ArrayType& >()[i]));
}

template< typename T, u32 Count >
RTTI::Value carray_BugHelper<T, Count>::trampoline_method_size_overload_0(RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    return RTTI::Value(Count);
}

template< typename T, u32 Count >
RTTI::Value carray_BugHelper<T, Count>::trampoline_method_Index_overload_0(RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    const ArrayType& array = parameters[0].as< const ArrayType& >();
    u32 index = parameters[1].as< u32  >();
    return RTTI::Value(RTTI::Value::ByRef(array[index]));
}

template< typename T, u32 Count >
RTTI::Value carray_BugHelper<T, Count>::trampoline_method_Index_overload_1(RTTI::Value* parameters, u32 parameterCount)
{
    be_forceuse(parameters);
    be_forceuse(parameterCount);
    ArrayType& array = parameters[0].as< ArrayType& >();
    u32 index = parameters[1].as< u32  >();
    return RTTI::Value(RTTI::Value::ByRef(array[index]));
}


template< typename T, u32 Count >
const RTTI::ObjectInfo carray_BugHelper<T, Count>::s_prop_value_type_object_value_type = {
    {0},
    {0},
    istring(istring("value_type")),
    RTTI::Value(value_type)
};

template< typename T, u32 Count >
const RTTI::Method::Parameter carray_BugHelper<T, Count>::s_method_size_overload_0_params[] = {
    {
        { 0 },
        istring("this"),
        be_typeid< const ArrayType& >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T, u32 Count >
const RTTI::Method::Overload carray_BugHelper<T, Count>::s_method_size_overloads[] = {
    {
        { 0 },
        { 1, s_method_size_overload_0_params },
        be_typeid< u32 >::type(),
        false,
        &trampoline_method_size_overload_0
    }
};

template< typename T, u32 Count >
const RTTI::Method::Parameter carray_BugHelper<T, Count>::s_method_Index_overload_0_params[] = {
    {
        { 0 },
        istring("this"),
        be_typeid< const ArrayType& >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        { 0 },
        istring("index"),
        be_typeid< u32  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T, u32 Count >
const RTTI::Method::Parameter carray_BugHelper<T, Count>::s_method_Index_overload_1_params[] = {
    {
        { 0 },
        istring("this"),
        be_typeid< ArrayType& >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    },
    {
        { 0 },
        istring("index"),
        be_typeid< u32  >::type(),
        {&RTTI::Method::Parameter::s_noDefaultValue}
    }
};

template< typename T, u32 Count >
const RTTI::Method::Overload carray_BugHelper<T, Count>::s_method_Index_overloads[] = {
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

template< typename T, u32 Count >
const RTTI::ScriptingArrayAPI carray_BugHelper<T, Count>::scriptingArrayAPI = {
    be_typeid<T>::type(),
    &array_size,
    &index,
    &indexConst
};

template< typename T, u32 Count >
const RTTI::ScriptingAPI carray_BugHelper<T, Count>::scriptingAPI = {
    {&scriptingArrayAPI}
};



template< typename T, u32 Count >
BE_EXPORT
raw<const RTTI::Class> be_typeid< T[Count] >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const RTTI::Method s_methods[2] = {
        {
            istring("size"),
            { 1, carray_BugHelper<T, Count>::s_method_size_overloads },
            {&s_methods[1]}
        },
        {
            istring("Index"),
            { 2, carray_BugHelper<T, Count>::s_method_Index_overloads },
            {&s_methods[2]}
        }
    };
    static const RTTI::Class s_class = {
        "array",
        u32(sizeof(T[Count])),
        0,
        RTTI::ClassType_Array,
        {0},
        {be_typeid< void >::klass().m_ptr},
        {0},
        { 0 },
        { 0, 0 },
        { 2, s_methods },
        { 0 },
        {&carray_BugHelper<T, Count>::scriptingAPI},
        &RTTI::wrap< T[Count] >::copy,
        &RTTI::wrap< T[Count] >::destroy
    };
    raw< const RTTI::Class > result = { &s_class };
    return result;
}

template< typename T, u32 Count >
BE_EXPORT
raw<const RTTI::Class> be_typeid< const T[Count] >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const RTTI::Method s_methods[2] = {
        {
            istring("size"),
            { 1, carray_BugHelper<const T, Count>::s_method_size_overloads },
            {&s_methods[1]}
        },
        {
            istring("Index"),
            { 2, carray_BugHelper<const T, Count>::s_method_Index_overloads },
            {&s_methods[2]}
        }
    };
    static const RTTI::Class s_class = {
        "array",
        u32(sizeof(const T[Count])),
        0,
        RTTI::ClassType_Array,
        {0},
        {be_typeid< void >::klass().m_ptr},
        {0},
        { 0 },
        { 0, 0 },
        { 2, s_methods },
        { 0 },
        {&carray_BugHelper<const T, Count>::scriptingAPI},
        &RTTI::wrap< T[Count] >::copy,
        &RTTI::wrap< const T[Count] >::destroy
    };
    raw< const RTTI::Class > result = { &s_class };
    return result;
}

}

/**************************************************************************************************/
#endif

