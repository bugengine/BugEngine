/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_METHOD6_HH_
#define BE_RTTI_ENGINE_HELPER_METHOD6_HH_
/*****************************************************************************/
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
struct callhelper< T, void, P1, P2, P3, P4, P5, P6 >
{
    enum {VarArg = 0 };

    template< void(*method)(P1, P2, P3, P4, P5, P6) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 6, "expecting 6 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P4>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P4>::type().name() | params[3].type().name(), return Value());
        be_assert_recover(be_typeid<P5>::type() <= params[4].type(), "expected parameter of type %s; got %s" | be_typeid<P5>::type().name() | params[4].type().name(), return Value());
        be_assert_recover(be_typeid<P6>::type() <= params[5].type(), "expected parameter of type %s; got %s" | be_typeid<P6>::type().name() | params[5].type().name(), return Value());
        (*method)(params[0].as<P1>(), params[1].as<P2>(), params[2].as<P3>(), params[3].as<P4>(), params[4].as<P5>(), params[5].as<P6>());
        return Value();
    }

    template< void(T::*method)(P1, P2, P3, P4, P5, P6) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 7, "expecting 7 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[3].type().name(), return Value());
        be_assert_recover(be_typeid<P4>::type() <= params[4].type(), "expected parameter of type %s; got %s" | be_typeid<P4>::type().name() | params[4].type().name(), return Value());
        be_assert_recover(be_typeid<P5>::type() <= params[5].type(), "expected parameter of type %s; got %s" | be_typeid<P5>::type().name() | params[5].type().name(), return Value());
        be_assert_recover(be_typeid<P6>::type() <= params[6].type(), "expected parameter of type %s; got %s" | be_typeid<P6>::type().name() | params[6].type().name(), return Value());
        (params[0].as<T*>()->*method)(params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>(), params[4].as<P4>(), params[5].as<P5>(), params[6].as<P6>());
        return Value();
    }

    template< void(T::*method)(P1, P2, P3, P4, P5, P6) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 7, "expecting 7 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[3].type().name(), return Value());
        be_assert_recover(be_typeid<P4>::type() <= params[4].type(), "expected parameter of type %s; got %s" | be_typeid<P4>::type().name() | params[4].type().name(), return Value());
        be_assert_recover(be_typeid<P5>::type() <= params[5].type(), "expected parameter of type %s; got %s" | be_typeid<P5>::type().name() | params[5].type().name(), return Value());
        be_assert_recover(be_typeid<P6>::type() <= params[6].type(), "expected parameter of type %s; got %s" | be_typeid<P6>::type().name() | params[6].type().name(), return Value());
        (params[0].as<const T*>()->*method)(params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>(), params[4].as<P4>(), params[5].as<P5>(), params[6].as<P6>());
        return Value();
    }
};

template< typename T, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
struct callhelper< T, R, P1, P2, P3, P4, P5, P6 >
{
    enum {VarArg = 0 };

    template< R(*method)(P1, P2, P3, P4, P5, P6) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 6, "expecting 6 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P4>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P4>::type().name() | params[3].type().name(), return Value());
        be_assert_recover(be_typeid<P5>::type() <= params[4].type(), "expected parameter of type %s; got %s" | be_typeid<P5>::type().name() | params[4].type().name(), return Value());
        be_assert_recover(be_typeid<P6>::type() <= params[5].type(), "expected parameter of type %s; got %s" | be_typeid<P6>::type().name() | params[5].type().name(), return Value());
        return Value((*method)(params[0].as<P1>(), params[1].as<P2>(), params[2].as<P3>(), params[3].as<P4>(), params[4].as<P5>(), params[5].as<P6>()));
    }

    template< R(T::*method)(P1, P2, P3, P4, P5, P6) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 7, "expecting 7 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[3].type().name(), return Value());
        be_assert_recover(be_typeid<P4>::type() <= params[4].type(), "expected parameter of type %s; got %s" | be_typeid<P4>::type().name() | params[4].type().name(), return Value());
        be_assert_recover(be_typeid<P5>::type() <= params[5].type(), "expected parameter of type %s; got %s" | be_typeid<P5>::type().name() | params[5].type().name(), return Value());
        be_assert_recover(be_typeid<P6>::type() <= params[6].type(), "expected parameter of type %s; got %s" | be_typeid<P6>::type().name() | params[6].type().name(), return Value());
        return Value((params[0].as<T*>()->*method)(params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>(), params[4].as<P4>(), params[5].as<P5>(), params[6].as<P6>()));
    }

    template< R(T::*method)(P1, P2) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 7, "expecting 7 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[3].type().name(), return Value());
        be_assert_recover(be_typeid<P4>::type() <= params[4].type(), "expected parameter of type %s; got %s" | be_typeid<P4>::type().name() | params[4].type().name(), return Value());
        be_assert_recover(be_typeid<P5>::type() <= params[5].type(), "expected parameter of type %s; got %s" | be_typeid<P5>::type().name() | params[5].type().name(), return Value());
        be_assert_recover(be_typeid<P6>::type() <= params[6].type(), "expected parameter of type %s; got %s" | be_typeid<P6>::type().name() | params[6].type().name(), return Value());
        return Value((params[0].as<const T*>()->*method)(params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>(), params[4].as<P4>(), params[5].as<P5>(), params[6].as<P6>()));
    }
};

}}

/*****************************************************************************/
#endif
