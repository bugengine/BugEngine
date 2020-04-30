/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/component.hh>
#include <bugengine/plugin.scripting.package/nodes/entity.hh>
#include <bugengine/plugin.scripting.package/nodes/package.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

Entity::Entity(const istring& name) : m_components(Arena::packageBuilder()), m_name(name)
{
}

Entity::~Entity()
{
}

void Entity::addComponent(ref< Component > component)
{
    m_components.push_back(component);
}

const istring& Entity::getName() const
{
    return m_name;
}

}}}  // namespace BugEngine::PackageBuilder::Nodes
