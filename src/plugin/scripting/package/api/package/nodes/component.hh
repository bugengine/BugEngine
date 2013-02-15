/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_COMPONENT_HH_
#define BE_PACKAGE_NODES_COMPONENT_HH_
/*****************************************************************************/
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

/*****************************************************************************/
#endif
