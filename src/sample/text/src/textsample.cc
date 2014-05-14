
/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <textsample.hh>
#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine
{

TextSample::TextSample(const Plugin::Context& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
    ,   m_packageManager("plugin.scripting.package", pluginContext())
    ,   m_pythonManager("plugin.scripting.python27", pluginContext())
    ,   m_pythonManager2("plugin.scripting.python34", pluginContext())
    ,   m_textManager("plugin.graphics.text", pluginContext())
    ,   m_3ddx("plugin.graphics.Dx9", pluginContext())
    ,   m_3dgl("plugin.graphics.GL4", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(istring("sample-text.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

TextSample::~TextSample()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}
