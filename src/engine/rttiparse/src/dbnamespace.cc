/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <rttiparse/dbnamespace.hh>
#include    <rttiparse/dbcontext.hh>
#include    <rttiparse/valueparse.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

Namespace::Namespace(minitl::Allocator& allocator)
    :   m_allocator(allocator)
    ,   m_children(m_allocator)
    ,   m_nodes(m_allocator)
    ,   m_value()
{
}

Namespace::Namespace(minitl::Allocator& allocator, const Value& value)
    :   m_allocator(allocator)
    ,   m_children(m_allocator)
    ,   m_nodes(m_allocator)
    ,   m_value(value)
{
}

Namespace::~Namespace()
{
}

void Namespace::add(DbContext& context, const inamespace& name, const Value& value)
{
    be_forceuse(context);
    weak<Namespace> current = this;
    for (u32 i = 0; i < name.size(); ++i)
    {
        istring n = name[i];
        ref<Namespace> newNamespace = ref<Namespace>::create(current->m_allocator,
                                                             byref(current->m_allocator));
        current = current->m_children.insert(n, newNamespace).first->second;
    }
    current->m_value = value;
}

void Namespace::add(DbContext& context, const inamespace& name, ref<Namespace> ns)
{
    be_forceuse(context);
    weak<Namespace> current = this;
    if (name.size())
    {
        for (u32 i = 0; i < name.size() - 1; ++i)
        {
            istring n = name[i];
            ref<Namespace> newNamespace = ref<Namespace>::create(current->m_allocator,
                                                                 byref(current->m_allocator));
            current = current->m_children.insert(n, newNamespace).first->second;
        }
        current->m_children[name[name.size() - 1]] = ns;
    }
}

void Namespace::add(DbContext& context, const inamespace& name, ref<Node> node)
{
    be_forceuse(context);
    weak<Namespace> current = this;
    if (name.size())
    {
        for (u32 i = 0; i < name.size() - 1; ++i)
        {
            istring n = name[i];
            ref<Namespace> newNamespace = ref<Namespace>::create(current->m_allocator,
                                                                 byref(current->m_allocator));
            current = current->m_children.insert(n, newNamespace).first->second;
        }
        current->m_nodes[name[name.size() - 1]] = node;
    }
}

ref<Namespace> Namespace::getChild(const istring name) const
{
    minitl::hashmap<istring, ref<Namespace> >::const_iterator it = m_children.find(name);
    if (it != m_children.end())
    {
        return it->second;
    }
    else
    {
        return ref<Namespace>();
    }
}

ref<Node> Namespace::getNode(const istring name) const
{
    minitl::hashmap<istring, ref<Node> >::const_iterator it = m_nodes.find(name);
    if (it != m_nodes.end())
    {
        return it->second;
    }
    else
    {
        return ref<Node>();
    }
}

const Value& Namespace::getValue() const
{
    return m_value;
}

}}}
