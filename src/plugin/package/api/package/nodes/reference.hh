/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_REFERENCE_HH_
#define BE_PACKAGE_NODES_REFERENCE_HH_
/*****************************************************************************/

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Package;

class Reference : public minitl::refcountable, public minitl::intrusive_list<Reference>::item
{
    friend class Package;
private:
    weak< Package > const   m_owner;
    inamespace              m_name;
    BugEngine::Value        m_value;
public:
    Reference(weak<Package> owner);
    ~Reference();

    void setName(const inamespace& name);

    const BugEngine::Value& value() const   { return m_value; }
    TypeInfo getType() const                { return m_value.type(); }
};

}}}

/*****************************************************************************/
#endif
