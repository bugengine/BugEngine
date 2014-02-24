/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_COMPONENT_HH_
#define BE_PACKAGE_NODES_COMPONENT_HH_
/**************************************************************************************************/
#include    <package/nodes/instance.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Component : public Instance
{
private:
private:
    void addedParameter(ref<Parameter> parameter) override;
public:
    Component(weak<Package> owner);
    ~Component();
};

}}}

/**************************************************************************************************/
#endif
