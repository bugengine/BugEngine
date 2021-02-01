/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_AST_INT2_HH_
#define BE_RTTI_AST_INT2_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/kernel/simd.hh>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Int2 : public Node
{
private:
    const bigint2 m_value;

protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Int2(const ParseLocation& location, bigint2 value);
    ~Int2();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
