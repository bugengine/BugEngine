/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace Kernel
{

IMemoryHost::IMemoryHost(const istring &name)
    :   m_name(name)
{
}

IMemoryHost::~IMemoryHost()
{
}

}}
