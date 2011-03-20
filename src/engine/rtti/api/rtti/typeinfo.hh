/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/
#include    <rtti/typeinfo.script.hh>
#include    <minitl/type/typemanipulation.hh>

namespace BugEngine
{

template< typename T >
struct BE_EXPORT be_typeid
{
    static inline ref<const RTTI::ClassInfo>    klass() { return klassBuilder(); }
    static inline TypeInfo                      type()  { return TypeInfo(klass(), TypeInfo::Class, TypeInfo::Mutable); }
private:
    struct PropertyBuilder
    {
        PropertyBuilder();
        ~PropertyBuilder();
    };
    static ref<RTTI::ClassInfo> klassBuilder();
    static PropertyBuilder s_properties;
};

template< typename T >
struct be_api(RTTI) be_typeid< T& > : public be_typeid<T>
{
};

template< typename T >
struct be_api(RTTI) be_typeid< const T > : public be_typeid< T >
{
    static inline TypeInfo                      type()  { TypeInfo type = be_typeid<T>::type(); return TypeInfo(type.metaclass, type.type, TypeInfo::Const); }
};

template< typename T >
struct be_api(RTTI) be_typeid< const T& > : public be_typeid<const T>
{
    static inline TypeInfo                      type()  { TypeInfo type = be_typeid<T>::type(); return TypeInfo(type.metaclass, type.type, TypeInfo::Const); }
};

template< typename T >
struct be_api(RTTI) be_typeid< ref<T> >
{
    static inline ref<const RTTI::ClassInfo>    klass() { return be_typeid<T>::klass(); }
    static inline TypeInfo                      type()  { return TypeInfo(be_typeid<T>::klass(), minitl::is_const<T>::Value ? TypeInfo::ConstRefPtr : TypeInfo::RefPtr, TypeInfo::Mutable); }
};

template< typename T >
struct be_api(RTTI) be_typeid< weak<T> >
{
    static inline ref<const RTTI::ClassInfo>    klass() { return be_typeid<T>::klass(); }
    static inline TypeInfo                      type()  { return TypeInfo(be_typeid<T>::klass(), minitl::is_const<T>::Value ? TypeInfo::ConstWeakPtr : TypeInfo::WeakPtr, TypeInfo::Mutable); }
};

template< typename T >
struct be_api(RTTI) be_typeid< T* >
{
    static inline ref<const RTTI::ClassInfo>    klass() { return be_typeid<T>::klass(); }
    static inline TypeInfo                      type()  { return TypeInfo(be_typeid<T>::klass(), minitl::is_const<T>::Value ? TypeInfo::ConstRawPtr : TypeInfo::RawPtr, TypeInfo::Mutable); }
};

template< > ref<RTTI::ClassInfo> be_typeid< void >::klassBuilder();

}

/*****************************************************************************/
#endif
