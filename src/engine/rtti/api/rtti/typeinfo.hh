/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

struct ClassInfo;

class TypeInfo
{
public:
    enum Type
    {
        Class = 0,
        Struct,
        Array,
        Const,
        Ref,
        Ptr,
        RefPtr,
        WeakPtr
    };
private:
    static const size_t s_maxAttributeCount = 7;
    const ClassInfo&    m_class;
    struct Indirection
    {
        unsigned int        type:5;
        unsigned int        count:27;
    }   m_attributes[s_maxAttributeCount];
public:
    inline TypeInfo(const TypeInfo& proxy, Type type, size_t count = 1);
    inline explicit TypeInfo(const ClassInfo& klass);
    inline ~TypeInfo();

    void    copy(const void* src, void* dst) const;
    void    destroy(void* src) const;
    size_t  size() const;
};

}

template< typename T >
struct be_typeid
{
    static const RTTI::ClassInfo* const klass;
    inline static RTTI::TypeInfo type()
    {
        return RTTI::TypeInfo(klass);
    }
};

template< typename T >
struct be_typeid< ref<T> >
{
    inline static RTTI::TypeInfo type()
    {
        return RTTI::TypeInfo(be_typeid<T>::type(), RTTI::TypeInfo::RefPtr);
    }
};

template< typename T >
struct be_typeid< weak<T> >
{
    inline static RTTI::TypeInfo type()
    {
        return RTTI::TypeInfo(be_typeid<T>::type(), RTTI::TypeInfo::WeakPtr);
    }
};

template< typename T >
struct be_typeid< const T >
{
    inline static RTTI::TypeInfo type()
    {
        return RTTI::TypeInfo(be_typeid<T>::type(), RTTI::TypeInfo::Const);
    }
};

template< typename T >
struct be_typeid< T& >
{
    inline static RTTI::TypeInfo type()
    {
        return RTTI::TypeInfo(be_typeid<T>::type(), RTTI::TypeInfo::Ref);
    }
};

template< typename T >
struct be_typeid< T* >
{
    inline static RTTI::TypeInfo type()
    {
        return RTTI::TypeInfo(be_typeid<T>::type(), RTTI::TypeInfo::Ptr);
    }
};

template< typename T, size_t count >
struct be_typeid< T[count] >
{
    inline static RTTI::TypeInfo type()
    {
        return RTTI::TypeInfo(be_typeid<T>::type(), RTTI::TypeInfo::Array, count);
    }
};

}

#include <rtti/typeinfo.inl>

/*****************************************************************************/
#endif
