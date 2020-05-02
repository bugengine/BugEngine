/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_INSTANCE_HH_
#define BE_PACKAGE_NODES_INSTANCE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/node.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

class Package;
class Parameter;

class Instance : public Node
{
protected:
    istring                                  m_name;
    minitl::vector< ref< const Parameter > > m_parameters;

protected:
    Instance(weak< Package > owner, u32 line, u32 begin, u32 end);
    ~Instance();

    virtual void addedParameter(ref< const Parameter > parameter) = 0;

public:
    istring name() const
    {
        return m_name;
    }

    void setName(istring name);
    void addParameter(ref< const Parameter > param);
};

}}}  // namespace BugEngine::PackageBuilder::Nodes

/**************************************************************************************************/
#endif