/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_REFERENCE_HH_
#define BE_PACKAGE_NODES_REFERENCE_HH_
/*****************************************************************************/
#include    <minitl/intrusive_list.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Package;
class Object;

class Reference : public minitl::refcountable, public minitl::intrusive_list<Reference>::item
{
    friend class Package;
private:
    weak< Package > const   m_owner;
    inamespace              m_name;
    RTTI::Value             m_value;
    weak<const Object>      m_object;
public:
    Reference(weak<Package> owner);
    ~Reference();

    void setName(const inamespace& name);

    RTTI::Value getValue() const;
    RTTI::Type getType() const;
};

}}}

/*****************************************************************************/
#endif
