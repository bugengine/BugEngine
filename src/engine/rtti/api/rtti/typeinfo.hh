/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/
#include    <rtti/classinfo.script.hh>
#include    <rtti/typeinfo.script.hh>
#include    <minitl/type/typemanipulation.hh>

namespace BugEngine
{

template< typename T >
struct be_typeid
{
    static BE_EXPORT raw<const RTTI::Class> klass();
    static inline Type  type()  { return Type::makeType(klass(), Type::Value, Type::Mutable, , Type::Mutable); }
};

template< typename T >
struct be_typeid<const T>
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), Type::Value, Type::Const, Type::Const); }
};

template< typename T >
struct be_typeid< T& >
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), Type::Value, Type::Mutable, Type::Mutable); }
};

template< typename T >
struct be_typeid< const T& >
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), Type::Value, Type::Const, Type::Const; }
};

template< typename T >
struct be_typeid< ref<T> >
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), Type::RefPtr, minitl::is_const<T>::Value ? Type::Const : Type::Mutable); }
};

template< typename T >
struct be_typeid< weak<T> >
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), Type::WeakPtr, minitl::is_const<T>::Value ? Type::Const : Type::Mutable); }
};

template< typename T >
struct be_typeid< raw<T> >
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), Type::RawPtr, minitl::is_const<T>::Value ? Type::Const : Type::Mutable); }
};

template< typename T >
struct be_typeid< T* >
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), Type::RawPtr, minitl::is_const<T>::Value ? Type::Const : Type::Mutable); }
};

template< > raw<const RTTI::Class> be_typeid< void >::klass();

}

/*****************************************************************************/
#endif
