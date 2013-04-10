/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/instance.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Instance::Instance(weak<Package> owner)
:   m_owner(owner)
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

void Instance::addParameter(ref<Parameter> param)
{
    m_parameters.push_back(param);
    addedParameter(param);
}

}}}
