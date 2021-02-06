/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_INT4_HH_
#define BE_RTTI_AST_INT4_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/kernel/simd.hh>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Int4 : public Node
{
private:
    const bigint4 m_value;

protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Int4(const ParseLocation& location, bigint4 value);
    ~Int4();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
