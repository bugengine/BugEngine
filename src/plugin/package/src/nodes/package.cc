/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/package.hh>
#include    <package/nodes/reference.hh>
#include    <package/nodes/object.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Package::Package()
    :   m_plugins(packageBuilderArena())
    ,   m_imports(packageBuilderArena())
    ,   m_nodes(packageBuilderArena())
{
}

Package::~Package()
{
}

void Package::insertNode(ref<Object> object)
{
    minitl::vector< ref<Object> >::iterator it = m_nodes.begin();
    while (it != m_nodes.end())
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

void Package::loadPlugin(istring plugin)
{
    Plugin<void*> p(plugin, Plugin<void*>::Preload);
    if (!p)
    {
    }
    else
    {
        m_plugins.push_back(p);
        m_imports.insert(std::make_pair(plugin, Value(p.pluginNamespace())));
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
        minitl::hashmap<istring, Value>::const_iterator it = m_imports.find(name[0]);
        if (it == m_imports.end())
        {
        }
        else if (!it->second)
        {
        }
        else
        {
            Value v(it->second);
            for (size_t i = 1; i < name.size(); ++i)
            {
                v = v[name[i]];
                if (!v)
                {
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

}}}
