/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VARIANT_MARSHALLER_INL_
#define BE_RTTI_VARIANT_MARSHALLER_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

const Value& Marshaller<Value>::castfrom(const Value& value) const
{
    return value;
}

const Value& Marshaller<Value>::castto(const Value& value) const
{
    return value;
}

}}


/*****************************************************************************/
#endif
