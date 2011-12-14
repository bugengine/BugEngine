/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>

namespace BugEngine { namespace Editor
{

Editor::Editor(const PluginContext& context)
    :   m_resourceManager(scoped<ResourceManager>::create(gameArena()))
    ,   m_pluginContext(m_resourceManager)
    ,   m_packageBuilder("packagebuilder", m_pluginContext)
{
    Plugin<void> p("shadermodel4", Plugin<void>::Preload);
}

Editor::~Editor()
{
}

}}
