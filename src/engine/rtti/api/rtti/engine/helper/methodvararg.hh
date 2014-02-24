/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_METHODVARARG_HH_
#define BE_RTTI_ENGINE_HELPER_METHODVARARG_HH_
/**************************************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename T >
struct functionhelper< T, Value, Value*, u32 >
{
    enum {VarArg = 1 };
    template< Value(*method)(Value*, u32) >
    static Value callStatic(Value* params, u32 paramCount)
    {
        return (*method)(params, paramCount);
    }
    template< Value(T::*method)(Value*, u32) >
    static Value call(Value* params, u32 paramCount)
    {
        be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T*>::type().name() | params[0].type().name(), return Value());
        return (params[0].as<T&>().*method)(params + 1, paramCount - 1);
    }
    template< Value(T::*method)(Value*, u32) const >
    static Value callConst(Value* params, u32 paramCount)
    {
        be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<const T*>::type().name() | params[0].type().name(), return Value());
        return (params[0].as<const T&>().*method)(params + 1, paramCount - 1);
    }
};


}}

/**************************************************************************************************/
#endif
