/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/kernel.script.hh>

namespace BugEngine { namespace KernelScheduler
{

Kernel::Kernel(const inamespace &name)
    :   m_name(name)
{
}

Kernel::~Kernel()
{
}

}}
