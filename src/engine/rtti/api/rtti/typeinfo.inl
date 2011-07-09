/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_INL_
#define BE_RTTI_TYPEINFO_INL_
/*****************************************************************************/
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>


namespace BugEngine
{

static inline bool operator==(TypeInfo t1, TypeInfo t2)
{
    return t1.metaclass == t2.metaclass && t1.type == t2.type && t1.constness == t2.constness;
}
static inline bool operator<=(TypeInfo t1, TypeInfo t2)
{
    return     (t1.type & TypeInfo::TypeMask) <= (t2.type & TypeInfo::TypeMask)
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
struct be_api(RTTI) be_typeid< minitl::vector<T> >
{
    static inline ref<const RTTI::ClassInfo>    klass() { return klassBuilder(); }
    static inline TypeInfo                      type()  { return TypeInfo(klass(), TypeInfo::Class, TypeInfo::Mutable); }
private:
    class PropertyBuilder
    {
        PropertyBuilder()
        {
        }
        ~PropertyBuilder()
        {
        }
    };
    static ref<RTTI::ClassInfo> klassBuilder()
    {
        static ref<RTTI::ClassInfo> klass = ref<RTTI::ClassInfo>::create(rttiArena(), inamespace("vector"), ref<RTTI::ClassInfo>(), 0, 0);
        return klass;
    }
    static PropertyBuilder s_properties;
};

template< typename T, size_t SIZE >
struct be_api(RTTI) be_typeid< minitl::array<T,SIZE> >
{
    static inline ref<const RTTI::ClassInfo>    klass() { return klassBuilder(); }
    static inline TypeInfo                      type()  { return TypeInfo(klass(), TypeInfo::Class, TypeInfo::Mutable); }
private:
    class PropertyBuilder
    {
        PropertyBuilder()
        {
        }
        ~PropertyBuilder()
        {
        }
    };
    static ref<RTTI::ClassInfo> klassBuilder()
    {
        static ref<RTTI::ClassInfo> klass = ref<RTTI::ClassInfo>::create(rttiArena(), inamespace("array"), ref<RTTI::ClassInfo>(), 0, 0);
        return klass;
    }
    static PropertyBuilder s_properties;
};

template< typename T1, typename T2, typename T3 >
struct be_api(RTTI) be_typeid< minitl::hashmap<T1, T2, T3> >
{
    static inline ref<const RTTI::ClassInfo>    klass() { return klassBuilder(); }
    static inline TypeInfo                      type()  { return TypeInfo(klass(), TypeInfo::Class, TypeInfo::Mutable); }
private:
    class PropertyBuilder
    {
        PropertyBuilder()
        {
        }
        ~PropertyBuilder()
        {
        }
    };
    static ref<RTTI::ClassInfo> klassBuilder()
    {
        static ref<RTTI::ClassInfo> klass;
        if (klass)
            return klass;
        klass = ref<RTTI::ClassInfo>::create(rttiArena(), inamespace("hashmap"), ref<RTTI::ClassInfo>(), 0, 0);
        return klass;
    }
    static PropertyBuilder s_properties;
};
}

/*****************************************************************************/
#endif
