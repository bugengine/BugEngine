/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUEREF_HH_
#define BE_RTTI_VALUEREF_HH_
/*****************************************************************************/
#include    <minitl/type/variant.hh>
#include    <rtti/value/valuetypes.hh>

namespace BugEngine
{

class Value;
    
namespace RTTI
{

class AbstractMarshaller;


class ValueRef
{
    friend class BugEngine::Value;
private:
    void*                       m_data;
    const AbstractMarshaller*   m_marshaller;
public:
    inline ValueRef(void* m_data, const AbstractMarshaller* m_marshaller);
    inline ValueRef(const ValueRef& other);
    inline ~ValueRef();
    inline void operator=(const Value& v);

    template< typename T >
    inline T as() const;
    template< typename T >
    inline T member() const;
};

}}

#include    <rtti/value/valueref.inl>

/*****************************************************************************/
#endif
