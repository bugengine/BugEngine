/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_NODE_ARRAY_HH_
#define BE_INTROSPECT_NODE_ARRAY_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/node.hh>

namespace BugEngine { namespace Meta { namespace AST {

class be_api(INTROSPECT) Array : public Node
{
private:
    const minitl::vector< ref< Node > > m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool           doResolve(DbContext & context) override;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Array(const minitl::vector< ref< Node > >& value);
    ~Array();
};

}}}  // namespace BugEngine::Meta::AST

/**************************************************************************************************/
#endif
