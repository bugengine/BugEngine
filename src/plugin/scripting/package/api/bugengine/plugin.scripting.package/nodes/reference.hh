/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_REFERENCE_HH_
#define BE_PACKAGE_NODES_REFERENCE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/minitl/intrusive_list.hh>
#include <bugengine/plugin.scripting.package/nodes/node.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

class Package;
class Object;

class Reference
    : public Node
    , public minitl::intrusive_list< Reference >::item
{
    friend class Package;

private:
    inamespace           m_name;
    RTTI::Value          m_value;
    weak< const Object > m_object;

public:
    Reference(weak< Package > owner, u32 line, u32 begin, u32 end);
    ~Reference();

    void setName(const inamespace& name);
    const inamespace& name() const
    {
        return m_name;
    }

    RTTI::Value getValue() const;
    RTTI::Type  getType() const;
};

}}}  // namespace BugEngine::PackageBuilder::Nodes

/**************************************************************************************************/
#endif
