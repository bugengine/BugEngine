/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/reference.hh>

#include <bugengine/rtti-ast/node/object.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Reference::Reference(const ParseLocation& location, const inamespace& name)
    : Node(location)
    , m_referenceName(name)
{
}

Reference::~Reference()
{
}

bool Reference::resolve(DbContext& context)
{
    weak< const Namespace > ns = context.rootNamespace;
    m_properties               = m_referenceName;
    while(m_properties.size())
    {
        istring                 n     = m_properties.pop_front();
        weak< const Namespace > child = ns->getChild(n);
        if(child)
        {
            ns = child;
            continue;
        }
        m_cachedNamespace = ns;
        m_cachedNode      = ns->getNode(n);
        if(m_cachedNode)
        {
            m_cachedType = m_cachedNode->getType();
        }
        else
        {
            const Value& v = ns->getValue();
            m_cachedType   = v.type();
        }
    }
    return true;
}

bool Reference::isCompatible(const Type& expectedType) const
{
    be_forceuse(expectedType);
    return true;
}

void Reference::doEval(const Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    be_forceuse(result);
}

}}}  // namespace BugEngine::RTTI::AST
