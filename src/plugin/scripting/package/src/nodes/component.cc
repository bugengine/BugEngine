/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/component.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Component::Component(weak<Package> owner)
:   Instance(owner)
{
}

Component::~Component()
{
}

void Component::addedParameter(ref<Parameter> /*parameter*/)
{
}

}}}
