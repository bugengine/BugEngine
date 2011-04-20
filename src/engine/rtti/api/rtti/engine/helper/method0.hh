/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_METHOD0_HH_
#define BE_RTTI_ENGINE_HELPER_METHOD0_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename T >
struct callhelper< T, void >
{
    enum {VarArg = 0 };

    template< void(*method)() >
    static Value callStatic(Value* /*params*/, u32 paramCount)
    {
        be_assert_recover(paramCount == 0, "expecting 0 parameter; got %d" | paramCount, return Value());
        (*method)();
        return Value();
    }

    template< void(T::*method)() >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        (params[0].as<T&>().*method)();
        return Value();
    }

    template< void(T::*method)() const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        (params[0].as<const T&>().*method)();
        return Value();
    }

    static Value constructPtr(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 0, "expecting 0 parameter; got %d" | paramCount, return Value());
        return Value(ref<T>::create(scriptArena()));
    }

    static Value construct(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 0, "expecting 0 parameter; got %d" | paramCount, return Value());
        Value v(be_typeid<T>::type(), Value::Reserve);
        new(v.memory()) T();
        return v;
    }
};

template< typename T, typename R >
struct callhelper< T, R >
{
    enum {VarArg = 0 };

    template< R(*method)() >
    static Value callStatic(Value* /*params*/, u32 paramCount)
    {
        be_assert_recover(paramCount == 0, "expecting 0 parameter; got %d" | paramCount, return Value());
        return Value((*method)());
    }
    
    template< R(T::*method)() >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        return Value((params[0].as<T&>().*method)());
    }

    template< R(T::*method)() const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        return Value((params[0].as<const T&>().*method)());
    }
};

}}

/*****************************************************************************/
#endif
