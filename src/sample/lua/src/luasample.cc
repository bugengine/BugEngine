/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/core/environment.hh>
#include <bugengine/filesystem/diskfolder.script.hh>
#include <luasample.hh>

namespace BugEngine {

LuaSample::LuaSample(const Plugin::Context& context)
    : Application(
       ref< DiskFolder >::create(Arena::game(), Environment::getEnvironment().getDataDirectory()),
       context.resourceManager, context.scheduler)
    , m_packageManager("plugin.scripting.package", pluginContext())
    , m_luaManager("plugin.scripting.lua", pluginContext())
    , m_mainPackage(ref< Package >::create(
         Arena::game(), pluginContext().dataFolder->openFile(istring("sample-lua.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

LuaSample::~LuaSample()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}  // namespace BugEngine
