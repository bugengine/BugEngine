/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_TYPEINFO_HH_
#define BE_META_TYPEINFO_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/typeinfo.script.hh>
#include <bugengine/minitl/typemanipulation.hh>

namespace BugEngine { namespace Meta {

struct Class;
struct Property;
struct Method;

}}  // namespace BugEngine::Meta

template < typename T >
static inline BugEngine::Meta::Type be_type();
template < typename T >
static inline raw< const BugEngine::Meta::Class > be_class();

namespace BugEngine { namespace Meta {

template < typename T >
struct ClassID
{
    static BE_EXPORT raw< const Meta::Class > klass();
};

template < typename T >
struct TypeID
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(ClassID< T >::klass(), Meta::Type::Value, Meta::Type::Mutable,
                                    Meta::Type::Mutable);
    }
};

template < typename T >
struct TypeID< const T > : public TypeID< T >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::Value,
                                    Meta::Type::Const, Meta::Type::Const);
    }
};

template < typename T >
struct TypeID< T& >
{
    static inline Meta::Type type()
    {
        Meta::Type t = TypeID< T >::type();
        t.access     = Meta::Type::Mutable;
        t.constness  = Meta::Type::Const;
        return t;
    }
};

template < typename T >
struct TypeID< const T& >
{
    static inline Meta::Type type()
    {
        Meta::Type t = TypeID< T >::type();
        t.access     = Meta::Type::Const;
        return t;
    }
};

template < typename T >
struct TypeID< ref< T > >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::RefPtr,
                                    minitl::is_const< T >::Value ? Meta::Type::Const
                                                                 : Meta::Type::Mutable,
                                    Meta::Type::Mutable);
    }
};

template < typename T >
struct TypeID< weak< T > >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::WeakPtr,
                                    minitl::is_const< T >::Value ? Meta::Type::Const
                                                                 : Meta::Type::Mutable,
                                    Meta::Type::Mutable);
    }
};

template < typename T >
struct TypeID< raw< T > >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::RawPtr,
                                    minitl::is_const< T >::Value ? Meta::Type::Const
                                                                 : Meta::Type::Mutable,
                                    Meta::Type::Mutable);
    }
};

template < typename T >
struct TypeID< T* >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::RawPtr,
                                    minitl::is_const< T >::Value ? Meta::Type::Const
                                                                 : Meta::Type::Mutable,
                                    Meta::Type::Mutable);
    }
};

template < typename T >
struct TypeID< ref< T > const >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::RefPtr,
                                    minitl::is_const< T >::Value ? Meta::Type::Const
                                                                 : Meta::Type::Mutable,
                                    Meta::Type::Const);
    }
};

template < typename T >
struct TypeID< weak< T > const >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::WeakPtr,
                                    minitl::is_const< T >::Value ? Meta::Type::Const
                                                                 : Meta::Type::Mutable,
                                    Meta::Type::Const);
    }
};

template < typename T >
struct TypeID< raw< T > const >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::RawPtr,
                                    minitl::is_const< T >::Value ? Meta::Type::Const
                                                                 : Meta::Type::Mutable,
                                    Meta::Type::Const);
    }
};

template < typename T >
struct TypeID< T* const >
{
    static inline Meta::Type type()
    {
        return Meta::Type::makeType(TypeID< T >::type().metaclass, Meta::Type::RawPtr,
                                    minitl::is_const< T >::Value ? Meta::Type::Const
                                                                 : Meta::Type::Mutable,
                                    Meta::Type::Const);
    }
};

template < typename T >
struct TypeID< scoped< T > >
{
};

}}  // namespace BugEngine::Meta

template < typename T >
static inline BugEngine::Meta::Type be_type()
{
    return BugEngine::Meta::TypeID< T >::type();
}

template < typename T >
static inline raw< const BugEngine::Meta::Class > be_class()
{
    return BugEngine::Meta::ClassID< T >::klass();
}

/**************************************************************************************************/
#endif
