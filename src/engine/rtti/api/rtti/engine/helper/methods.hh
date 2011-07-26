/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_METHOD_PARAMS
# error don't include directly
#endif
/*****************************************************************************/


namespace BugEngine { namespace RTTI
{

#define BE_LPAREN   (
#define BE_RPAREN   )

template< typename T, typename R BE_COMMA(BE_METHOD_PARAMS) BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, typename P) >
struct functionhelper< T, R BE_COMMA(BE_METHOD_PARAMS) BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, P) >
{
    enum {VarArg = 0 };

    template< R(*method)(BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, P)) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        be_forceuse(params);
        be_forceuse(paramCount);
        be_assert_recover(paramCount == BE_METHOD_PARAMS, "expecting %d parameter; got %d" | BE_METHOD_PARAMS | paramCount, return Value());
        BE_LIST5(BE_METHOD_PARAMS, , be_assert_recover BE_LPAREN be_typeid<P, >::type() <= params[, -1].type() BE_COMMA_1 "expected parameter of type %s; got %s" | be_typeid<P, >::type().name() | params[, -1].type().name() BE_COMMA_1 return Value() BE_RPAREN;);
        return Value((*method)(BE_LIST3(BE_METHOD_PARAMS, BE_COMMA_1, params[, -1].as<P, >())));
    }

    template< R(T::*method)(BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, P)) >
    static Value call(Value* params, u32 paramCount)
    {
        be_forceuse(params);
        be_forceuse(paramCount);
        be_assert_recover(paramCount == BE_METHOD_PARAMS+1, "expecting %d parameter; got %d" | BE_METHOD_PARAMS+1 | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        BE_LIST5(BE_METHOD_PARAMS, , be_assert_recover BE_LPAREN be_typeid<P, >::type() <= params[, -1].type() BE_COMMA_1 "expected parameter of type %s; got %s" | be_typeid<P, >::type().name() | params[, -1].type().name() BE_COMMA_1 return Value() BE_RPAREN;);
        return Value((params[0].as<T&>().*method)(BE_LIST3(BE_METHOD_PARAMS, BE_COMMA_1, params[, ].as<P, >())));
    }

    template< R(T::*method)(BE_LIST(BE_METHOD_PARAMS,BE_COMMA_1,  P)) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_forceuse(params);
        be_forceuse(paramCount);
        be_assert_recover(paramCount == BE_METHOD_PARAMS+1, "expecting %d parameter; got %d" | BE_METHOD_PARAMS+1 | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<const T*>::type().name() | params[0].type().name(), return Value());
        BE_LIST5(BE_METHOD_PARAMS, , be_assert_recover BE_LPAREN be_typeid<P, >::type() <= params[, -1].type() BE_COMMA_1 "expected parameter of type %s; got %s" | be_typeid<P, >::type().name() | params[, -1].type().name() BE_COMMA_1 return Value() BE_RPAREN;);
        return Value((params[0].as<const T&>().*method)(BE_LIST3(BE_METHOD_PARAMS, BE_COMMA_1, params[, ].as<P, >())));
    }
};


template< typename T BE_COMMA(BE_METHOD_PARAMS) BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, typename P) >
struct procedurehelper< T BE_COMMA(BE_METHOD_PARAMS) BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, P) >
{
    enum {VarArg = 0 };

    template< void(*method)(BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, P)) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        be_forceuse(params);
        be_forceuse(paramCount);
        be_assert_recover(paramCount == BE_METHOD_PARAMS, "expecting %d parameter; got %d" | BE_METHOD_PARAMS | paramCount, return Value());
        BE_LIST5(BE_METHOD_PARAMS, , be_assert_recover BE_LPAREN be_typeid<P, >::type() <= params[, -1].type() BE_COMMA_1 "expected parameter of type %s; got %s" | be_typeid<P, >::type().name() | params[,-1].type().name() BE_COMMA_1 return Value() BE_RPAREN;);
        (*method)(BE_LIST3(BE_METHOD_PARAMS, BE_COMMA_1, params[, -1].as<P, >()));
        return Value();
    }

    template< void(T::*method)(BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, P)) >
    static Value call(Value* params, u32 paramCount)
    {
        be_forceuse(params);
        be_forceuse(paramCount);
        be_assert_recover(paramCount == BE_METHOD_PARAMS+1, "expecting %d parameter; got %d" | BE_METHOD_PARAMS+1 | paramCount, return Value());
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T>::type().name() | params[0].type().name(), return Value());
        BE_LIST5(BE_METHOD_PARAMS, , be_assert_recover BE_LPAREN be_typeid<P, >::type() <= params[,].type() BE_COMMA_1 "expected parameter of type %s; got %s" | be_typeid<P, >::type().name() | params[,].type().name() BE_COMMA_1 return Value() BE_RPAREN;);
        (params[0].as<T&>().*method)(BE_LIST3(BE_METHOD_PARAMS, BE_COMMA_1, params[, ].as<P, >()));
        return Value();
    }

    template< void(T::*method)(BE_LIST(BE_METHOD_PARAMS, BE_COMMA_1, P)) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_forceuse(params);
        be_forceuse(paramCount);
        be_assert_recover(paramCount == BE_METHOD_PARAMS+1, "expecting %d parameter; got %d" | BE_METHOD_PARAMS+1 | paramCount, return Value());
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<const T*>::type().name() | params[0].type().name(), return Value());
        BE_LIST5(BE_METHOD_PARAMS, , be_assert_recover BE_LPAREN be_typeid<P, >::type() <= params[,].type() BE_COMMA_1 "expected parameter of type %s; got %s" | be_typeid<P, >::type().name() | params[,].type().name() BE_COMMA_1 return Value() BE_RPAREN;);
        (params[0].as<const T&>().*method)(BE_LIST3(BE_METHOD_PARAMS, BE_COMMA_1, params[, ].as<P, >()));
        return Value();
    }

    static Value constructPtr(Value* params, u32 paramCount)
    {
        be_forceuse(params);
        be_forceuse(paramCount);
        be_assert_recover(paramCount == BE_METHOD_PARAMS, "expecting %d parameter; got %d" | BE_METHOD_PARAMS | paramCount, return Value());
        BE_LIST5(BE_METHOD_PARAMS, , be_assert_recover BE_LPAREN be_typeid<P, >::type() <= params[, -1].type() BE_COMMA_1 "expected parameter of type %s; got %s" | be_typeid<P, >::type().name() | params[, -1].type().name() BE_COMMA_1 return Value() BE_RPAREN;);
        return Value(ref<T>::create(scriptArena() BE_COMMA(BE_METHOD_PARAMS) BE_LIST3(BE_METHOD_PARAMS, BE_COMMA_1, params[, -1].as<P, >())));
    }

    static Value construct(Value* params, u32 paramCount)
    {
        be_forceuse(params);
        be_forceuse(paramCount);
        be_assert_recover(paramCount == BE_METHOD_PARAMS, "expecting %d parameter; got %d" | BE_METHOD_PARAMS | paramCount, return Value());
        BE_LIST5(BE_METHOD_PARAMS, , be_assert_recover BE_LPAREN be_typeid<P, >::type() <= params[, -1].type() BE_COMMA_1 "expected parameter of type %s; got %s" | be_typeid<P, >::type().name() | params[, -1].type().name() BE_COMMA_1 return Value() BE_RPAREN;);
        Value v(be_typeid<T>::type(), Value::Reserve);
        new(v.memory()) T(BE_LIST3(BE_METHOD_PARAMS, BE_COMMA_1, params[, -1].as<P, >()));
        return v;
    }
};

}}

/*****************************************************************************/
