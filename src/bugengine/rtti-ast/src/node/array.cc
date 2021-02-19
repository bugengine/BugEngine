/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/array.hh>

#include <bugengine/minitl/array.hh>
#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/engine/scriptingapi.hh>

namespace BugEngine { namespace RTTI { namespace AST {

static const istring value_type = istring("value_type");

Array::Array(const minitl::vector< ref< Node > >& value) : Node(), m_value(value)
{
}

Array::~Array()
{
}

ConversionCost Array::distance(const Type& type) const
{
    if(type.metaclass->type() == RTTI::ClassType_Array)
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
        return RTTI::ConversionCost::s_incompatible;
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

void Array::doEval(const RTTI::Type& expectedType, Value& result) const
{
    RTTI::Type valueType = expectedType.metaclass->apiMethods->arrayScripting->value_type;

    minitl::array< RTTI::Value > v(Arena::temporary(), be_checked_numcast< u32 >(m_value.size()));
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

}}}  // namespace BugEngine::RTTI::AST
