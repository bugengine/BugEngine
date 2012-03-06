/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_INL_
#define BE_RTTI_TYPEINFO_INL_
/*****************************************************************************/
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>


namespace BugEngine
{

static inline bool operator==(Type t1, Type t2)
{
    return t1.metaclass == t2.metaclass && t1.indirection == t2.indirection && t1.constness == t2.constness;
}
static inline bool operator<=(Type t1, Type t2)
{
    return     (t1.indirection & Type::IndirectionMask) <= (t2.indirection & Type::IndirectionMask)
            && t1.indirection <= t2.indirection
            && t2.metaclass->isA(t1.metaclass)
            && t1.constness <= t2.constness;
}

namespace RTTI
{

template< typename T >
struct RefType
{
    typedef T Type;
    enum { Reference = Type::Class, Constness = Type::Mutable };
};

template< typename T >
struct RefType<const T>
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = RefType<T>::Reference, Constness = Type::Const };
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
    enum { Reference = Type::RefPtr, Constness = Type::Mutable };
};

template< typename T >
struct RefType< ref<const T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = Type::ConstRefPtr, Constness = Type::Mutable };
};

template< typename T >
struct RefType< weak<T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = Type::WeakPtr, Constness = Type::Mutable };
};

template< typename T >
struct RefType< weak<const T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = Type::ConstWeakPtr, Constness = Type::Mutable };
};

template< typename T >
struct RefType< raw<T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = Type::RawPtr, Constness = Type::Mutable };
};

template< typename T >
struct RefType< raw<const T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = Type::ConstRawPtr, Constness = Type::Mutable };
};

}

template< typename T >
struct be_typeid< minitl::vector<T> >
{
    static const RTTI::Class klass;
    static inline Type  type()  { raw<const RTTI::Class> ci = {&klass};  return Type::makeType(ci, Type::Class, Type::Mutable); }
};
template< typename T >
const RTTI::Class be_typeid< minitl::vector<T> >::klass =
{
    "vector", {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }}
};

template< typename T, size_t SIZE >
struct be_typeid< minitl::array<T,SIZE> >
{
    static const RTTI::Class klass;
    static inline Type  type()  { raw<const RTTI::Class> ci = {&klass};  return Type::makeType(ci, Type::Class, Type::Mutable); }
};
template< typename T, size_t SIZE >
const RTTI::Class be_typeid< minitl::array<T,SIZE> >::klass =
{
    "array", {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }}
};

template< typename T1, typename T2, typename T3 >
struct be_typeid< minitl::hashmap<T1, T2, T3> >
{
    static const RTTI::Class klass;
    static inline Type  type()  { raw<const RTTI::Class> ci = {&klass};  return Type::makeType(ci, Type::Class, Type::Mutable); }
};
template< typename T1, typename T2, typename T3 >
const RTTI::Class be_typeid< minitl::hashmap<T1, T2, T3> >::klass =
{
    "hashmap", {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }}
};

}

/*****************************************************************************/
#endif
