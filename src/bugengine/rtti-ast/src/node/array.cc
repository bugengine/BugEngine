/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/array.hh>

#include <bugengine/minitl/array.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>

namespace BugEngine { namespace RTTI { namespace AST {

static const istring value_type = istring("value_type");

Array::Array(const minitl::vector< ref< Node > >& value) : Node(), m_value(value)
{
}

Array::~Array()
{
}

bool Array::resolve(DbContext& context)
{
    bool result = true;
    for(u32 i = 0; i < m_value.size(); ++i)
    {
        result = result | m_value[i]->resolve(context);
    }
    return result;
}

bool Array::isCompatible(const Type& expectedType) const
{
    if(expectedType.metaclass->type() == RTTI::ClassType_Array)
    {
        raw< const RTTI::ObjectInfo > o = expectedType.metaclass->getStaticProperty(value_type);
        if(!o)
        {
            be_error("type %s does not have a %s property" | expectedType.name() | value_type);
        }
        else if(!o->value.type().isA(be_type< const RTTI::Type >()))
        {
            be_error("type %s, property %s does not have the right type; expected %s, got %s"
                     | expectedType.name() | value_type | be_type< const RTTI::Type >().name()
                     | o->value.type().name());
        }
        else
        {
            RTTI::Type valueType = o->value.as< const RTTI::Type >();
            for(minitl::vector< ref< Node > >::const_iterator it = m_value.begin();
                it != m_value.end(); ++it)
            {
                if(!(*it)->isCompatible(valueType))
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void Array::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from array to %s" | expectedType);
    raw< const RTTI::ObjectInfo > o = expectedType.metaclass->getStaticProperty(value_type);
    be_assert(o, "unable to find the array value type");
    RTTI::Type valueType = o->value.as< const RTTI::Type >();

    minitl::array< RTTI::Value > v(Arena::temporary(), be_checked_numcast< u32 >(m_value.size()));
    for(u32 i = 0; i < m_value.size(); ++i)
    {
        m_value[i]->doEval(valueType, v[i]);
    }
    result = expectedType.metaclass->constructor->doCall(v.begin(),
                                                         be_checked_numcast< u32 >(m_value.size()));
}

}}}  // namespace BugEngine::RTTI::AST
