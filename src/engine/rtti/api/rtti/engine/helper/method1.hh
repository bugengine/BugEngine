/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_METHOD1_HH_
#define BE_RTTI_ENGINE_HELPER_METHOD1_HH_
/*****************************************************************************/
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{


template< typename T, typename P1 >
struct callhelper< T, void, P1 >
{
    enum {VarArg = 0 };

    template< void(*method)(P1) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        (*method)(params[0].as<P1>());
        return Value();
    }

    template< void(T::*method)(P1) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 2, "expecting 2 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        (params[0].as<T*>()->*method)(params[1].as<P1>());
        return Value();
    }

    template< void(T::*method)(P1) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 2, "expecting 2 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        (params[0].as<const T*>()->*method)(params[1].as<P1>());
        return Value();
    }
};

template< typename T, typename R, typename P1 >
struct callhelper< T, R, P1 >
{
    enum {VarArg = 0 };

    template< R(*method)(P1) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        return Value((*method)(params[0].as<P1>()));
    }

    template< R(T::*method)(P1) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 2, "expecting 2 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        return Value((params[0].as<T*>()->*method)(params[1].as<P1>()));
    }

    template< R(T::*method)(P1) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 2, "expecting 2 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        return Value((params[0].as<const T*>()->*method)(params[1].as<P1>()));
    }
};


}}

/*****************************************************************************/
#endif
