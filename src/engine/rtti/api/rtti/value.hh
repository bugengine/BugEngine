/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_HH_
#define BE_RTTI_VALUE_HH_
/*****************************************************************************/
#include    <minitl/type/variant.hh>
#include    <rtti/value/valuetypes.hh>
#include    <rtti/value/valuecopy.hh>
#include    <rtti/value/valueref.hh>

namespace BugEngine
{

class Value
{
private:
    typedef TYPELIST2(RTTI::ValueCopy,RTTI::ValueRef)   typelist;
private:
    minitl::variant<typelist>   m_value;
public:
    inline Value();
    inline explicit Value(const RTTI::ValueRef& valueref);
    inline explicit Value(const RTTI::ValueCopy& copy);
    inline Value(const Value& other);
    template< typename T >
    inline explicit Value(const T& immediate);
    inline ~Value();
    inline Value& operator=(const Value& other);
    template< typename T >
    inline Value& operator=(const T& other);

    template< typename T >
    inline T as() const;
    template< typename T >
    inline T member() const;

    inline RTTI::ValueTypeIndex type() const;

    inline operator void*() const;
};

}

#include    <rtti/value.inl>

/*****************************************************************************/
#endif
