/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/instance.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Instance::Instance(weak<Package> owner, u32 line, u32 begin, u32 end)
    :   Node(owner, line, begin, end)
    ,   m_name("")
    ,   m_parameters(Arena::packageBuilder())
{
}

Instance::~Instance()
{
}

void Instance::setName(istring name)
{
    if (m_owner->findByName(name))
    {
        // error
        be_unimplemented();
    }
    m_name = name;
}

void Instance::addParameter(ref<const Parameter> param)
{
    m_parameters.push_back(param);
    addedParameter(param);
}

}}}
