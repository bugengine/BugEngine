/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_COMPONENT_HH_
#define BE_PACKAGE_NODES_COMPONENT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/instance.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

class Component : public Instance
{
private:
    void addedParameter(ref< const Parameter > parameter) override;

public:
    Component(weak< Package > owner, u32 line, u32 begin, u32 end);
    ~Component();
};

}}}  // namespace BugEngine::PackageBuilder::Nodes

/**************************************************************************************************/
#endif
