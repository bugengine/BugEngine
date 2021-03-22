/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_NODE_BOOL_HH_
#define BE_INTROSPECT_NODE_BOOL_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/node.hh>

namespace BugEngine { namespace Meta { namespace AST {

class be_api(INTROSPECT) Bool : public Node
{
private:
    const bool m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Bool(bool value);
    ~Bool();
};

}}}  // namespace BugEngine::Meta::AST

/**************************************************************************************************/
#endif
