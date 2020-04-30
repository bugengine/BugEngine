/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.ui.console/stdafx.h>
#include <bugengine/plugin.ui.console/uiconsole.hh>
#include <bugengine/resource/resourcemanager.hh>
#include <uiwidgetloader.hh>
#include <window.script.hh>

namespace BugEngine {

UIConsole::UIConsole(const Plugin::Context& context)
    : m_resourceManager(context.resourceManager)
    , m_uiWidgetLoader(ref< UIWidgetLoader >::create(Arena::general()))
{
    m_resourceManager->attach< Window >(m_uiWidgetLoader);
}

UIConsole::~UIConsole()
{
    m_resourceManager->detach< Window >(m_uiWidgetLoader);
}

}  // namespace BugEngine
