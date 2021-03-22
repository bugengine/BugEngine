/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/meta/engine/propertyinfo.script.hh>
#include <bugengine/plugin.scripting.package/nodes/package.hh>
#include <bugengine/resource/resourcemanager.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

static const istring s_name("name");

Package::Package(const ifilename& filename, ref< Folder > dataFolder)
    : m_filename(filename)
    , m_context(Arena::packageBuilder(), dataFolder)
    , m_plugins(Arena::packageBuilder())
    , m_nodes(Arena::packageBuilder())
    , m_values(Arena::packageBuilder())
{
    m_context.rootNamespace->add(inamespace("bugengine"), Meta::Value(be_bugengine_Namespace()));
}

Package::~Package()
{
}

void Package::loadPlugin(inamespace plugin, inamespace name)
{
    Plugin::Plugin< void* > p(plugin, Plugin::Plugin< void* >::Preload);
    if(!p)
    {
        be_notreached();
    }
    else
    {
        m_plugins.push_back(p);
        m_context.rootNamespace->add(name, Meta::Value(p.pluginNamespace()));
    }
}

void Package::insertNode(const istring name, ref< Meta::AST::Node > object)
{
    for(minitl::vector< ref< Meta::AST::Node > >::iterator it = m_nodes.begin();
        it != m_nodes.end(); ++it)
    {
        if(*it == object)
        {
            be_error("Object added twice");
            return;
        }
    }
    m_nodes.push_back(object);
    object->setMetadata(s_name, name);
    m_context.rootNamespace->add(inamespace(name), object);
}

void Package::removeNode(ref< Meta::AST::Node > object)
{
    istring name = object->getMetadata(s_name).as< const istring >();
    m_context.rootNamespace->remove(inamespace(name), object);

    minitl::vector< ref< Meta::AST::Node > >::iterator it = m_nodes.begin();
    while(it != m_nodes.end())
    {
        if(*it == object)
        {
            m_nodes.erase(it);
            return;
        }
        ++it;
    }
    be_error("Object does not exist");
}

ref< Meta::AST::Node > Package::findByName(istring name) const
{
    return m_context.rootNamespace->getNode(name);
}

const Meta::Value& Package::getValue(weak< const Meta::AST::Node > object) const
{
    size_t index = 0;
    for(minitl::vector< ref< Meta::AST::Node > >::const_iterator it = m_nodes.begin();
        it != m_nodes.end(); ++it, ++index)
    {
        if((*it) == object)
        {
            be_assert_recover(index < m_values.size(), "access to a value not yet created",
                              return m_empty);
            be_assert(m_values[index], "access to a value not yet created");
            return m_values[index];
        }
    }
    return m_empty;
}

void Package::createObjects(weak< Resource::ResourceManager > manager)
{
    m_values.resize(m_nodes.size());
    for(size_t i = 0; i < m_nodes.size(); ++i)
    {
        m_nodes[i]->eval(be_type< void >(), m_values[i]);
        if(m_values[i].isA(be_type< const Resource::Description >()))
        {
            manager->load(m_values[i].type().metaclass,
                          m_values[i].as< weak< const Resource::Description > >());
        }
    }
}

void Package::deleteObjects(weak< Resource::ResourceManager > manager)
{
    for(size_t i = m_values.size(); i > 0; --i)
    {
        if(m_values.back().isA(be_type< const Resource::Description >()))
        {
            manager->unload(m_values.back().type().metaclass,
                            m_values.back().as< weak< const Resource::Description > >());
        }
        m_values.pop_back();
    }
}

void Package::diffFromPackage(weak< Package > previous,
                              weak< Resource::ResourceManager > /*manager*/)
{
    minitl::swap(previous->m_values, m_values);
    // for(size_t i = 0; i < m_nodes.size(); ++i)
    //{
    //    be_forceuse(manager);
    //    be_forceuse(previous);
    //}
}

const ifilename& Package::filename() const
{
    return m_filename;
}

bool Package::success() const
{
    return m_context.errorCount == 0;
}

void Package::resolve()
{
    for(minitl::vector< ref< Meta::AST::Node > >::const_iterator it = m_nodes.begin();
        it != m_nodes.end(); ++it)
    {
        (*it)->resolve(m_context);
    }
}

}}}  // namespace BugEngine::PackageBuilder::Nodes
