/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PROPERTYGETTERS_HH_
#define BE_RTTI_PROPERTYGETTERS_HH_
/*****************************************************************************/
#include    <minitl/type/typemanipulation.hh>

namespace BugEngine { namespace RTTI
{

template< typename OWNER,
          typename T >
class GetImpossible
{
public:
    typedef T PropertyType;
    typedef OWNER   Owner;
    enum
    {
        Read = 0,
        Ref = 0
    };
    static inline T get(OWNER* from);
};

//-----------------------------------------------------------------------------

template< typename OWNER,
          typename T,
          T (OWNER::*GETTER)() >
class GetFromGetter
{
public:
    typedef typename minitl::remove_const< typename minitl::remove_reference<T>::type >::type   PropertyType;
    typedef OWNER                                                                               Owner;
    enum
    {
        Read = 1,
        Ref = minitl::is_reference<T>::Value
    };
    static inline T get(OWNER* from);
};

//-----------------------------------------------------------------------------

template< typename OWNER,
          typename T,
          T (OWNER::*GETTER)() const >
class GetFromGetterConst
{
public:
    typedef typename minitl::remove_const< typename minitl::remove_reference<T>::type >::type   PropertyType;
    typedef OWNER                                                                               Owner;
    enum
    {
        Read = 1,
        Ref = minitl::is_reference<T>::Value && ! minitl::is_const<T>::Value
    };
    static inline T get(OWNER* from);
};

//-----------------------------------------------------------------------------

template< typename OWNER,
          typename T,
          size_t offset >
class GetFromField
{
public:
    typedef T PropertyType;
    typedef OWNER   Owner;
    enum
    {
        Read = 1,
        Ref = 1
    };
    static inline T& get(OWNER* from);
};

//-----------------------------------------------------------------------------

template< typename ValueGetter,
          bool UseRef,
          typename Marshaller >
struct ValueGetterProxy;

template< typename ValueGetter,
          typename Marshaller >
struct ValueGetterProxy<ValueGetter, true, Marshaller >
{
public:
    static inline Value get(typename ValueGetter::Owner* from, const Marshaller* marshaller);
};
template< typename ValueGetter,
          typename Marshaller >
struct ValueGetterProxy<ValueGetter, false, Marshaller >
{
public:
    static inline Value get(typename ValueGetter::Owner* from, const Marshaller* marshaller);
};

}}

#include    <rtti/properties/getters.inl>

/*****************************************************************************/
#endif
