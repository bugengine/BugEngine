/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_REFERENCE_HH_
#define BE_RTTIPARSE_NODES_REFERENCE_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/valueparse.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class Namespace;
class Object;

class Reference : public Node
{
    friend class Object;
private:
    const inamespace        m_referenceName;
    inamespace              m_properties;
    Type                    m_cachedType;
    weak<const Namespace>   m_cachedNamespace;
    ref<const Node>         m_cachedNode;
protected:
    virtual bool resolve(DbContext& context) override;
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;
public:
    Reference(const ParseLocation& location, const inamespace& name);
    ~Reference();
};

}}}

/**************************************************************************************************/
#endif
