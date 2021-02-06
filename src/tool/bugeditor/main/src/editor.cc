/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/filesystem/diskfolder.script.hh>
#include <bugengine/plugin.scripting.package/package.script.hh>
#include <editor.hh>

namespace BugEngine { namespace Editor {

Editor::Editor(const Plugin::Context& context)
    : Application(
        ref< DiskFolder >::create(Arena::game(), Environment::getEnvironment().getDataDirectory()),
        context.resourceManager, context.scheduler)
    , m_renderer("plugin.graphics.GL4", pluginContext())
    , m_packageManager("plugin.scripting.package", pluginContext())
    , m_luaScripting("plugin.scripting.lua", pluginContext())
    , m_mainPackage(ref< Package >::create(
          Arena::game(), pluginContext().dataFolder->openFile(istring("main.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

Editor::~Editor()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}}  // namespace BugEngine::Editor
