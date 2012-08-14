/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/kernel/imemorybank.hh>
#include    <system/scheduler/kernel/imemoryprovider.hh>


namespace BugEngine { namespace Kernel
{


IMemoryBank::IMemoryBank(weak<const IMemoryProvider> owner)
    :   m_owner(owner)
{
}

}}
