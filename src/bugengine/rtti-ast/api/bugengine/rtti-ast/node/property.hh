/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_PROPERTY_HH_
#define BE_RTTI_AST_PROPERTY_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti-ast/node/object.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Property : public Node
{
private:
    const weak< const Node > m_owner;
    const inamespace         m_propertyName;

protected:
    virtual minitl::tuple< raw< const RTTI::Method >, bool > getCall(DbContext & context)
        const override;
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool           doResolve(DbContext & context) override;
    virtual bool isCompatible(DbContext & context, const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Property(weak< const Object > owner, const inamespace& propertyName);
    ~Property();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
