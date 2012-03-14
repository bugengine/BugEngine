/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_GET_HH_
#define BE_RTTI_ENGINE_HELPER_GET_HH_
/*****************************************************************************/
#include   <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

template< typename T, typename Owner, T (Owner::*Member) >
struct PropertyHelper
{
    static Value get(void* from, bool isConst)
    {
        if (isConst)
        {
            return Value::ByRef(reinterpret_cast<const Owner*>(from)->*Member);
        }
        else
        {
            return Value::ByRef(reinterpret_cast<Owner*>(from)->*Member);
        }
    }
};

}}

/*****************************************************************************/
#endif
