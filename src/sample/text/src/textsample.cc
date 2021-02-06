/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/core/environment.hh>
#include <bugengine/filesystem/diskfolder.script.hh>
#include <textsample.hh>

namespace BugEngine {

TextSample::TextSample(const Plugin::Context& context)
    : Application(
        ref< DiskFolder >::create(Arena::game(), Environment::getEnvironment().getDataDirectory()),
        context.resourceManager, context.scheduler)
    , m_packageManager("plugin.scripting.package", pluginContext())
    , m_textManager("plugin.graphics.text", pluginContext())
    , m_3ddx("plugin.graphics.Dx9", pluginContext())
    , m_3dgl("plugin.graphics.GL4", pluginContext())
    , m_mainPackage(ref< Package >::create(
          Arena::game(), pluginContext().dataFolder->openFile(istring("sample-text.pkg"))))
{
    if(m_3ddx)
    {
        m_startRenderDx = Task::ITask::CallbackConnection(worlUpdateTask(),
                                                          m_3ddx->syncTask()->startCallback());
        m_startNextUpdateDx
            = Task::ITask::CallbackConnection(m_3ddx->syncTask(), worlUpdateTask()->startCallback(),
                                              Task::ITask::ICallback::Completed);
    }
    if(m_3dgl)
    {
        m_startRenderGL = Task::ITask::CallbackConnection(worlUpdateTask(),
                                                          m_3dgl->syncTask()->startCallback());
        m_startNextUpdateGL
            = Task::ITask::CallbackConnection(m_3dgl->syncTask(), worlUpdateTask()->startCallback(),
                                              Task::ITask::ICallback::Completed);
    }
    pluginContext().resourceManager->load(m_mainPackage);
}

TextSample::~TextSample()
{
    pluginContext().resourceManager->unload(m_mainPackage);
}

}  // namespace BugEngine
