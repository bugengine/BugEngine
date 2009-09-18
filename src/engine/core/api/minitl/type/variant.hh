/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_TYPE_UNION_
#define BE_MINITL_TYPE_UNION_
/*****************************************************************************/
#include    <minitl/type/typelist.hh>


namespace minitl
{

template< typename typelist >
void destroy(size_t typeindex, unsigned char* dest);
template< typename typelist >
void copy(size_t typeindex, unsigned char* dest, const unsigned char* src);

template< typename typelist >
class variant
{
private:
    size_t          m_typeIndex;
    unsigned char   m_value[typelist::SizeOf];
public:
    variant();
    
    template< typename T >
    inline explicit variant(const T& value);
    inline variant(const variant<typelist>& other);
    inline variant<typelist>& operator=(const variant<typelist>& other);
    template< typename T >
    inline variant& operator=(const T& value);
    
    inline ~variant();

    inline size_t typeindex() const { return m_typeIndex; }
    template< typename T >
    inline T& as();
    template< typename T >
    inline const T& as() const;
};


template< typename typelist >
variant<typelist>::variant() :
    m_typeIndex( indexof<void,typelist>::Value )
{
}


template< typename typelist >
template< typename T >
variant< typelist >::variant(const T& value) :
    m_typeIndex( indexof<T,typelist>::Value )
{
    new(m_value) T(value);
}

template< typename typelist >
variant<typelist>::variant(const variant& other) :
    m_typeIndex( other.m_typeIndex )    
{
    copy<typelist>(m_typeIndex, m_value, other.m_value);
}

template< typename typelist >
variant<typelist>& variant<typelist>::operator=(const variant<typelist>& other)
{
    destroy<typelist>(m_typeIndex, m_value);
    m_typeIndex = other.m_typeIndex;
    copy<typelist>(m_typeIndex, m_value, other.m_value);
    return *this;
}

template< typename typelist >
template< typename T >
variant<typelist>& variant<typelist>::operator=(const T& value)
{
    destroy<typelist>(m_typeIndex, m_value);
    m_typeIndex = indexof<T,typelist>::Value;
    new(m_value) T(value);
    return *this;
}

template< typename typelist >
variant< typelist >::~variant()
{
    destroy<typelist>(m_typeIndex, m_value);
}

template< typename typelist >
template< typename T >
const T& variant< typelist >::as() const
{
    be_assert(m_typeIndex == (indexof<T,typelist>::Value), "invalid use of the variant value, asked for type %s" | typeid(T).name());
    return *reinterpret_cast<const T*>(&m_value[0]);
}

template< typename typelist >
template< typename T >
T& variant< typelist >::as()
{
    be_assert(m_typeIndex == (indexof<T,typelist>::Value), "invalid use of the variant value, asked for type %s" | typeid(T).name());
    return *reinterpret_cast<T*>(&m_value[0]);
}

//-----------------------------------------------------------------------------

template< >
inline void destroy<void>(size_t typeindex, unsigned char *data)
{
    UNUSED(typeindex);
    UNUSED(data);
}

template< typename typelist >
inline void destroy(size_t typeindex, unsigned char *data)
{
    if(typeindex == typelist::IndexOf)
    {
        typedef typename typelist::Type thistype;
        reinterpret_cast<thistype*>(data)->~thistype();
    }
    else
    {
        destroy<typename typelist::Tail>(typeindex, data);
    }
}


template< >
inline void copy<void>(size_t typeindex, unsigned char* dest, const unsigned char* src)
{
    UNUSED(typeindex);
    UNUSED(dest);
    UNUSED(src);
}

template< typename typelist >
inline void copy(size_t typeindex, unsigned char* dest, const unsigned char* src)
{
    if(typeindex == typelist::IndexOf)
    {
        typedef typename typelist::Type thistype;
        new(dest) thistype(*reinterpret_cast<const thistype*>(src));
    }
    else
    {
        copy<typename typelist::Tail>(typeindex, dest, src);
    }
}


}

/*****************************************************************************/
#endif
