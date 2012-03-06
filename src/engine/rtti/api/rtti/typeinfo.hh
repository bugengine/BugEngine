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
    static inline Type  type()  { return Type::makeType(klass(), Type::Class, Type::Mutable); }
};

template< typename T >
struct be_typeid< T& > : public be_typeid<T>
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), Type::Class, Type::Mutable); }
};

template< typename T >
struct be_typeid< const T > : public be_typeid<T>
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::type(), Type::MakeConst); }
};

template< typename T >
struct be_typeid< const T& > : public be_typeid<T>
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::type(), Type::MakeConst); }
};

template< typename T >
struct be_typeid< ref<T> > : public be_typeid<T>
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), minitl::is_const<T>::Value ? Type::ConstRefPtr : Type::RefPtr, Type::Mutable); }
};

template< typename T >
struct be_typeid< weak<T> > : public be_typeid<T>
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), minitl::is_const<T>::Value ? Type::ConstWeakPtr : Type::WeakPtr, Type::Mutable); }
};

template< typename T >
struct be_typeid< raw<T> > : public be_typeid<T>
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), minitl::is_const<T>::Value ? Type::ConstRawPtr : Type::RawPtr, Type::Mutable); }
};

template< typename T >
struct be_typeid< T* > : public be_typeid<T>
{
    static inline Type  type()  { return Type::makeType(be_typeid<T>::klass(), minitl::is_const<T>::Value ? Type::ConstRawPtr : Type::RawPtr, Type::Mutable); }
};

template< > raw<const RTTI::Class> be_typeid< void >::klass();

}

/*****************************************************************************/
#endif
