/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_INL_
#define BE_RTTI_TYPEINFO_INL_
/*****************************************************************************/
#include    <rtti/engine/classinfo.script.hh>


namespace BugEngine
{

static inline bool operator==(TypeInfo t1, TypeInfo t2)
{
    return t1.metaclass == t2.metaclass && t1.type == t2.type && t1.constness == t2.constness;
}
static inline bool operator<=(TypeInfo t1, TypeInfo t2)
{
    return     (t1.type & TypeInfo::TypeMask) == (t2.type & TypeInfo::TypeMask)
            && t1.type <= t2.type
            && t2.metaclass->isA(t1.metaclass)
            && t1.constness <= t2.constness;
}

namespace RTTI
{

template< typename T >
struct RefType
{
    typedef T Type;
    enum { Reference = TypeInfo::Class, Constness = TypeInfo::Mutable };
};

template< typename T >
struct RefType<const T>
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = RefType<T>::Reference, Constness = TypeInfo::Const };
};

template< typename T >
struct RefType< T& >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = RefType<T>::Reference, Constness = RefType<T>::Constness };
};

template< typename T >
struct RefType< ref<T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::RefPtr, Constness = TypeInfo::Mutable };
};

template< typename T >
struct RefType< ref<const T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::ConstRefPtr, Constness = TypeInfo::Mutable };
};

template< typename T >
struct RefType< weak<T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::WeakPtr, Constness = TypeInfo::Mutable };
};

template< typename T >
struct RefType< weak<const T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::ConstWeakPtr, Constness = TypeInfo::Mutable };
};

template< typename T >
struct RefType< raw<T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::RawPtr, Constness = TypeInfo::Mutable };
};

template< typename T >
struct RefType< raw<const T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::ConstRawPtr, Constness = TypeInfo::Mutable };
};

}

template< typename T >
TypeInfo be_typeid< T >::type()
{
    return TypeInfo(be_typeid<typename RTTI::RefType<T>::Type>::klass(), TypeInfo::Type(RTTI::RefType<T>::Reference), TypeInfo::Constness(RTTI::RefType<T>::Constness));
}

}

/*****************************************************************************/
#endif
