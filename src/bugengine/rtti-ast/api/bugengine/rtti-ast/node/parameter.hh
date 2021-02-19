/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_NODE_PARAMETER_HH_
#define BE_RTTI_AST_NODE_PARAMETER_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Parameter : public Node
{
private:
    const istring m_name;
    ref< Node >   m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool           doResolve(DbContext & context) override;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Parameter(const istring name, ref< Node > value);
    ~Parameter();

    const istring name() const
    {
        return m_name;
    }
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
