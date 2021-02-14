/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_FLOAT_HH_
#define BE_RTTI_AST_FLOAT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Float : public Node
{
private:
    const double m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool isCompatible(DbContext & context, const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Float(double value);
    ~Float();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
