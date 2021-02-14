/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_OBJECT_HH_
#define BE_RTTI_AST_OBJECT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti/engine/call.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI { namespace AST {

typedef RTTI::ArgInfo< weak< const Node > > ArgInfo;

class Reference;

struct Parameter
{
    istring     name;
    ref< Node > value;
};

class be_api(RTTI_AST) Object : public Node
{
private:
    const ref< Reference >            m_className;
    const minitl::vector< Parameter > m_parameters;
    minitl::vector< ArgInfo >         m_arguments;
    CallInfo                          m_callInfo;
    u32                               m_argumentThis;

protected:
    virtual minitl::tuple< raw< const RTTI::Method >, bool > getCall(DbContext & context)
        const override;
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool           doResolve(DbContext & context) override;
    virtual bool isCompatible(DbContext & context, const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Object(ref< Reference > className, const minitl::vector< Parameter >& parameters);
    ~Object();
    Type getType() const;
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
