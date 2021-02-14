/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_STRING_HH_
#define BE_RTTI_AST_STRING_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) String : public Node
{
private:
    const char* m_value;

protected:
    virtual ConversionCost distance(const Type& type) const override;
    virtual bool isCompatible(DbContext & context, const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    String(const char* value);
    ~String();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
