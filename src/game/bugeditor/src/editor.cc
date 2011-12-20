/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>
#include    <package/package.script.hh>

namespace BugEngine { namespace Editor
{

Editor::Editor(const PluginContext& context)
    :   m_resourceManager(scoped<ResourceManager>::create(gameArena()))
    ,   m_pluginContext(m_resourceManager)
    ,   m_packageBuilder("package", m_pluginContext)
{
    
}

Editor::~Editor()
{
}

}}
