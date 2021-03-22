/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/policy.script.hh>

#include <bugengine/introspect/introspectionhint.hh>
#include <bugengine/introspect/node/object.hh>

namespace BugEngine { namespace Meta { namespace AST {

Policy::~Policy()
{
}

ref< IntrospectionHint > Policy::verify(Meta::AST::DbContext&           context,
                                        weak< const Meta::AST::Object > object,
                                        const CallInfo& callInfo, u32 argumentThis) const
{
    be_forceuse(context);
    be_forceuse(object);
    return ref< IntrospectionHint >::create(Arena::meta(), object, callInfo, argumentThis);
}

}}}  // namespace BugEngine::Meta::AST
