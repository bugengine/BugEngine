/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_GETSET_HH_
#define BE_RTTI_ENGINE_GETSET_HH_
/*****************************************************************************/
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename T, typename Owner, T (Owner::*Field) >
static inline Value get(weak<const PropertyInfo> _this, void* from, bool isConst)
{
    if(isConst)
    {
        return Value(Value::ByRef((const Owner*)from->*Field));
    }
    else
    {
        return Value(Value::ByRef((Owner*)from->*Field));
    }
}

template< typename T, typename Owner, T (Owner::*Field) >
static inline void set(weak<const PropertyInfo> _this, void* from, Value& value, bool isConst)
{
    be_assert_recover(!isConst, "Setting property on const object", return);
    (Owner*)from->*Field = value.as<T>();
}


template< typename T, void(T::*method)() >
Value call0(Value* params, size_t paramCount)
{
    be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
    be_assert_recover(params[0].type() <= be_typeid<T>::type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
    params[0].as<T>().*method();
    return Value();
}


template< typename T, typename P1, void(T::*method)(P1) >
Value call1(Value* params, size_t paramCount)
{
    be_assert_recover(paramCount == 2, "expecting 2 parameter; got %d" | paramCount, return Value());
    be_assert_recover(params[0].type() <= be_typeid<T>::type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
    be_assert_recover(params[1].type() <= be_typeid<P1>::type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
    params[0].as<T>().*method(params[1].as<P1>());
    return Value();
}

template< typename T, typename P1, typename P2, void(T::*method)(P1, P2) >
Value call2(Value* params, size_t paramCount)
{
    be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
    be_assert_recover(params[0].type() <= be_typeid<T>::type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
    be_assert_recover(params[1].type() <= be_typeid<P1>::type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
    be_assert_recover(params[2].type() <= be_typeid<P2>::type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
    (params[0].as<T>().*method)(params[1].as<P1>(), params[2].as<P2>());
    return Value();
}

}}

/*****************************************************************************/
#endif
