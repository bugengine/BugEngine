/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/

namespace BugEngine
{

class ClassInfo;

class TypeInfo
{
public:
    enum Type
    {
        Class = 0,
        Array,
        Const,
        Ref,
        Ptr,
        RefPtr,
        WeakPtr
    };
private:
    static const size_t s_maxAttributeCount = 7;
    weak<const ClassInfo>   m_class;
    struct Indirection
    {
        unsigned int        type:8;
        unsigned int        count:24;
    }   m_attributes[s_maxAttributeCount];
public:
    inline TypeInfo(const TypeInfo& proxy, Type type, size_t count = 1);
    inline explicit TypeInfo(weak<const ClassInfo> klass);
    inline ~TypeInfo();

    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
    size_t size() const;
    std::string name() const;
};

template< typename T >
struct be_typeid
{
    static weak<const ClassInfo> klass();
    inline static TypeInfo type()
    {
        return TypeInfo(klass());
    }
};

template< typename T >
struct be_typeid< ref<T> >
{
    inline static TypeInfo type()
    {
        return TypeInfo(be_typeid<T>::type(), TypeInfo::RefPtr);
    }
};

template< typename T >
struct be_typeid< weak<T> >
{
    inline static TypeInfo type()
    {
        return TypeInfo(be_typeid<T>::type(), TypeInfo::WeakPtr);
    }
};

template< typename T >
struct be_typeid< const T >
{
    inline static TypeInfo type()
    {
        return TypeInfo(be_typeid<T>::type(), TypeInfo::Const);
    }
};

template< typename T >
struct be_typeid< T& >
{
    inline static TypeInfo type()
    {
        return TypeInfo(be_typeid<T>::type(), TypeInfo::Ref);
    }
};

template< typename T >
struct be_typeid< T* >
{
    inline static TypeInfo type()
    {
        return TypeInfo(be_typeid<T>::type(), TypeInfo::Ptr);
    }
};

template< typename T, size_t count >
struct be_typeid< T[count] >
{
    inline static TypeInfo type()
    {
        return TypeInfo(be_typeid<T>::type(), TypeInfo::Array, count);
    }
};

}

#include <rtti/typeinfo.inl>

/*****************************************************************************/
#endif
