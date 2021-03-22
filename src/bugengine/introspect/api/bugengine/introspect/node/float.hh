/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_NODE_FLOAT_HH_
#define BE_INTROSPECT_NODE_FLOAT_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/node.hh>

namespace BugEngine { namespace Meta { namespace AST {

class be_api(INTROSPECT) Float : public Node
{
private:
    const double m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Float(double value);
    ~Float();
};

}}}  // namespace BugEngine::Meta::AST

/**************************************************************************************************/
#endif
