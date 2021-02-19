/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_SIMPLEPOLICY_FACTORY_HH_
#define BE_RTTI_AST_SIMPLEPOLICY_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/policy.script.hh>

#include <bugengine/rtti-ast/node/object.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/helper/method.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine { namespace RTTI {

namespace AST {

template < typename INTROSPECTION_HINT >
struct SimplePolicy : public Policy
{
public:
    virtual ref< IntrospectionHint > verify(RTTI::AST::DbContext&           context,
                                            weak< const RTTI::AST::Object > object,
                                            const CallInfo& callInfo, u32 argumentThis) const
    {
        be_forceuse(context);
        return ref< INTROSPECTION_HINT >::create(Arena::rtti(), object, callInfo, argumentThis);
    }
};

}  // namespace AST

template < typename INTROSPECTION_HINT >
struct ClassID< AST::SimplePolicy< INTROSPECTION_HINT > >
{
    static BE_EXPORT raw< const RTTI::Class > klass()
    {
        static const RTTI::Class s_class
            = {istring("SimplePolicy"),
               u32(sizeof(AST::SimplePolicy< INTROSPECTION_HINT >)),
               0,
               RTTI::ClassType_Object,
               {0},
               {be_class< AST::Policy >().m_ptr},
               {0},
               {0},
               {0, 0},
               {0, 0},
               {0},
               {0},
               &wrap< AST::SimplePolicy< INTROSPECTION_HINT > >::copy,
               &wrap< AST::SimplePolicy< INTROSPECTION_HINT > >::destroy};
        raw< const RTTI::Class > result = {&s_class};
        return result;
    }
};

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
