/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>

namespace BugEngine { namespace Editor
{

Editor::Editor(weak<Application> application)
    :   m_application(application)
    ,   m_resourceManager(scoped<ResourceManager>::create(gameArena()))
    ,   m_packageBuilder("packagebuilder", weak<ResourceManager>(m_resourceManager))
{
    Plugin<void> p("shadermodel4", Plugin<void>::Preload);
}

Editor::~Editor()
{
}

}}
