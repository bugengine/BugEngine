/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/entity.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/component.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Entity::Entity()
    :   m_components(Arena::packageBuilder())
{
}

Entity::~Entity()
{
}

void Entity::addComponent(ref<Component> component)
{
    m_components.push_back(component);
}

}}}
