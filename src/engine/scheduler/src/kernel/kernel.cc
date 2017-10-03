/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/kernel.script.hh>
#include    <rtti/engine/namespace.hh>

BE_REGISTER_NAMESPACE_2_(bugengine, BugEngine, Kernel);

namespace BugEngine { namespace Kernel
{

KernelDescription::KernelDescription(const inamespace& name)
    :   Description()
    ,   m_name(name)
{
}

}}
