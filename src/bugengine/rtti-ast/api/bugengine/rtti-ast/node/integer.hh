/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_NODE_INTEGER_HH_
#define BE_RTTI_AST_NODE_INTEGER_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Integer : public Node
{
private:
    const i64 m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Integer(i64 value);
    ~Integer();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif