/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/array.hh>

#include <bugengine/introspect/dbcontext.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/engine/scriptingapi.hh>
#include <bugengine/minitl/array.hh>

namespace BugEngine { namespace Meta { namespace AST {

static const istring value_type = istring("value_type");

Array::Array(const minitl::vector< ref< Node > >& value) : Node(), m_value(value)
{
}

Array::~Array()
{
}

ConversionCost Array::distance(const Type& type) const
{
    if(type.metaclass->type() == Meta::ClassType_Array)
    {
        ConversionCost                 result = ConversionCost();
        raw< const ScriptingArrayAPI > api    = type.metaclass->apiMethods->arrayScripting;
        for(minitl::vector< ref< Node > >::const_iterator it = m_value.begin(); it != m_value.end();
            ++it)
        {
            ConversionCost itemCost = (*it)->distance(api->value_type);
            if(itemCost > result) result = itemCost;
        }
        return result;
    }
    else
    {
        return Meta::ConversionCost::s_incompatible;
    }
}

bool Array::doResolve(DbContext& context)
{
    bool result = true;
    for(u32 i = 0; i < m_value.size(); ++i)
    {
        result = result & m_value[i]->resolve(context);
    }
    return result;
}

void Array::doEval(const Meta::Type& expectedType, Value& result) const
{
    Meta::Type valueType = expectedType.metaclass->apiMethods->arrayScripting->value_type;

    minitl::array< Meta::Value > v(Arena::temporary(), be_checked_numcast< u32 >(m_value.size()));
    for(u32 i = 0; i < m_value.size(); ++i)
    {
        m_value[i]->doEval(valueType, v[i]);
    }
    result = expectedType.metaclass->constructor->doCall(v.begin(),
                                                         be_checked_numcast< u32 >(m_value.size()));
}

void Array::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
