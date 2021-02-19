/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_NODE_PROPERTY_HH_
#define BE_RTTI_AST_NODE_PROPERTY_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti-ast/node/object.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Property : public Node
{
private:
    const ref< const Object > m_owner;
    const inamespace          m_propertyName;
    RTTI::Type                m_type;

protected:
    virtual minitl::tuple< raw< const RTTI::Method >, bool > getCall(DbContext & context)
        const override;
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool           doResolve(DbContext & context) override;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Property(ref< const Object > owner, const inamespace& propertyName);
    ~Property();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
