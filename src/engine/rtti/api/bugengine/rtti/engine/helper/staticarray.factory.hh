/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
#define BE_RTTI_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/helper/method.hh>
#include <bugengine/rtti/engine/helper/staticarray.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/engine/propertyinfo.script.hh>
#include <bugengine/rtti/engine/scriptingapi.hh>
#include <bugengine/rtti/typeinfo.hh>
#include <bugengine/rtti/typeinfo.script.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI {

template < typename T >
struct ClassID< RTTI::staticarray< T > >
{
    static const RTTI::Type value_type;
    static u32              array_size(const RTTI::Value& v);
    static RTTI::Value      index(RTTI::Value& v, u32 i);
    static RTTI::Value      indexConst(const RTTI::Value& v, u32 i);

    static RTTI::Value callStaticArrayOperatorIndex(RTTI::Value* params, u32 paramCount);
    static RTTI::Value callStaticArraySize(RTTI::Value* params, u32 paramCount);
    static RTTI::Value callStaticArrayOperatorIndexConst(RTTI::Value* params, u32 paramCount);

    static const RTTI::Method::Parameter s_index_0_params[2];
    static const RTTI::Method::Parameter s_index_1_params[2];
    static const RTTI::Method::Overload  s_method_index_overloads[2];
    static const RTTI::Method::Parameter s_size_params[1];
    static const RTTI::Method::Overload  s_method_size_overloads[1];
    static const RTTI::ScriptingArrayAPI scriptingArrayAPI;
    static const RTTI::ScriptingAPI      scriptingAPI;

    static BE_EXPORT raw< const RTTI::Class > klass();
};

template < typename T >
const RTTI::Type ClassID< RTTI::staticarray< T > >::value_type = be_type< T >();

template < typename T >
u32 ClassID< RTTI::staticarray< T > >::array_size(const RTTI::Value& v)
{
    return v.as< const RTTI::staticarray< T >& >().count;
}

template < typename T >
RTTI::Value ClassID< RTTI::staticarray< T > >::index(RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< RTTI::staticarray< T >& >().operator[](i)));
}

template < typename T >
RTTI::Value ClassID< RTTI::staticarray< T > >::indexConst(const RTTI::Value& v, u32 i)
{
    return RTTI::Value(RTTI::Value::ByRef(v.as< const RTTI::staticarray< T >& >().operator[](i)));
}

template < typename T >
RTTI::Value ClassID< RTTI::staticarray< T > >::callStaticArrayOperatorIndex(RTTI::Value* params,
                                                                            u32          paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< RTTI::staticarray< T >& >().operator[](params[1].as< u32 >()));
}

template < typename T >
RTTI::Value ClassID< RTTI::staticarray< T > >::callStaticArraySize(RTTI::Value* params,
                                                                   u32          paramCount)
{
    be_assert(paramCount == 1, "expected 1 parameter; received %d" | paramCount);
    return RTTI::Value(params[0].as< const RTTI::staticarray< T >& >().count);
}

template < typename T >
RTTI::Value
ClassID< RTTI::staticarray< T > >::callStaticArrayOperatorIndexConst(RTTI::Value* params,
                                                                     u32          paramCount)
{
    be_assert(paramCount == 2, "expected 2 parameter; received %d" | paramCount);
    return RTTI::Value(
       params[0].as< const RTTI::staticarray< T >& >().operator[](params[1].as< u32 >()));
}

template < typename T >
const RTTI::Method::Parameter ClassID< RTTI::staticarray< T > >::s_index_0_params[2]
   = {{{0},
       istring("this"),
       be_type< RTTI::staticarray< T >& >(),
       {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}},
      {{0},
       istring("index"),
       be_type< u32 >(),
       {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const RTTI::Method::Parameter ClassID< RTTI::staticarray< T > >::s_index_1_params[2]
   = {{{0},
       istring("this"),
       be_type< const RTTI::staticarray< T >& >(),
       {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}},
      {{0},
       istring("index"),
       be_type< u32 >(),
       {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const RTTI::Method::Overload ClassID< RTTI::staticarray< T > >::s_method_index_overloads[2] = {
   {{0}, {2, s_index_0_params}, be_type< const T& >(), false, &callStaticArrayOperatorIndexConst},
   {{0}, {2, s_index_1_params}, be_type< T& >(), false, &callStaticArrayOperatorIndex}};

template < typename T >
const RTTI::Method::Parameter ClassID< RTTI::staticarray< T > >::s_size_params[1]
   = {{{0},
       istring("this"),
       be_type< const RTTI::staticarray< T >& >(),
       {&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue}}};

template < typename T >
const RTTI::Method::Overload ClassID< RTTI::staticarray< T > >::s_method_size_overloads[1]
   = {{{0}, {1, s_size_params}, be_type< u32 >(), false, &callStaticArraySize}};

template < typename T >
const RTTI::ScriptingArrayAPI ClassID< RTTI::staticarray< T > >::scriptingArrayAPI
   = {value_type, &array_size, &index, &indexConst};

template < typename T >
const RTTI::ScriptingAPI ClassID< RTTI::staticarray< T > >::scriptingAPI = {{&scriptingArrayAPI}};

template < typename T >
BE_EXPORT raw< const RTTI::Class > ClassID< RTTI::staticarray< T > >::klass()
{
    /* work around Intel compiler issue
     * internal error: assertion failed: adjust_cleanup_state_for_aggregate_init: NULL dip
     * (shared/edgcpfe/lower_init.c, line 6280)
     */
    static const RTTI::Method s_methods[2]
       = {{RTTI::Class::nameOperatorIndex(),
           {2, ClassID< RTTI::staticarray< T > >::s_method_index_overloads},
           {&s_methods[0]}},
          {istring("size"),
           {1, ClassID< RTTI::staticarray< T > >::s_method_size_overloads},
           {&s_methods[1]}}};
    static const ::BugEngine::RTTI::Class s_class
       = {istring(minitl::format< 1024u >("staticarray<%s>") | be_type< T >().name()),
          u32(sizeof(RTTI::staticarray< T >)),
          0,
          RTTI::ClassType_Array,
          {be_bugengine_Namespace().m_ptr},
          {be_class< void >().m_ptr},
          {0},
          {0},
          {0, 0},
          {2, s_methods},
          {0},
          {&ClassID< RTTI::staticarray< T > >::scriptingAPI},
          &RTTI::wrap< RTTI::staticarray< T > >::copy,
          &RTTI::wrap< RTTI::staticarray< T > >::destroy};
    raw< const RTTI::Class > result = {&s_class};
    return result;
}

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
