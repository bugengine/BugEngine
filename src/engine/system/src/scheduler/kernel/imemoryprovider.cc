/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/kernel/imemoryprovider.hh>


namespace BugEngine { namespace Kernel
{

IMemoryProvider::IMemoryProvider(const istring& name)
    :   m_name(name)
{
}

}}
