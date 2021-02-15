/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_REFERENCE_HH_
#define BE_RTTI_AST_REFERENCE_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class Namespace;

class be_api(RTTI_AST) Reference : public Node
{
    friend class Object;

private:
    const inamespace  m_referenceName;
    ref< const Node > m_node;
    RTTI::Value       m_value;

protected:
    virtual minitl::tuple< raw< const RTTI::Method >, bool > getCall(DbContext & context)
        const override;
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool           doResolve(DbContext & context) override;
    virtual bool isCompatible(DbContext & context, const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Reference(const inamespace& name);
    ~Reference();

    const inamespace& name() const
    {
        return m_referenceName;
    }
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
