/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>
#include    <package/package.script.hh>
#include    <system/file/diskfolder.script.hh>
#include    <system/scheduler/task/method.hh>
#include    <world/world.script.hh>

#include    <system/scheduler/kernel/kernel.script.hh>

namespace BugEngine { namespace Editor
{

Editor::Editor(const PluginContext& /*context*/)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()))
    ,   m_renderer("graphics.nullrender", pluginContext())
    ,   m_packageManager("scripting.package", pluginContext())
    ,   m_luaScripting("scripting.lua", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(istring("main.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
    ref<Kernel> k = ref<Kernel>::create(Arena::game(), "graphics.3d.batchrender");
    pluginContext().resourceManager->load(k);
}

Editor::~Editor()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}}
