/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUECOPY_HH_
#define BE_RTTI_VALUECOPY_HH_
/*****************************************************************************/
#include    <minitl/type/variant.hh>
#include    <rtti/value/valuetypes.hh>

namespace BugEngine { namespace RTTI
{

class ValueCopy
{
    friend class Value;
private:
    minitl::variant<ValueTypes> m_value;
public:
    template< typename T >
    inline explicit ValueCopy(const T& value);
    template< typename T >
    inline void operator=(const T& value);

    template< typename T >
    T as() const;
    template< typename T >
    T member() const;

    ValueTypeIndex typeindex() const;
};


}}

#include    <rtti/value/valuecopy.inl>

/*****************************************************************************/
#endif
