/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/kernel/imemoryprovider.hh>


namespace BugEngine { namespace Kernel
{

IMemoryProvider::IMemoryProvider(const istring& name)
    :   m_name(name)
{
}

}}
