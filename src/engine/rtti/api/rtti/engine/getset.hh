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

template< typename T, T* object >
static inline Value staticget(weak<const PropertyInfo> _this, void* from, bool isConst)
{
    return Value(*object);
}

template< typename T >
static inline Value call(Value& _this, Value* params, u32 paramCount)
{
    return _this.as<const T&>()(params, paramCount);
}

template< typename T, typename Owner, T (Owner::*Field) >
static inline void set(weak<const PropertyInfo> _this, void* from, Value& value, bool isConst)
{
    be_assert_recover(!isConst, "Setting property on const object", return);
    (Owner*)from->*Field = value.as<T>();
}

template<   typename T,
            typename R,
            typename P1 = void,
            typename P2 = void,
            typename P3 = void,
            typename P4 = void,
            typename P5 = void,
            typename P6 = void,
            typename P7 = void,
            typename P8 = void,
            typename P9 = void >
struct callhelper;

template< typename T >
struct callhelper< T, void >
{
    enum {VarArg = 0 };
    template< void(T::*method)() >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        (params[0].as<T*>()->*method)();
        return Value();
    }
    template< void(T::*method)() const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        (params[0].as<const T*>()->*method)();
        return Value();
    }
};

template< typename T, typename R >
struct callhelper< T, R >
{
    enum {VarArg = 0 };
    template< R(T::*method)() >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        return Value((params[0].as<T*>()->*method)());
    }
    template< R(T::*method)() const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 1, "expecting 1 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        return Value((params[0].as<const T*>()->*method)());
    }
};


template< typename T, typename P1 >
struct callhelper< T, void, P1 >
{
    enum {VarArg = 0 };
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



template< typename T, typename P1, typename P2 >
struct callhelper< T, void, P1, P2 >
{
    enum {VarArg = 0 };
    template< void(T::*method)(P1, P2) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        (params[0].as<T*>()->*method)(params[1].as<P1>(), params[2].as<P2>());
        return Value();
    }
    template< void(T::*method)(P1, P2) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        (params[0].as<const T*>()->*method)(params[1].as<P1>(), params[2].as<P2>());
        return Value();
    }
};

template< typename T, typename R, typename P1, typename P2 >
struct callhelper< T, R, P1, P2 >
{
    enum {VarArg = 0 };
    template< R(T::*method)(P1, P2) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        return Value((params[0].as<T*>()->*method)(params[1].as<P1>(), params[2].as<P2>()));
    }
    template< R(T::*method)(P1, P2) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(paramCount == 3, "expecting 3 parameter; got %d" | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        be_assert_recover(be_typeid<P1>::type() <= params[1].type(), "expected parameter of type %s; got %s" | be_typeid<P1>::type().name() | params[1].type().name(), return Value());
        be_assert_recover(be_typeid<P2>::type() <= params[2].type(), "expected parameter of type %s; got %s" | be_typeid<P2>::type().name() | params[2].type().name(), return Value());
        return Value((params[0].as<const T*>()->*method)(params[1].as<P1>(), params[2].as<P2>()));
    }
};

template< typename T >
struct callhelper< T, Value, Value*, u32 >
{
    enum {VarArg = 1 };
    template< Value(T::*method)(Value*, u32) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        return (params[0].as<T*>()->*method)(params + 1, paramCount - 1);
    }
    template< Value(T::*method)(Value*, u32) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        return (params[0].as<const T*>()->*method)(params + 1, paramCount - 1);
    }
};

}}

/*****************************************************************************/
#endif
