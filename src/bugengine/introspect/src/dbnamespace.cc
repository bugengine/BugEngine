/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/dbcontext.hh>
#include <bugengine/introspect/dbnamespace.hh>
#include <bugengine/introspect/node/object.hh>

namespace BugEngine { namespace Meta { namespace AST {

Namespace::Namespace(minitl::Allocator& allocator)
    : m_allocator(allocator)
    , m_children(m_allocator)
    , m_nodes(m_allocator)
    , m_value()
{
}

Namespace::Namespace(minitl::Allocator& allocator, const Value& value)
    : m_allocator(allocator)
    , m_children(m_allocator)
    , m_nodes(m_allocator)
    , m_value(value)
{
}

Namespace::~Namespace()
{
}

void Namespace::add(const inamespace& name, const Value& value)
{
    weak< Namespace > current = this;
    for(u32 i = 0; i < name.size(); ++i)
    {
        istring                                                                       n = name[i];
        minitl::tuple< minitl::hashmap< istring, ref< Namespace > >::iterator, bool > result
            = current->m_children.insert(n, ref< Namespace >());
        if(result.second)
        {
            result.first->second
                = ref< Namespace >::create(current->m_allocator, byref(current->m_allocator));
        }
        current = result.first->second;
    }
    current->m_value = value;
}

void Namespace::add(const inamespace& name, ref< Namespace > ns)
{
    weak< Namespace > current = this;
    if(name.size())
    {
        for(u32 i = 0; i < name.size() - 1; ++i)
        {
            istring n = name[i];
            minitl::tuple< minitl::hashmap< istring, ref< Namespace > >::iterator, bool > result
                = current->m_children.insert(n, ref< Namespace >());
            if(result.second)
            {
                result.first->second
                    = ref< Namespace >::create(current->m_allocator, byref(current->m_allocator));
            }
        }
        current->m_children[name[name.size() - 1]] = ns;
    }
}

void Namespace::add(const inamespace& name, ref< Node > node)
{
    weak< Namespace > current = this;
    if(name.size())
    {
        for(u32 i = 0; i < name.size() - 1; ++i)
        {
            istring n = name[i];
            minitl::tuple< minitl::hashmap< istring, ref< Namespace > >::iterator, bool > result
                = current->m_children.insert(n, ref< Namespace >());
            if(result.second)
            {
                result.first->second
                    = ref< Namespace >::create(current->m_allocator, byref(current->m_allocator));
            }
        }
        current->m_nodes[name[name.size() - 1]] = node;
    }
}

void Namespace::remove(const inamespace& name, ref< Node > node)
{
    weak< Namespace > current = this;
    if(name.size())
    {
        for(u32 i = 0; i < name.size() - 1; ++i)
        {
            current = current->getChild(name[i]);
            be_assert_recover(current,
                              "could not remove object %s: unable to find child namespace %s" | name
                                  | name[i],
                              return;);
        }
        be_assert(node == current->m_nodes[name[name.size() - 1]],
                  "node %s does not match the node to remove" | name);
        current->m_nodes.erase(current->m_nodes.find(name[name.size() - 1]));
    }
}

ref< Namespace > Namespace::getChild(const istring name) const
{
    minitl::hashmap< istring, ref< Namespace > >::const_iterator it = m_children.find(name);
    if(it != m_children.end())
    {
        return it->second;
    }
    else
    {
        return ref< Namespace >();
    }
}

ref< Node > Namespace::getNode(const istring name) const
{
    minitl::hashmap< istring, ref< Node > >::const_iterator it = m_nodes.find(name);
    if(it != m_nodes.end())
    {
        return it->second;
    }
    else
    {
        return ref< Object >();
    }
}

const Value& Namespace::getValue() const
{
    return m_value;
}

}}}  // namespace BugEngine::Meta::AST
