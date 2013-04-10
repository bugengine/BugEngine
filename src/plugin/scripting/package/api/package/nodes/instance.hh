/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_INSTANCE_HH_
#define BE_PACKAGE_NODES_INSTANCE_HH_
/*****************************************************************************/
#include    <package/nodes/overloadmatch.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Package;
class Parameter;

class Instance : public minitl::refcountable
{
protected:
    weak< Package > const               m_owner;
    istring                             m_name;
    minitl::vector< ref<Parameter> >    m_parameters;
protected:
    Instance(weak<Package> owner);
    ~Instance();

    virtual void addedParameter(ref<Parameter> parameter) = 0;
public:
    istring name() const                                        { return m_name; }

    void setName(istring name);
    void addParameter(ref<Parameter> param);
};

}}}

/*****************************************************************************/
#endif
