/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/kernel/kernel.script.hh>

BE_REGISTER_NAMESPACE_2_NAMED(game, BugEngine, Kernel);

namespace BugEngine { namespace Kernel
{

Kernel::Kernel(const inamespace& name)
    :   Resource()
    ,   m_name(name)
{
}

}}
