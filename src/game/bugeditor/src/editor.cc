/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>
#include    <package/package.script.hh>
#include    <system/file/diskfolder.script.hh>
#include    <system/scheduler/task/method.hh>
#include    <world/world.script.hh>

namespace BugEngine { namespace Editor
{

Editor::Editor(const PluginContext& /*context*/)
    :   m_resourceManager(scoped<ResourceManager>::create(gameArena()))
    ,   m_pluginContext(m_resourceManager)
    ,   m_renderer("nullrender", m_pluginContext)
    ,   m_packageManager("package", m_pluginContext)
    ,   m_dataFolder(ref<DiskFolder>::create(gameArena(), Environment::getEnvironment().getDataDirectory()))
    ,   m_mainPackage(ref<Package>::create(gameArena(), m_dataFolder->openFile(istring("main.pkg"))))
    ,   m_resourceTask(ref< Task< MethodCaller<ResourceManager, &ResourceManager::updateTickets> > >::create(taskArena(), "resource", color32(0,255,0), MethodCaller<ResourceManager, &ResourceManager::updateTickets>(m_resourceManager)))
{
    m_resourceManager->attach<World>(this);
    addTask(m_resourceTask);
    m_resourceManager->load(m_mainPackage);
}

Editor::~Editor()
{
    m_resourceManager->unload(m_mainPackage);
    m_resourceManager->detach<World>(this);
    removeTask(m_resourceTask);
}

}}
