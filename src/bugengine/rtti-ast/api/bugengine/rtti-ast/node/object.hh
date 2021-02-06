/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_OBJECT_HH_
#define BE_RTTI_AST_OBJECT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class Reference;

struct Parameter
{
    ParseLocation location;
    istring       name;
    ref< Node >   value;
};

class be_api(RTTI_AST) Object : public Node
{
private:
    const ref< Reference >            m_className;
    const minitl::vector< Parameter > m_parameters;

protected:
    virtual bool resolve(DbContext & context) override;
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Object(const ParseLocation& location, ref< Reference > className,
           const minitl::vector< Parameter >& parameters);
    ~Object();
    Type getType() const;
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
