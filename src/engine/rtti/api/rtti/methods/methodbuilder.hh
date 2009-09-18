/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_METHODS_METHODBUILDER_HH_
#define BE_RTTI_METHODS_METHODBUILDER_HH_
/*****************************************************************************/
#include    <rtti/methods/method.hh>

namespace BugEngine { namespace RTTI { namespace _
{


template< typename Owner, typename R >
struct MethodBuilderR0
{
    template< R (Owner::*Func)() >
    Method* buildMethod() { return new MethodR0<Owner, R, Func >(); }
};
template< typename Owner, typename R >
struct MethodBuilderCR0
{
    template< R (Owner::*Func)() const >
    Method* buildMethod() { return new MethodCR0<Owner, R, Func >(); }
};

template< typename Owner, typename R >
inline static MethodBuilderR0<Owner, R> createMethodBuilder(R (Owner::*)())         { return MethodBuilderR0<Owner, R>(); }
template< typename Owner, typename R >
inline static MethodBuilderCR0<Owner, R> createMethodBuilder(R (Owner::*)() const)   { return MethodBuilderCR0<Owner, R>(); }




template< typename Owner, typename R, typename T1 >
struct MethodBuilderR1
{
    template< R (Owner::*Func)(T1) >
    Method* buildMethod() { return new MethodR1<Owner, R, T1, Func >(); }
};
template< typename Owner, typename R, typename T1 >
struct MethodBuilderCR1
{
    template< R (Owner::*Func)(T1) const >
    Method* buildMethod() { return new MethodCR1<Owner, R, T1, Func >(); }
};

template< typename Owner, typename R, typename T1 >
inline static MethodBuilderR1<Owner, R, T1> createMethodBuilder(R (Owner::*)(T1))         { return MethodBuilderR1<Owner, R, T1>(); }
template< typename Owner, typename R, typename T1 >
inline static MethodBuilderCR1<Owner, R, T1> createMethodBuilder(R (Owner::*)(T1) const)   { return MethodBuilderCR1<Owner, R, T1>(); }




template< typename Owner, typename R, typename T1, typename T2 >
struct MethodBuilderR2
{
    template< R (Owner::*Func)(T1, T2) >
    Method* buildMethod() { return new MethodR2<Owner, R, T1, T2, Func >(); }
};
template< typename Owner, typename R, typename T1, typename T2 >
struct MethodBuilderCR2
{
    template< R (Owner::*Func)(T1, T2) const >
    Method* buildMethod() { return new MethodCR2<Owner, R, T1, T2, Func >(); }
};

template< typename Owner, typename R, typename T1, typename T2 >
inline static MethodBuilderR2<Owner, R, T1, T2> createMethodBuilder(R (Owner::*)(T1, T2))         { return MethodBuilderR2<Owner, R, T1, T2>(); }
template< typename Owner, typename R, typename T1, typename T2 >
inline static MethodBuilderCR2<Owner, R, T1, T2> createMethodBuilder(R (Owner::*)(T1, T2) const)   { return MethodBuilderCR2<Owner, R, T1, T2>(); }

}}}

/*****************************************************************************/
#endif
