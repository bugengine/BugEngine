/* BugEngine <bugengine.devel@gmail.com>
see LICENSE for detail */

#ifndef BE_META_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
#define BE_META_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/helper/method.hh>
#include <bugengine/meta/engine/helper/staticarray.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>
#include <bugengine/meta/engine/scriptingapi.hh>
#include <bugengine/meta/typeinfo.hh>
#include <bugengine/meta/typeinfo.script.hh>
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta {

template < typename T >
struct ClassID< Meta::staticarray< T > >
{
    static const Meta::Type value_type;
    static u32              array_size(const Meta::Value& v);
    static Meta::Value      index(Meta::Value& v, u32 i);
    static Meta::Value      indexConst(const Meta::Value& v, u32 i);

    static Meta::Value callStaticArrayOperatorIndex(Meta::Value* params, u32 paramCount);
    static Meta::Value callStaticArraySize(Meta::Value* params, u32 paramCount);
    static Meta::Value callStaticArrayOperatorIndexConst(Meta::Value* params, u32 paramCount);

    static const Meta::Method::Parameter s_index_0_params[2];
    static const Meta::Method::Parameter s_index_1_params[2];
    static const Meta::Method::Overload  s_method_index_overloads[2];
    static const Meta::Method::Parameter s_size_params[1];
    static const Meta::Method::Overload  s_method_size_overloads[1];
    static const Meta::ScriptingArrayAPI scriptingArrayAPI;
    static const Meta::ScriptingAPI      scriptingAPI;

    static BE_EXPORT raw< const Meta::Class > klass();
};

template < typename T >
const Meta::Type ClassID< Meta::staticarray< T > >::value_type = be_type< T >();

template < typename T >
u32 ClassID< Meta::staticarray< T > >::array_size(const Meta::Value& v)
{
    return v.as< const Meta::staticarray< T >& >().count;
}

template < typename T >
Meta::Value ClassID< Meta::staticarray< T > >::index(Meta::Value& v, u32 i)
{
    return Meta::Value(Meta::Value::ByRef(v.as< Meta::staticarray< T >& >().operator[](i)));
}

template < typename T >
Meta::Value ClassID< Meta::staticarray< T > >::indexConst(const Meta::Value& v, u32 i)
{
    return Meta::Value(Meta::Value::ByRef(v.as< const Meta::staticarray< T >& >().operator[](i)));
}

template < typename T >
Meta::Value ClassID< Meta::staticarray< T > >::callStaticArrayOperatorIndex(Meta::Value* params,
                                                                            u32          paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return Meta::Value(params[0].as< Meta::staticarray< T >& >().operator[](params[1].as< u32 >()));
}

template < typename T >
Meta::Value ClassID< Meta::staticarray< T > >::callStaticArraySize(Meta::Value* params,
                                                                   u32          paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return Meta::Value(params[0].as< const Meta::staticarray< T >& >().count);
}

template < typename T >
Meta::Value
ClassID< Meta::staticarray< T > >::callStaticArrayOperatorIndexConst(Meta::Value* params,
                                                                     u32          paramCount)
{
    be_assert(paramCount == 2, "expected 2 parameter; received %d" | paramCount);
    return Meta::Value(
        params[0].as< const Meta::staticarray< T >& >().operator[](params[1].as< u32 >()));
}

template < typename T >
const Meta::Method::Parameter ClassID< Meta::staticarray< T > >::s_index_0_params[2]
    = {{{0},
        istring("this"),
        be_type< Meta::staticarray< T >& >(),
        {&::BugEngine::Meta::Method::Parameter::s_noDefaultValue}},
       {{0},
        istring("index"),
        be_type< u32 >(),
        {&::BugEngine::Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const Meta::Method::Parameter ClassID< Meta::staticarray< T > >::s_index_1_params[2]
    = {{{0},
        istring("this"),
        be_type< const Meta::staticarray< T >& >(),
        {&::BugEngine::Meta::Method::Parameter::s_noDefaultValue}},
       {{0},
        istring("index"),
        be_type< u32 >(),
        {&::BugEngine::Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const Meta::Method::Overload ClassID< Meta::staticarray< T > >::s_method_index_overloads[2]
    = {{{0},
        {2, s_index_0_params},
        be_type< const T& >(),
        false,
        {0, 0},
        &callStaticArrayOperatorIndexConst},
       {{0}, {2, s_index_1_params}, be_type< T& >(), false, {0, 0}, &callStaticArrayOperatorIndex}};

template < typename T >
const Meta::Method::Parameter ClassID< Meta::staticarray< T > >::s_size_params[1]
    = {{{0},
        istring("this"),
        be_type< const Meta::staticarray< T >& >(),
        {&::BugEngine::Meta::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const Meta::Method::Overload ClassID< Meta::staticarray< T > >::s_method_size_overloads[1]
    = {{{0}, {1, s_size_params}, be_type< u32 >(), false, {0, 0}, &callStaticArraySize}};

template < typename T >
const Meta::ScriptingArrayAPI ClassID< Meta::staticarray< T > >::scriptingArrayAPI
    = {value_type, &array_size, &index, &indexConst};

template < typename T >
const Meta::ScriptingAPI ClassID< Meta::staticarray< T > >::scriptingAPI = {{&scriptingArrayAPI}};

template < typename T >
BE_EXPORT raw< const Meta::Class > ClassID< Meta::staticarray< T > >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const Meta::Method s_methods[2]
        = {{Meta::Class::nameOperatorIndex(),
            {2, ClassID< Meta::staticarray< T > >::s_method_index_overloads},
            {&s_methods[0]}},
           {istring("size"),
            {1, ClassID< Meta::staticarray< T > >::s_method_size_overloads},
            {&s_methods[1]}}};
    static const ::BugEngine::Meta::Class s_class
        = {istring(minitl::format< 1024u >("staticarray<%s>") | be_type< T >().name()),
           u32(sizeof(Meta::staticarray< T >)),
           0,
           Meta::ClassType_Array,
           {be_bugengine_Namespace().m_ptr},
           {be_class< void >().m_ptr},
           {0},
           {0},
           {0, 0},
           {2, s_methods},
           {0},
           {&ClassID< Meta::staticarray< T > >::scriptingAPI},
           &Meta::wrap< Meta::staticarray< T > >::copy,
           &Meta::wrap< Meta::staticarray< T > >::destroy};
    raw< const Meta::Class > result = {&s_class};
    return result;
}

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
