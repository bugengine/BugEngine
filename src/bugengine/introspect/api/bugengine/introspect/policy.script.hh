/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_POLICY_SCRIPT_HH_
#define BE_INTROSPECT_POLICY_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/dbcontext.hh>
#include <bugengine/introspect/introspectionhint.hh>

namespace BugEngine { namespace Meta {

struct CallInfo;

namespace AST {

class Object;

struct be_api(INTROSPECT) Policy
{
public:
    virtual ~Policy();
    virtual ref< IntrospectionHint > verify(Meta::AST::DbContext & context,
                                            weak< const Meta::AST::Object > object,
                                            const CallInfo& callInfo, u32 argumentThis) const;
};

}  // namespace AST
}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
