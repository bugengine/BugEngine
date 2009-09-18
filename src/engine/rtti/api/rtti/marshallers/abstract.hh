/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_MARSHALLER_HH_
#define BE_RTTI_MARSHALLER_HH_
/*****************************************************************************/
#include    <rtti/value/valuecopy.hh>

namespace BugEngine { namespace RTTI
{

class AbstractMarshaller
{
public:
    virtual ValueCopy get(void* source)                                                 const = 0;
    virtual ValueCopy get(void* /*source*/, size_t /*index*/)                           const { throw 0; }
    virtual ValueCopy get(void* /*source*/, const Value& /*key*/)                       const { throw 0; }
    virtual void      set(const Value& value, void* dst)                                const = 0;
    virtual void      set(const Value& /*value*/, void* /*dst*/, size_t /*index*/)      const { throw 0; }
    virtual void      set(const Value& /*value*/, void* /*dst*/, const Value& /*key*/)  const { throw 0; }
    virtual void      push_back(const Value& /*value*/, void* /*dst*/)                  const { throw 0; }
    virtual void      push_front(const Value& /*value*/, void* /*dst*/)                 const { throw 0; }
    virtual void      pop_back(const Value& /*value*/, void* /*dst*/)                   const { throw 0; }
    virtual void      pop_front(const Value& /*value*/, void* /*dst*/)                  const { throw 0; }

    virtual ValueTypeIndex  type() const = 0;
};

template< typename T >
class Marshaller : public AbstractMarshaller
{
};

template< typename T >
class Marshaller<const T&> : public Marshaller<T>
{
};

}}

/*****************************************************************************/
#endif
