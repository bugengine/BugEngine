/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/entity.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/component.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Entity::Entity(const istring& name)
    :   m_components(Arena::packageBuilder())
    ,   m_name(name)
{
}

Entity::~Entity()
{
}

void Entity::addComponent(ref<Component> component)
{
    m_components.push_back(component);
}

const istring& Entity::getName() const
{
    return m_name;
}

}}}
