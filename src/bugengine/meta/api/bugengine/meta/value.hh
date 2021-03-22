/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_VALUE_HH_
#define BE_META_VALUE_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/typeinfo.script.hh>
#include <bugengine/minitl/typemanipulation.hh>

namespace BugEngine { namespace Meta {

struct Class;
struct Property;

class be_api(META) Value
{
    friend struct Class;
    friend struct Property;

private:
    Type m_type;
    struct Reference
    {
        void* m_pointer;
        bool  m_deallocate;
    };
    union
    {
        Reference m_ref;
        char      m_buffer[47];
    };
    bool m_reference;

private:
    inline const void* memory() const;
    inline void*       rawget() const;

private:
    template < typename T >
    struct ByRefType
    {
        T& value;
        explicit ByRefType(T& t) : value(t)
        {
        }
    };
    void* unpackAs(const Type& ti, ref< minitl::refcountable >& rptr,
                   weak< minitl::refcountable >& wptr, minitl::refcountable*& ptr);
    void  store(const void* src);

public:
    enum ReserveType
    {
        Reserve
    };

    Value(Type type, ReserveType);
    inline void* memory();

public:
    enum MakeConstType
    {
        MakeConst
    };

    inline Value();
    template < typename T >
    explicit Value(T t);
    template < typename T >
    explicit Value(T t, MakeConstType constify);
    Value(const Value& other);
    template < typename T >
    Value(ByRefType< T > t);
    Value(Type typeinfo, void* location);
    Value(Type typeinfo, const Value& castedFrom);
    ~Value();

    template < typename T >
    Value& operator=(const T& t);
    Value& operator=(const Value& other);

    inline Type type();
    inline Type type() const;

    template < typename T >
    inline const T as() const;
    template < typename T >
    inline T as();

    template < typename T >
    static inline ByRefType< T > ByRef(T & t)
    {
        return ByRefType< T >(t);
    }
    static inline ByRefType< const Value > ByRef(const Value& t)
    {
        return ByRefType< const Value >(t);
    }
    inline bool isConst() const;

    inline      operator const void*() const;
    inline bool operator!() const;

    Value operator[](const istring& name);
    Value operator[](const istring& name) const;
    Value operator()(Value params[], u32 paramCount);

    void swap(Value & other);

    inline bool isA(const Type& type) const
    {
        return m_type.isA(type);
    }
};

}}  // namespace BugEngine::Meta

#include <bugengine/meta/value.inl>

/**************************************************************************************************/
#endif
