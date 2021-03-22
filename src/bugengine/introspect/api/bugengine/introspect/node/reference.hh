/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_NODE_REFERENCE_HH_
#define BE_INTROSPECT_NODE_REFERENCE_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/node.hh>

namespace BugEngine { namespace Meta { namespace AST {

class Namespace;

class be_api(INTROSPECT) Reference : public Node
{
    friend class Object;

private:
    const inamespace  m_referenceName;
    ref< const Node > m_node;
    Meta::Value       m_value;

protected:
    virtual minitl::tuple< raw< const Meta::Method >, bool > getCall(DbContext & context)
        const override;
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool           doResolve(DbContext & context) override;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Reference(const inamespace& name);
    ~Reference();

    const inamespace& name() const
    {
        return m_referenceName;
    }
};

}}}  // namespace BugEngine::Meta::AST

/**************************************************************************************************/
#endif
