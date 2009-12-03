/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PROPERTYSETTERS_HH_
#define BE_RTTI_PROPERTYSETTERS_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{


template< typename OWNER,
          typename T >
class SetImpossible
{
public:
    typedef T PropertyType;
    typedef OWNER   Owner;
    enum
    {
        Write = 0,
        Ref = 0
    };
    static inline void set(weak<OWNER> from, const T& value);
};

template< typename OWNER,
          typename T,
          void (OWNER::*SETTER)(T value) >
class SetFromSetter
{
public:
    typedef typename minitl::remove_const< typename minitl::remove_reference<T>::type >::type   PropertyType;
    typedef OWNER                                                                               Owner;
    enum
    {
        Write = 1
    };
    static inline void set(weak<OWNER> from, const PropertyType& value);
};

template< typename OWNER, typename T, size_t offset >
class SetFromField
{
public:
    typedef T PropertyType;
    typedef OWNER   Owner;
    enum
    {
        Write = 1
    };
    static inline void set(weak<OWNER> from, const T& value);
};

}}

#include    <rtti/properties/setters.inl>

/*****************************************************************************/
#endif
