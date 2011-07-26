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
template< typename T,
          typename P1,
          typename P2,
          typename P3,
          typename P4,
          typename P5,
          typename P6,
          typename P7,
          typename P8,
          typename P9,
          typename P10,
          typename P11,
          typename P12,
          typename P13,
          typename P14,
          typename P15,
          typename P16,
          typename Dummy>
struct procedurehelper;
}

class Value
{
    friend class RTTI::ClassInfo;
    friend struct RTTI::PropertyInfo;
    template< typename T,
          typename P1,
          typename P2,
          typename P3,
          typename P4,
          typename P5,
          typename P6,
          typename P7,
          typename P8,
          typename P9,
          typename P10,
          typename P11,
          typename P12,
          typename P13,
          typename P14,
          typename P15,
          typename P16,
          typename Dummy>
    friend struct RTTI::procedurehelper;
private:
    TypeInfo        m_type;
    struct Reference
    {
        void*           m_pointer;
        bool            m_deallocate;
    };
    union
    {
        Reference       m_ref;
        char            m_buffer[47];
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
    enum ReserveType { Reserve };

    inline Value(TypeInfo type, ReserveType);
    inline Value(TypeInfo typeinfo, void* location);
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
    
    inline TypeInfo type();
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
