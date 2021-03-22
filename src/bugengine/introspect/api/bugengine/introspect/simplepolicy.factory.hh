/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_SIMPLEPOLICY_FACTORY_HH_
#define BE_INTROSPECT_SIMPLEPOLICY_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/policy.script.hh>

#include <bugengine/introspect/node/object.hh>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/helper/method.hh>
#include <bugengine/meta/typeinfo.hh>

namespace BugEngine { namespace Meta {

namespace AST {

template < typename INTROSPECTION_HINT >
struct SimplePolicy : public Policy
{
public:
    virtual ref< IntrospectionHint > verify(Meta::AST::DbContext&           context,
                                            weak< const Meta::AST::Object > object,
                                            const CallInfo& callInfo, u32 argumentThis) const
    {
        be_forceuse(context);
        return ref< INTROSPECTION_HINT >::create(Arena::meta(), object, callInfo, argumentThis);
    }
};

}  // namespace AST

template < typename INTROSPECTION_HINT >
struct ClassID< AST::SimplePolicy< INTROSPECTION_HINT > >
{
    static BE_EXPORT raw< const Meta::Class > klass()
    {
        static const Meta::Class s_class
            = {istring("SimplePolicy"),
               u32(sizeof(AST::SimplePolicy< INTROSPECTION_HINT >)),
               0,
               Meta::ClassType_Object,
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
        raw< const Meta::Class > result = {&s_class};
        return result;
    }
};

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
