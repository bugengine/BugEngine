/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_ENTITY_HH_
#define BE_PACKAGE_NODES_ENTITY_HH_
/*****************************************************************************/
#include    <package/nodes/instance.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Component;

class Entity : public minitl::refcountable
{
private:
    minitl::vector< ref<Component> >    m_components;
public:
    Entity();
    ~Entity();

    void addComponent(ref<Component> component);
};

}}}

/*****************************************************************************/
#endif
