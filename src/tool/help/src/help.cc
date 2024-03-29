/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/filesystem/diskfolder.script.hh>
#include <help.hh>

namespace BugEngine {

Help::Help(const Plugin::Context& context)
    : Application(
        ref< DiskFolder >::create(Arena::game(), Environment::getEnvironment().getDataDirectory()),
        context.resourceManager, context.scheduler)
    , m_packageManager("plugin.scripting.package", pluginContext())
    , m_ui("plugin.ui.console", pluginContext())
    , m_mainPackage(ref< Package >::create(
          Arena::game(), pluginContext().dataFolder->openFile(istring("help.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

Help::~Help()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}  // namespace BugEngine
