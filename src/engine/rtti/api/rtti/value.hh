/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_HH_
#define BE_RTTI_VALUE_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>

namespace BugEngine
{

namespace RTTI
{
class ClassInfo;
struct PropertyInfo;
}

class Value
{
    friend class ::BugEngine::RTTI::ClassInfo;
    friend struct ::BugEngine::RTTI::PropertyInfo;
private:
    TypeInfo        m_type;
    union
    {
        char        m_buffer[47];
        struct
        {
            void*   m_pointer;
            bool    m_deallocate;
        };
    };
    bool            m_reference;
private:
    inline void* memory();
    inline const void* memory() const;
    inline void* rawget() const;
private:
    template< typename T >
    struct ByRefType
    {
        T& value;
        explicit ByRefType(T& t) : value(t) { }
    private:
        ByRefType& operator=(const ByRefType& other);
    };
private:
    inline Value(TypeInfo typeinfo, void* location);
    enum ReserveType { Reserve };
    inline Value(TypeInfo type, ReserveType);
public:
    enum ConstifyType { Constify };

    inline Value();
    template< typename T > explicit inline Value(T t);
    template< typename T > explicit inline Value(T t, ConstifyType constify);
    inline Value(const Value& other);
    template< typename T > explicit inline Value(ByRefType<T> t);
    inline ~Value();

    template< typename T > inline Value& operator=(const T& t);
    inline Value& operator=(const Value& other);

    inline TypeInfo type() const;

    template< typename T > inline const T as() const;
    template< typename T > inline T as();

    template< typename T > static inline ByRefType<T> ByRef(T& t) { return ByRefType<T>(t); }
    template< typename T > static inline ByRefType<const T> ByRef(const T& t) { return ByRefType<const T>(t); }
    inline bool isConst() const;

    inline operator const void*() const;
    inline bool operator!() const;

    inline Value operator[](const istring& name);
};

}

/*****************************************************************************/
#endif
