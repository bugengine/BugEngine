/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_FLOAT3_HH_
#define BE_RTTI_AST_FLOAT3_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/kernel/simd.hh>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Float3 : public Node
{
private:
    const float3 m_value;

protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Float3(const ParseLocation& location, float3 value);
    ~Float3();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
