/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_AST_BOOL_HH_
#define BE_RTTI_AST_BOOL_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Bool : public Node
{
private:
    const bool m_value;

protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Bool(const ParseLocation& location, bool value);
    ~Bool();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
