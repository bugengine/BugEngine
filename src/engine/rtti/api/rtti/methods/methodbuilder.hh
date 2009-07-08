/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_RTTI_METHODS_METHODBUILDER_HH_
#define BE_RTTI_METHODS_METHODBUILDER_HH_
/*****************************************************************************/
#include    <rtti/methods/method.hh>

namespace BugEngine { namespace RTTI { namespace _
{

template< typename Owner >
struct MethodBuilder0
{
    template< void (Owner::*Func)() >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethod0<Owner, Func >(); }
};
template< typename Owner >
struct MethodBuilderC0
{
    template< void (Owner::*Func)() const >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodC0<Owner, Func >(); }
};
template< typename Owner, typename R >
struct MethodBuilderR0
{
    template< R (Owner::*Func)() >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodR0<Owner, R, Func >(); }
};
template< typename Owner, typename R >
struct MethodBuilderCR0
{
    template< R (Owner::*Func)() const >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodCR0<Owner, R, Func >(); }
};


template< typename Owner >
inline static MethodBuilder0<Owner> createMethodBuilder(void (Owner::*)())         { return MethodBuilder0<Owner>(); }
template< typename Owner >
inline static MethodBuilderC0<Owner> createMethodBuilder(void (Owner::*)() const)   { return MethodBuilderC0<Owner>(); }
template< typename Owner, typename R >
inline static MethodBuilderR0<Owner, R> createMethodBuilder(R (Owner::*)())         { return MethodBuilderR0<Owner, R>(); }
template< typename Owner, typename R >
inline static MethodBuilderCR0<Owner, R> createMethodBuilder(R (Owner::*)() const)   { return MethodBuilderCR0<Owner, R>(); }




template< typename Owner, typename T1 >
struct MethodBuilder1
{
    template< void (Owner::*Func)(T1) >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethod1<Owner, T1, Func >(); }
};
template< typename Owner, typename T1 >
struct MethodBuilderC1
{
    template< void (Owner::*Func)(T1) const >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodC1<Owner, T1, Func >(); }
};
template< typename Owner, typename R, typename T1 >
struct MethodBuilderR1
{
    template< R (Owner::*Func)(T1) >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodR1<Owner, R, T1, Func >(); }
};
template< typename Owner, typename R, typename T1 >
struct MethodBuilderCR1
{
    template< R (Owner::*Func)(T1) const >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodCR1<Owner, R, T1, Func >(); }
};

template< typename Owner, typename T1 >
inline static MethodBuilder1<Owner, T1> createMethodBuilder(void (Owner::*)(T1))         { return MethodBuilder1<Owner, T1>(); }
template< typename Owner, typename T1 >
inline static MethodBuilderC1<Owner, T1> createMethodBuilder(void (Owner::*)(T1) const)   { return MethodBuilderC1<Owner, T1>(); }
template< typename Owner, typename R, typename T1 >
inline static MethodBuilderR1<Owner, R, T1> createMethodBuilder(R (Owner::*)(T1))         { return MethodBuilderR1<Owner, R, T1>(); }
template< typename Owner, typename R, typename T1 >
inline static MethodBuilderCR1<Owner, R, T1> createMethodBuilder(R (Owner::*)(T1) const)   { return MethodBuilderCR1<Owner, R, T1>(); }




template< typename Owner, typename T1, typename T2 >
struct MethodBuilder2
{
    template< void (Owner::*Func)(T1, T2) >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethod2<Owner, T1, T2, Func >(); }
};
template< typename Owner, typename T1, typename T2 >
struct MethodBuilderC2
{
    template< void (Owner::*Func)(T1, T2) const >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodC2<Owner, T1, T2, Func >(); }
};
template< typename Owner, typename R, typename T1, typename T2 >
struct MethodBuilderR2
{
    template< R (Owner::*Func)(T1, T2) >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodR2<Owner, R, T1, T2, Func >(); }
};
template< typename Owner, typename R, typename T1, typename T2 >
struct MethodBuilderCR2
{
    template< R (Owner::*Func)(T1, T2) const >
    Object::MetaClass::Method* buildMethod() { return new ObjectMethodCR2<Owner, R, T1, T2, Func >(); }
};

template< typename Owner, typename T1, typename T2 >
inline static MethodBuilder2<Owner, T1, T2> createMethodBuilder(void (Owner::*)(T1, T2))         { return MethodBuilder2<Owner, T1, T2>(); }
template< typename Owner, typename T1, typename T2 >
inline static MethodBuilderC2<Owner, T1, T2> createMethodBuilder(void (Owner::*)(T1, T2) const)   { return MethodBuilderC2<Owner, T1, T2>(); }
template< typename Owner, typename R, typename T1, typename T2 >
inline static MethodBuilderR2<Owner, R, T1, T2> createMethodBuilder(R (Owner::*)(T1, T2))         { return MethodBuilderR2<Owner, R, T1, T2>(); }
template< typename Owner, typename R, typename T1, typename T2 >
inline static MethodBuilderCR2<Owner, R, T1, T2> createMethodBuilder(R (Owner::*)(T1, T2) const)   { return MethodBuilderCR2<Owner, R, T1, T2>(); }

}}}

/*****************************************************************************/
#endif
