/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/imemorybank.hh>
#include    <scheduler/kernel/imemoryprovider.hh>


namespace BugEngine { namespace Kernel
{


IMemoryBank::IMemoryBank(weak<const IMemoryProvider> owner)
    :   m_owner(owner)
{
}

}}
