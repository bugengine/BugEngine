/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/component.hh>
#include <bugengine/plugin.scripting.package/nodes/package.hh>
#include <bugengine/plugin.scripting.package/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

Component::Component(weak< Package > owner, u32 line, u32 begin, u32 end)
    : Instance(owner, line, begin, end)
{
}

Component::~Component()
{
}

void Component::addedParameter(ref< const Parameter > /*parameter*/)
{
}

}}}  // namespace BugEngine::PackageBuilder::Nodes
