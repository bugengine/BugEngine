/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_REFERENCE_HH_
#define BE_PACKAGE_NODES_REFERENCE_HH_
/**************************************************************************************************/
#include    <package/nodes/node.hh>
#include    <minitl/intrusive_list.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Package;
class Object;

class Reference : public Node,
                  public minitl::intrusive_list<Reference>::item
{
    friend class Package;
private:
    inamespace              m_name;
    RTTI::Value             m_value;
    weak<const Object>      m_object;
public:
    Reference(weak<Package> owner, u32 line, u32 begin, u32 end);
    ~Reference();

    void setName(const inamespace& name);

    RTTI::Value getValue() const;
    RTTI::Type getType() const;
};

}}}

/**************************************************************************************************/
#endif
