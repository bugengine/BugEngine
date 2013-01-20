/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/
#include    <rtti/typeinfo.script.hh>
#include    <minitl/typemanipulation.hh>

namespace BugEngine
{

template< typename T >
struct be_typeid
{
    template< typename U > friend struct be_typeid;
private:
    static raw<const RTTI::Class> registerProperties();
public:
    static BE_EXPORT raw<RTTI::Class> preklass();
    static inline raw<const RTTI::Class> klass()
    {
        static raw<const RTTI::Class> cls = registerProperties();
        return cls;
    }
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable); }
};

template< typename T >
struct be_typeid<const T> : public be_typeid<T>
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::Value, RTTI::Type::Const, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< T& >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< const T& >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::Value, RTTI::Type::Const, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< ref<T> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::RefPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Mutable); }
};

template< typename T >
struct be_typeid< weak<T> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::WeakPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Mutable); }
};

template< typename T >
struct be_typeid< raw<T> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::RawPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Mutable); }
};

template< typename T >
struct be_typeid< T* >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::RawPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Mutable); }
};


template< typename T >
struct be_typeid< ref<T> const >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::RefPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< weak<T> const >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::WeakPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< raw<T> const >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::RawPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< T* const >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(be_typeid<T>::preklass(), RTTI::Type::RawPtr, minitl::is_const<T>::Value ? RTTI::Type::Const : RTTI::Type::Mutable, RTTI::Type::Const); }
};

template< typename T >
struct be_typeid< scoped<T> >
{
};

template< > BE_EXPORT raw<const RTTI::Class> be_typeid< void >::klass();

}


namespace minitl
{

template<u16 SIZE>
const minitl::format<SIZE>& operator|(const minitl::format<SIZE>& format, const BugEngine::RTTI::Type& type)
{
    be_forceuse(type);
    /*BugEngine::Debug::Format<> n(metaclass->name.str());
    switch(access)
    {
    case Const:
        n.append(" const");
        break;
    case Mutable:
        break;
    default:
        be_notreached();
    }

    switch(indirection)
    {
    case Value:
        break;
    case RawPtr:
        n = BugEngine::Debug::Format<>("raw<%s>") | n;
        break;
    case WeakPtr:
        n = BugEngine::Debug::Format<>("weak<%s>") | n;
        break;
    case RefPtr:
        n = BugEngine::Debug::Format<>("ref<%s>") | n;
        break;
    default:
        be_notreached();
        break;
    }

    switch(constness)
    {
    case Const:
        n.append(" const");
        break;
    case Mutable:
        break;
    default:
        be_notreached();
    }
    return n;*/
    return format;
}

}

/*****************************************************************************/
#endif
