/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_OBJECT_HH_
#define BE_PACKAGE_NODES_OBJECT_HH_
/**************************************************************************************************/
#include    <package/nodes/instance.hh>
#include    <package/nodes/overloadmatch.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Reference;

class Object : public Instance
{
private:
    RTTI::Type                          m_objectType;
    ref<Reference>                      m_methodReference;
    const RTTI::Method*                 m_method;
    minitl::vector< OverloadMatch >     m_overloads;
private:
    void resolveOverload();
    void addedParameter(ref<Parameter> parameter) override;
public:
    Object(weak<Package> owner, u32 line, u32 begin, u32 end);
    ~Object();

    const RTTI::Method* method() const                          { return m_method; }
    const minitl::vector< OverloadMatch >& overloads() const    { return m_overloads; }

    void setMethod(ref<Reference> reference);

    RTTI::Value create() const;
    RTTI::Type getType() const;
};

}}}

/**************************************************************************************************/
#endif
