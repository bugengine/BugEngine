/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/imemoryprovider.hh>


namespace BugEngine { namespace Kernel
{

IMemoryProvider::IMemoryProvider(const istring& name)
    :   m_name(name)
{
}

}}