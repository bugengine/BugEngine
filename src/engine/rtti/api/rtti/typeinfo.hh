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
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable); }
};

template< typename T >
struct be_typeid<const T> : public be_typeid<T>
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::Value, RTTI::Type::Const, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< T& >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< const T& >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::Value, RTTI::Type::Const, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< ref<T> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::RefPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Mutable); }
};

template< typename T >
struct be_typeid< weak<T> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::WeakPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Mutable); }
};

template< typename T >
struct be_typeid< raw<T> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::RawPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Mutable); }
};

template< typename T >
struct be_typeid< T* >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::RawPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Mutable); }
};


template< typename T >
struct be_typeid< ref<T> const >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::RefPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< weak<T> const >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::WeakPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< raw<T> const >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::RawPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< T* const >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::klass(), RTTI::Type::RawPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< scoped<T> >
{
};

template< > BE_EXPORT raw<const RTTI::Class> be_typeid< void >::klass();

}

/*****************************************************************************/
#endif
