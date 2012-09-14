/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_GET_HH_
#define BE_RTTI_ENGINE_HELPER_GET_HH_
/*****************************************************************************/
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class Value;

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
