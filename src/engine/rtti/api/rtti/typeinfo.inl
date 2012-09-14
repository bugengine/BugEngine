/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_INL_
#define BE_RTTI_TYPEINFO_INL_
/*****************************************************************************/
#include    <rtti/typeinfo.hh>

namespace BugEngine { namespace RTTI
{

template< typename T >
bool Type::isA() const
{
    return isA(be_typeid<T>::type());
}

}}

/*****************************************************************************/
#endif
