/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/
#include    <rtti/typeinfo.script.hh>
#include    <minitl/typemanipulation.hh>
#include    <rtti/classinfo.script.hh>

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

template< > BE_EXPORT raw<RTTI::Class> be_typeid< void >::preklass();

}


namespace minitl
{

template<u16 SIZE>
const minitl::format<SIZE>& operator|(const minitl::format<SIZE>& format, const BugEngine::RTTI::Type& type)
{
    minitl::format<4096> typeName("%s%s%s%s%s");
    if (type.constness == BugEngine::RTTI::Type::Const)
    {
        typeName | "const ";
    }
    else
    {
        typeName | "";
    }
    const char* constness = "";
    if (type.access == BugEngine::RTTI::Type::Const)
    {
        constness = "const ";
    }
    switch(type.indirection)
    {
        case BugEngine::RTTI::Type::RefPtr:
        typeName | "ref<" | constness | type.metaclass->fullname() | ">";
        break;
    case BugEngine::RTTI::Type::WeakPtr:
        typeName | "weak<" | constness | type.metaclass->fullname() | ">";
        break;
    case BugEngine::RTTI::Type::RawPtr:
        typeName | "raw<" | constness | type.metaclass->fullname() | ">";
        break;
    case BugEngine::RTTI::Type::Value:
        typeName | "" | "" | type.metaclass->fullname() | "";
        break;
    default:
        be_notreached();
        break;
    }
    return format | typeName.c_str();
}

}

/*****************************************************************************/
#endif
