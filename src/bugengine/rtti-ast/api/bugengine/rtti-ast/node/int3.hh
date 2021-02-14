/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_INT3_HH_
#define BE_RTTI_AST_INT3_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/kernel/simd.hh>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Int3 : public Node
{
private:
    const bigint3 m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool isCompatible(DbContext & context, const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Int3(bigint3 value);
    ~Int3();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
