/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/kernel/imemorybank.hh>
#include    <core/memory/kernel/imemoryprovider.hh>


namespace BugEngine { namespace Kernel
{


IMemoryBank::IMemoryBank(weak<const IMemoryProvider> owner)
    :   m_owner(owner)
{
    
}

}}
