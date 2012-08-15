/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>
#include    <package/package.script.hh>
#include    <filesystem/diskfolder.script.hh>


namespace BugEngine { namespace Editor
{

Editor::Editor(const PluginContext& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
    ,   m_renderer("graphics.GL4", pluginContext())
    ,   m_packageManager("scripting.package", pluginContext())
    ,   m_luaScripting("scripting.lua", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(istring("main.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

Editor::~Editor()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}}
