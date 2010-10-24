/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_INL_
#define BE_RTTI_TYPEINFO_INL_
/*****************************************************************************/

namespace BugEngine
{

static inline bool operator==(TypeInfo t1, TypeInfo t2)
{
    return t1.metaclass == t2.metaclass && t1.type == t2.type && t1.constness == t2.constness;
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
    typedef T Type;
    enum { Reference = TypeInfo::Class, Constness = TypeInfo::Const };
};
template< typename T >
struct RefType< T& >
{
    typedef T Type;
    enum { Reference = TypeInfo::Class, Constness = RefType<T>::Constness };
};

template< typename T >
struct RefType< ref<T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::RefPtr, Constness = RefType<T>::Constness };
};

template< typename T >
struct RefType< weak<T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::WeakPtr, Constness = RefType<T>::Constness };
};

template< typename T >
struct RefType< raw<T> >
{
    typedef typename RefType<T>::Type Type;
    enum { Reference = TypeInfo::RawPtr, Constness = RefType<T>::Constness };
};


}

template< typename T >
struct be_typeid< const T >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::Class, TypeInfo::Const }; return i; }
};

template< typename T >
struct be_typeid< T& >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::Reference, TypeInfo::Mutable }; return i; }
};

template< typename T >
struct be_typeid< const T& >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::Reference, TypeInfo::Const }; return i; }
};

template< typename T >
struct be_typeid< raw<T> >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::RawPtr, TypeInfo::Mutable }; return i; }
};

template< typename T >
struct be_typeid< raw<const T> >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::RawPtr, TypeInfo::Const }; return i; }
};

template< typename T >
struct be_typeid< ref<T> >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::RefPtr, TypeInfo::Mutable }; return i; }
};

template< typename T >
struct be_typeid< ref<const T> >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::RefPtr, TypeInfo::Const }; return i; }
};

template< typename T >
struct be_typeid< weak<T> >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::WeakPtr, TypeInfo::Mutable }; return i; }
};

template< typename T >
struct be_typeid< weak<const T> >
{
    static inline TypeInfo type() { TypeInfo i = { be_typeid<T>::klass, TypeInfo::WeakPtr, TypeInfo::Const }; return i; }
};

}

/*****************************************************************************/
#endif
