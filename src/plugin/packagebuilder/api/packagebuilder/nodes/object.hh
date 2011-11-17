/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_NODES_OBJECT_HH_
#define BE_PACKAGEBUILDER_NODES_OBJECT_HH_
/*****************************************************************************/

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
    const RTTI::MethodInfo::OverloadInfo*   m_overload;
    minitl::vector< ref<Parameter> >        m_parameters;
private:
    void resolveOverload();
public:
    Object(weak<Package> owner);
    ~Object();

    istring name() const                                    { return m_name; }
    const RTTI::MethodInfo* method() const                  { return m_method; }
    const RTTI::MethodInfo::OverloadInfo* overload() const  { return m_overload; }
    const minitl::vector< ref<Parameter> >& params() const  { return m_parameters; }

    void setName(istring name);
    void setMethod(ref<Reference> reference);
    void addParam(ref<Parameter> param);

    void doCall();
};

}}}

/*****************************************************************************/
#endif
