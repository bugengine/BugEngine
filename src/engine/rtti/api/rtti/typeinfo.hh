/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/

namespace BugEngine
{

class ClassInfo;

struct TypeInfo
{
public:
    enum Type
    {
        Raw,
        Array,
        Class,
        Const,
        Ref,
        Ptr,
        Struct,
        RefPtr,
        WeakPtr
    };
private:
    Type                    m_type;
    size_t                  m_count;
    weak<const ClassInfo>   m_class;
public:
    inline TypeInfo(const TypeInfo& proxy, Type type, size_t count = 1);
    inline explicit TypeInfo(weak<const ClassInfo> klass);
    inline ~TypeInfo();

    inline void copy(const void* src, void* dst) const;
    inline void destroy(void* src) const;
    inline size_t size() const;
};

template< typename T >
struct be_typeid
{
    static TypeInfo type();
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
