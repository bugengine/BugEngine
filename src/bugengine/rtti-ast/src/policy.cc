/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/policy.script.hh>

#include <bugengine/rtti-ast/introspectionhint.hh>
#include <bugengine/rtti-ast/node/object.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Policy::~Policy()
{
}

ref< IntrospectionHint > Policy::verify(RTTI::AST::DbContext&           context,
                                        weak< const RTTI::AST::Object > object,
                                        const CallInfo& callInfo, u32 argumentThis) const
{
    be_forceuse(context);
    be_forceuse(object);
    return ref< IntrospectionHint >::create(Arena::rtti(), object, callInfo, argumentThis);
}

}}}  // namespace BugEngine::RTTI::AST
