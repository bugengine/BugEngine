/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_POLICY_SCRIPT_HH_
#define BE_RTTI_AST_POLICY_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti-ast/introspectionhint.hh>

namespace BugEngine { namespace RTTI {

struct CallInfo;

namespace AST {

class Object;

struct be_api(RTTI_AST) Policy
{
public:
    virtual ~Policy();
    virtual ref< IntrospectionHint > verify(RTTI::AST::DbContext & context,
                                            weak< const RTTI::AST::Object > object,
                                            const CallInfo& callInfo, u32 argumentThis) const;
};

}  // namespace AST
}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
