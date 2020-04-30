/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_ENTITY_HH_
#define BE_PACKAGE_NODES_ENTITY_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/instance.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

class Component;

class Entity : public minitl::refcountable
{
private:
    minitl::vector< ref< Component > > m_components;
    const istring                      m_name;

public:
    Entity(const istring& name);
    ~Entity();

    void           addComponent(ref< Component > component);
    const istring& getName() const;
};

}}}  // namespace BugEngine::PackageBuilder::Nodes

/**************************************************************************************************/
#endif
