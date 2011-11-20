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
    static BE_EXPORT raw<const RTTI::ClassInfo> klass();
    static inline TypeInfo  type()  { return TypeInfo::makeType(klass(), TypeInfo::Class, TypeInfo::Mutable); }
};

template< typename T >
struct be_typeid< T& > : public be_typeid<T>
{
    static inline TypeInfo  type()  { return TypeInfo::makeType(be_typeid<T>::klass(), TypeInfo::Class, TypeInfo::Mutable); }
};

template< typename T >
struct be_typeid< const T > : public be_typeid<T>
{
    static inline TypeInfo  type()  { return TypeInfo::makeType(be_typeid<T>::type(), TypeInfo::Constify); }
};

template< typename T >
struct be_typeid< const T& > : public be_typeid<T>
{
    static inline TypeInfo  type()  { return TypeInfo::makeType(be_typeid<T>::type(), TypeInfo::Constify); }
};

template< typename T >
struct be_typeid< ref<T> > : public be_typeid<T>
{
    static inline TypeInfo  type()  { return TypeInfo::makeType(be_typeid<T>::klass(), minitl::is_const<T>::Value ? TypeInfo::ConstRefPtr : TypeInfo::RefPtr, TypeInfo::Mutable); }
};

template< typename T >
struct be_typeid< weak<T> > : public be_typeid<T>
{
    static inline TypeInfo  type()  { return TypeInfo::makeType(be_typeid<T>::klass(), minitl::is_const<T>::Value ? TypeInfo::ConstWeakPtr : TypeInfo::WeakPtr, TypeInfo::Mutable); }
};

template< typename T >
struct be_typeid< raw<T> > : public be_typeid<T>
{
    static inline TypeInfo  type()  { return TypeInfo::makeType(be_typeid<T>::klass(), minitl::is_const<T>::Value ? TypeInfo::ConstRawPtr : TypeInfo::RawPtr, TypeInfo::Mutable); }
};

template< typename T >
struct be_typeid< T* > : public be_typeid<T>
{
    static inline TypeInfo  type()  { return TypeInfo::makeType(be_typeid<T>::klass(), minitl::is_const<T>::Value ? TypeInfo::ConstRawPtr : TypeInfo::RawPtr, TypeInfo::Mutable); }
};

template< > raw<const RTTI::ClassInfo> be_typeid< void >::klass();

}

/*****************************************************************************/
#endif
