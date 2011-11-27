/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_NODES_OBJECT_HH_
#define BE_PACKAGEBUILDER_NODES_OBJECT_HH_
/*****************************************************************************/
#include    <packagebuilder/nodes/overloadmatch.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Package;
class Parameter;
class Reference;

class Object : public minitl::refcountable
{
private:
    weak< Package > const                   m_owner;
    istring                                 m_name;
    ref<Reference>                          m_methodReference;
    const RTTI::MethodInfo*                 m_method;
    minitl::vector< ref<Parameter> >        m_parameters;
    minitl::vector< OverloadMatch >         m_overloads;
private:
    void resolveOverload();
public:
    Object(weak<Package> owner);
    ~Object();

    istring name() const                                        { return m_name; }
    const RTTI::MethodInfo* method() const                      { return m_method; }
    const minitl::vector< OverloadMatch >& overloads() const    { return m_overloads; }

    void setName(istring name);
    void setMethod(ref<Reference> reference);
    void addParameter(ref<Parameter> param);
};

}}}

/*****************************************************************************/
#endif
