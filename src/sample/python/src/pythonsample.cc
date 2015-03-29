/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <pythonsample.hh>
#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>

namespace BugEngine
{

PythonSample::PythonSample(const Plugin::Context& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
    ,   m_packageManager("plugin.scripting.package", pluginContext())
    ,   m_pythonManager("plugin.scripting.python", pluginContext())
    ,   m_mainPackage(ref<Package>::create(Arena::game(), pluginContext().dataFolder->openFile(istring("sample-python.pkg"))))
{
    pluginContext().resourceManager->load(m_mainPackage);
}

PythonSample::~PythonSample()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}