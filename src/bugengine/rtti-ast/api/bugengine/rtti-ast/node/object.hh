/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_NODE_OBJECT_HH_
#define BE_RTTI_AST_NODE_OBJECT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

#include <bugengine/rtti-ast/introspectionhint.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class Reference;
class Parameter;
class IntrospectionHint;

class be_api(RTTI_AST) Object : public Node
{
private:
    const ref< Reference >                   m_className;
    const minitl::vector< ref< Parameter > > m_parameters;
    minitl::vector< ArgInfo >                m_arguments;
    ref< IntrospectionHint >                 m_introspectionHint;

private:
    bool resolveInternal(DbContext & context);

protected:
    virtual minitl::tuple< raw< const RTTI::Method >, bool > getCall(DbContext & context)
        const override;
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool           doResolve(DbContext & context) override;
    virtual ref< Node >    getProperty(DbContext & context, const inamespace& propertyName) const;
    virtual void           doEval(const Type& expectedType, Value& result) const override;
    virtual void           doVisit(Node::Visitor & visitor) const override;

public:
    Object(ref< Reference > className, const minitl::vector< ref< Parameter > >& parameters);
    ~Object();

    Type getType() const;
    bool getPropertyType(DbContext & context, const istring propertyName, Type& propertyType) const;

    weak< const Parameter > getParameter(const istring name) const;
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
