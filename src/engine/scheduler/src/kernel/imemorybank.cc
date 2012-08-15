/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
