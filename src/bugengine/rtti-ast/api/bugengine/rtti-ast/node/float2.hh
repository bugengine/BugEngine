/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_AST_FLOAT2_HH_
#define BE_RTTI_AST_FLOAT2_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/kernel/simd.hh>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Float2 : public Node
{
private:
    const float2 m_value;

protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Float2(const ParseLocation& location, float2 value);
    ~Float2();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
