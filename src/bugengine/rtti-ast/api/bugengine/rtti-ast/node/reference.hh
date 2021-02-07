/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_REFERENCE_HH_
#define BE_RTTI_AST_REFERENCE_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class Namespace;

class be_api(RTTI_AST) Reference : public Node
{
    friend class Object;

private:
    const inamespace m_referenceName;
    inamespace       m_properties;

protected:
    virtual bool resolve(DbContext & context) override;
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Reference(const ParseLocation& location, const inamespace& name);
    ~Reference();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
