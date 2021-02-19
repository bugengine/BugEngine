/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_INTROSPECTIONHINT_HH_
#define BE_RTTI_AST_INTROSPECTIONHINT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti/engine/call.hh>

namespace BugEngine { namespace RTTI { namespace AST {

static inline RTTI::ConversionCost calculateConversion(weak< const Node > node,
                                                       const RTTI::Type&  other)
{
    return node->distance(other);
}

static inline void convert(weak< const Node > node, void* buffer, RTTI::Type type)
{
    new(buffer) Value(node->eval(type));
}

class Object;
class Node;
typedef RTTI::ArgInfo< weak< const Node > > ArgInfo;

class be_api(RTTI_AST) IntrospectionHint : public minitl::refcountable
{
protected:
    weak< const Object > m_owner;
    CallInfo             m_callInfo;
    u32                  m_argumentThis;

public:
    IntrospectionHint(weak< const Object > owner, const CallInfo& callInfo, u32 argumentThis);
    virtual ~IntrospectionHint();

    virtual ConversionCost calculateConversion(const Type& targetType) const;
    virtual Type           getType() const;
    virtual bool           getPropertyType(DbContext & context, const istring propertyName,
                                           Type& propertyType) const;
    virtual Value          call(const ArgInfo arguments[], u32 argumentCount) const;
    virtual minitl::tuple< minitl::raw< const Method >, bool > getCall(DbContext & context) const;
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
