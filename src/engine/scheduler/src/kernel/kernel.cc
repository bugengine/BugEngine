/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/kernel.script.hh>

BE_REGISTER_NAMESPACE_2_(game, BugEngine, Kernel);

namespace BugEngine { namespace Kernel
{

Kernel::Kernel(const inamespace& name)
    :   Description()
    ,   m_name(name)
{
}

}}
