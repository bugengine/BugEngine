/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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
