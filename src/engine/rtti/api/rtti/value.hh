/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_HH_
#define BE_RTTI_VALUE_HH_
/*****************************************************************************/
#include    <rtti/typeinfo.script.hh>
#include    <minitl/type/typemanipulation.hh>

namespace BugEngine { namespace RTTI
{

struct Class;
struct Property;
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


class be_api(RTTI) Value
{
    friend struct Class;
    friend struct Property;
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
    friend struct procedurehelper;
private:
    Type                m_type;
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
    bool                 m_reference;
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
    void* unpackAs(const Type& ti, ref<minitl::refcountable>& rptr, weak<minitl::refcountable>& wptr, minitl::refcountable*& ptr);
    void store(const void* src);
private:
    enum ReserveType { Reserve };

    Value(Type type, ReserveType);
public:
    enum MakeConstType { MakeConst };

    inline Value();
    template< typename T > explicit Value(T t);
    template< typename T > explicit Value(T t, MakeConstType constify);
    Value(const Value& other);
    template< typename T > Value(ByRefType<T> t);
    Value(Type typeinfo, void* location);
    ~Value();

    template< typename T > Value& operator=(const T& t);
    Value& operator=(const Value& other);

    inline Type type();
    inline Type type() const;

    template< typename T > inline const T as() const;
    template< typename T > inline typename minitl::remove_const<T>::type as();

    template< typename T > static inline ByRefType<T> ByRef(T& t) { return ByRefType<T>(t); }
    template< typename T > static inline ByRefType<const T> ByRef(const T& t) { return ByRefType<const T>(t); }
    inline bool isConst() const;

    inline operator const void*() const;
    inline bool operator!() const;

    inline Value operator[](const istring& name);
    inline Value operator()(Value params[], u32 paramCount);

    inline bool isA(const Type& type) const
    {
        return m_type.isA(type);
    }
};

}}

/*****************************************************************************/
#endif
