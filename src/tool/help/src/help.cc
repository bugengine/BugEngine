/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <help.hh>
#include    <filesystem/diskfolder.script.hh>


namespace BugEngine
{

Help::Help(const Plugin::Context& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
    ,   m_packageManager("plugin.scripting.package", pluginContext())
    ,   m_ui("plugin.ui.console", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(istring("help.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

Help::~Help()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}
