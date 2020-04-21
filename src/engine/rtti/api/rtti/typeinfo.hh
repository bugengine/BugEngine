/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.script.hh>
#include    <minitl/typemanipulation.hh>


namespace BugEngine { namespace RTTI
{

struct Class;
struct Property;
struct Method;

}}

template< typename T > static inline BugEngine::RTTI::Type be_type();
template< typename T > static inline raw<const BugEngine::RTTI::Class> be_class();

namespace BugEngine { namespace RTTI
{

template< typename T >
struct ClassID
{
    static BE_EXPORT raw<const RTTI::Class> klass();
};

template< typename T >
struct TypeID
{
    static inline RTTI::Type type()
    {
        return RTTI::Type::makeType(ClassID<T>::klass(),
                                    RTTI::Type::Value,
                                    RTTI::Type::Mutable,
                                    RTTI::Type::Mutable);
    }
};

template< typename T >
struct TypeID<const T> : public TypeID<T>
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::Value,
                                    RTTI::Type::Const,
                                    RTTI::Type::Const);
    }
};

template< typename T >
struct TypeID< T& >
{
    static inline RTTI::Type  type()
    {
        RTTI::Type t = TypeID<T>::type();
        t.access = RTTI::Type::Mutable;
        t.constness = RTTI::Type::Const;
        return t;
    }
};

template< typename T >
struct TypeID< const T& >
{
    static inline RTTI::Type  type()
    {
        RTTI::Type t = TypeID<T>::type();
        t.access = RTTI::Type::Const;
        return t;
    }
};

template< typename T >
struct TypeID< ref<T> >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::RefPtr,
                                    minitl::is_const<T>::Value
                                        ?   RTTI::Type::Const
                                        :   RTTI::Type::Mutable,
                                    RTTI::Type::Mutable); }
};

template< typename T >
struct TypeID< weak<T> >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::WeakPtr,
                                    minitl::is_const<T>::Value
                                        ?   RTTI::Type::Const
                                        :   RTTI::Type::Mutable,
                                    RTTI::Type::Mutable);
    }
};

template< typename T >
struct TypeID< raw<T> >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::RawPtr,
                                    minitl::is_const<T>::Value
                                        ?   RTTI::Type::Const
                                        :   RTTI::Type::Mutable,
                                    RTTI::Type::Mutable);
    }
};

template< typename T >
struct TypeID< T* >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::RawPtr,
                                    minitl::is_const<T>::Value
                                        ?   RTTI::Type::Const
                                        :   RTTI::Type::Mutable,
                                    RTTI::Type::Mutable);
    }
};


template< typename T >
struct TypeID< ref<T> const >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::RefPtr,
                                    minitl::is_const<T>::Value
                                        ?   RTTI::Type::Const
                                        :   RTTI::Type::Mutable,
                                    RTTI::Type::Const);
    }
};

template< typename T >
struct TypeID< weak<T> const >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::WeakPtr,
                                    minitl::is_const<T>::Value
                                        ?   RTTI::Type::Const
                                        :   RTTI::Type::Mutable,
                                    RTTI::Type::Const);
    }
};

template< typename T >
struct TypeID< raw<T> const >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::RawPtr,
                                    minitl::is_const<T>::Value
                                        ?   RTTI::Type::Const
                                        :   RTTI::Type::Mutable,
                                    RTTI::Type::Const);
    }
};

template< typename T >
struct TypeID< T* const >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(TypeID<T>::type().metaclass,
                                    RTTI::Type::RawPtr,
                                    minitl::is_const<T>::Value
                                        ?   RTTI::Type::Const
                                        :   RTTI::Type::Mutable,
                                    RTTI::Type::Const);
    }
};

template< typename T >
struct TypeID< scoped<T> >
{
};

}}

template< typename T > static inline BugEngine::RTTI::Type be_type()
{
    return BugEngine::RTTI::TypeID<T>::type();
}

template< typename T > static inline raw<const BugEngine::RTTI::Class> be_class()
{
    return BugEngine::RTTI::ClassID<T>::klass();
}

/**************************************************************************************************/
#endif
