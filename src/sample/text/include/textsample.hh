/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_TEXT_KERNELSAMPLE_HH_
#define BE_SAMPLES_TEXT_KERNELSAMPLE_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/application.hh>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin.graphics.3d/renderer/irenderer.hh>
#include <bugengine/plugin.scripting.package/package.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/resourcemanager.hh>

namespace BugEngine {

class TextSample : public Application
{
private:
    Plugin::Plugin< Resource::ILoader > const m_packageManager;
    Plugin::Plugin< minitl::pointer > const   m_textManager;
    Plugin::Plugin< IRenderer > const         m_3ddx;
    Plugin::Plugin< IRenderer > const         m_3dgl;
    ref< const Package > const                m_mainPackage;
    Task::ITask::CallbackConnection           m_startRenderDx;
    Task::ITask::CallbackConnection           m_startNextUpdateDx;
    Task::ITask::CallbackConnection           m_startRenderGL;
    Task::ITask::CallbackConnection           m_startNextUpdateGL;

public:
    TextSample(const Plugin::Context& context);
    ~TextSample();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
