/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_METHOD3_HH_
#define BE_RTTI_ENGINE_HELPER_METHOD3_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{


template< typename T, typename P1, typename P2, typename P3 >
struct callhelper< T, void, P1, P2, P3 >
{
    enum {VarArg = 0 };

    template< void(*method)(P1, P2, P3) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[2].type().name(), return Value());
        (*method)(params[0].as<P1>(), params[1].as<P2>(), params[2].as<P3>());
        return Value();
    }

    template< void(T::*method)(P1, P2, P3) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 4, "expecting 4 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[3].type().name(), return Value());
        (params[0].as<T&>().*method)(params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>());
        return Value();
    }

    template< void(T::*method)(P1, P2, P3) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 4, "expecting 4 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[3].type().name(), return Value());
        (params[0].as<const T&>().*method)(params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>());
        return Value();
    }

    static Value constructPtr(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[2].type().name(), return Value());
        return Value(ref<T>::create(scriptArena(), params[0].as<P1>(), params[1].as<P2>(), params[2].as<P3>()));
    }

    static Value construct(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[2].type().name(), return Value());
        Value v(be_typeid<T>::type(), Value::Reserve);
        new(v.memory()) T(params[0].as<P1>(), params[1].as<P2>(), params[2].as<P3>());
        return v;
    }
};

template< typename T, typename R, typename P1, typename P2, typename P3 >
struct callhelper< T, R, P1, P2, P3 >
{
    enum {VarArg = 0 };

    template< R(*method)(P1, P2, P3) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[2].type().name(), return Value());
        return Value((*method)(params[0].as<P1>(), params[1].as<P2>(), params[2].as<P3>()));
    }

    template< R(T::*method)(P1, P2, P3) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 4, "expecting 4 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[3].type().name(), return Value());
        return Value((params[0].as<T&>().*method)(params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>()));
    }

    template< R(T::*method)(P1, P2, P3) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 4, "expecting 4 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        be_assert_recover(be_typeid<P3>::type() <= params[3].type(), "expected parameter of type %s; got %s" | be_typeid<P3>::type().name() | params[3].type().name(), return Value());
        return Value((params[0].as<const T&>().*method)(params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>()));
    }
};


}}

/*****************************************************************************/
#endif
