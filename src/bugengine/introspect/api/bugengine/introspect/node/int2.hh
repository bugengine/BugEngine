/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_NODE_INT2_HH_
#define BE_INTROSPECT_NODE_INT2_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/node.hh>
#include <bugengine/kernel/simd.hh>

namespace BugEngine { namespace Meta { namespace AST {

class be_api(INTROSPECT) Int2 : public Node
{
private:
    const bigint2 m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Int2(bigint2 value);
    ~Int2();
};

}}}  // namespace BugEngine::Meta::AST

/**************************************************************************************************/
#endif
