/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/imemoryhost.hh>
#include <bugengine/scheduler/kernel/iproduct.script.hh>

namespace BugEngine {

raw< RTTI::Class > be_bugengine_Namespace_BugEngine_KernelScheduler();

namespace KernelScheduler {

IProduct::~IProduct()
{
}

void IProduct::addOutputHost(weak< IMemoryHost > host)
{
    for(minitl::vector< HostInformation >::iterator it = m_productOutput.begin();
        it != m_productOutput.end(); ++it)
    {
        if(it->first == host)
        {
            ++it->second;
            return;
        }
    }
    m_productOutput.push_back(HostInformation(host, 1));
}

void IProduct::removeOutputHost(weak< IMemoryHost > host)
{
    for(minitl::vector< HostInformation >::iterator it = m_productOutput.begin();
        it != m_productOutput.end(); ++it)
    {
        if(it->first == host)
        {
            if(--it->second == 0) m_productOutput.erase(it);
            return;
        }
    }
    be_notreached();
}

raw< RTTI::Class > IProduct::getNamespace()
{
    return be_bugengine_Namespace_BugEngine_KernelScheduler();
}

}  // namespace KernelScheduler
}  // namespace BugEngine
