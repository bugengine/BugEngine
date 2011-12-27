/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/package.hh>
#include    <package/nodes/reference.hh>
#include    <package/nodes/object.hh>
#include    <system/resource/resourcemanager.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Package::Package()
    :   m_plugins(packageBuilderArena())
    ,   m_imports(packageBuilderArena())
    ,   m_nodes(packageBuilderArena())
    ,   m_values(packageBuilderArena())
{
    m_imports.insert(std::make_pair("game", BugEngine::Value(be_game_Namespace())));
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

const BugEngine::Value& Package::getValue(weak<const Object> object) const
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

void Package::loadPlugin(istring plugin)
{
    Plugin<void*> p(plugin, Plugin<void*>::Preload);
    if (!p)
    {
    }
    else
    {
        m_plugins.push_back(p);
        m_imports.insert(std::make_pair(plugin, BugEngine::Value(p.pluginNamespace())));
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
        minitl::hashmap<istring, BugEngine::Value>::const_iterator it = m_imports.find(name[0]);
        if (it == m_imports.end())
        {
            if (name.size() == 1) //TODO: namespaces
            {
                reference->m_object = findByName(name[0]);
            }
        }
        else if (!it->second)
        {
            be_notreached();
        }
        else
        {
            BugEngine::Value v(it->second);
            for (size_t i = 1; i < name.size(); ++i)
            {
                v = v[name[i]];
                if (!v)
                {
                    be_notreached();
                    return;
                }
            }
            reference->m_value = v;
        }
    }
}

void Package::binarySave() const
{
}

void Package::textSave() const
{
}

void Package::createObjects(weak<const ResourceManager> manager)
{
    m_values.resize(m_nodes.size());
    for(size_t i = 0; i < m_nodes.size(); ++i)
    {
        m_values[i] = m_nodes[i]->create();
        if (be_typeid<const Resource>::type() <= m_values[i].type())
        {
            manager->load(m_values[i].type().metaclass, m_values[i].as< weak<const Resource> >());
        }
    }
}

void Package::deleteObjects(weak<const ResourceManager> manager)
{
    m_values.resize(m_nodes.size());
    for(size_t i = m_nodes.size(); i > 0; --i)
    {
        if (be_typeid<const Resource>::type() <= m_values[i-1].type())
        {
            manager->unload(m_values[i-1].type().metaclass, m_values[i-1].as< weak<const Resource> >());
        }
    }
    m_values.clear();
}

}}}
