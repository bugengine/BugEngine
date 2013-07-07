/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/package.hh>
#include    <package/nodes/reference.hh>
#include    <package/nodes/object.hh>
#include    <resource/resourcemanager.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Package::Namespace::Namespace()
    :   m_values(Arena::packageBuilder())
    ,   m_children(Arena::packageBuilder())
{
}

Package::Namespace::~Namespace()
{
}

RTTI::Value Package::Namespace::get(const inamespace& name) const
{
    minitl::hashmap<istring, RTTI::Value>::const_iterator it = m_values.find(name[0]);
    if (it == m_values.end())
    {
        minitl::hashmap< istring, ref<Namespace> >::const_iterator child = m_children.find(name[0]);
        if (child != m_children.end())
        {
            inamespace childname = name;
            childname.pop_front();
            return child->second->get(childname);
        }
        else
        {
            return RTTI::Value();
        }
    }
    else if (!it->second)
    {
        return RTTI::Value();
    }
    else
    {
        RTTI::Value v(it->second);
        for (u32 i = 1; i < name.size(); ++i)
        {
            v = v[name[i]];
            if (!v)
            {
                return v;
            }
        }
        return v;
    }
}

void Package::Namespace::add(const inamespace& name, const RTTI::Value& value)
{
    if (name.size() == 1)
    {
        bool inserted = m_values.insert(name[0], value).second;
        if (!inserted)
        {
            be_notreached();
        }
    }
    else
    {
        minitl::hashmap< istring, RTTI::Value >::iterator it = m_values.find(name[0]);
        if (it != m_values.end())
        {
            be_notreached();
        }
        minitl::pair< minitl::hashmap< istring, ref<Namespace> >::iterator, bool > result = m_children.insert(name[0], ref<Namespace>());
        if (result.second)
        {
            result.first->second = ref<Namespace>::create(Arena::packageBuilder());
        }
        inamespace childname = name;
        childname.pop_front();
        result.first->second->add(childname, value);
    }
}


Package::Package()
    :   m_plugins(Arena::packageBuilder())
    ,   m_rootNamespace(ref<Namespace>::create(Arena::packageBuilder()))
    ,   m_nodes(Arena::packageBuilder())
    ,   m_values(Arena::packageBuilder())
{
    m_rootNamespace->add(inamespace("game"), RTTI::Value(be_game_Namespace()));
}

Package::~Package()
{
}

void Package::insertNode(ref<Object> object)
{
    for (minitl::vector< ref<Object> >::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
    {
        if (*it == object)
        {
            be_error("Object added twice");
            return;
        }
    }
    m_nodes.push_back(object);
}

void Package::removeNode(ref<Object> object)
{
    minitl::vector< ref<Object> >::iterator it = m_nodes.begin();
    while (it != m_nodes.end())
    {
        if (*it == object)
        {
            m_nodes.erase(it);
            return;
        }
        ++it;
    }
    be_error("Object does not exist");
}

ref<Object> Package::findByName(istring name) const
{
    minitl::vector< ref<Object> >::const_iterator it = m_nodes.begin();
    while (it != m_nodes.end())
    {
        if ((*it)->name() == name)
        {
            return *it;
        }
        ++it;
    }
    return ref<Object>();
}

const RTTI::Value& Package::getValue(weak<const Object> object) const
{
    size_t index = 0;
    for (minitl::vector< ref<Object> >::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it, ++index)
    {
        if ((*it) == object)
        {
            be_assert_recover(index < m_values.size(), "access to a value not yet created", return m_empty);
            be_assert(m_values[index], "access to a value not yet created");
            return m_values[index];
        }
    }
    return m_empty;
}

void Package::loadPlugin(inamespace plugin)
{
    Plugin::Plugin<void*> p(plugin, Plugin::Plugin<void*>::Preload);
    if (!p)
    {
        be_notreached();
    }
    else
    {
        m_plugins.push_back(p);
        m_rootNamespace->add(plugin, RTTI::Value(p.pluginNamespace()));
    }
}

void Package::addReference(weak<Reference> reference)
{
    m_references.push_back(*reference);
    resolveReference(reference);
}

void Package::resolveReference(weak<Reference> reference)
{
    inamespace name = reference->m_name;
    if (name.size())
    {
        reference->m_value = m_rootNamespace->get(name);
        if (!reference->m_value)
        {
            reference->m_object = findByName(name[0]);
        }
    }
}

void Package::binarySave() const
{
}

void Package::textSave() const
{
}

void Package::createObjects(weak<Resource::ResourceManager> manager)
{
    m_values.resize(m_nodes.size());
    for(size_t i = 0; i < m_nodes.size(); ++i)
    {
        m_values[i] = m_nodes[i]->create();
        if (m_values[i].isA(be_typeid<const Resource::Description>::type()))
        {
            manager->load(m_values[i].type().metaclass, m_values[i].as< weak<const Resource::Description> >());
        }
    }
}

void Package::deleteObjects(weak<Resource::ResourceManager> manager)
{
    for(size_t i = m_values.size(); i > 0; --i)
    {
        if (m_values.back().isA(be_typeid<const Resource::Description>::type()))
        {
            manager->unload(m_values.back().type().metaclass, m_values.back().as< weak<const Resource::Description> >());
        }
        m_values.pop_back();
    }
}


void Package::diffFromPackage(weak<Package> previous, weak<Resource::ResourceManager> /*manager*/)
{
    minitl::swap(previous->m_values, m_values);
    //for(size_t i = 0; i < m_nodes.size(); ++i)
    //{
    //    be_forceuse(manager);
    //    be_forceuse(previous);
    //}
}

}}}
