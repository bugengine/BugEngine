/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <console/stdafx.h>
#include    <console/uiconsole.hh>
#include    <uiwidgetloader.hh>
#include    <window.script.hh>
#include    <resource/resourcemanager.hh>


namespace BugEngine
{

UIConsole::UIConsole(const Plugin::Context& context)
    :   m_resourceManager(context.resourceManager)
    ,   m_uiWidgetLoader(ref<UIWidgetLoader>::create(Arena::general()))
{
    m_resourceManager->attach<Window>(m_uiWidgetLoader);
}

UIConsole::~UIConsole()
{
    m_resourceManager->detach<Window>(m_uiWidgetLoader);
}

}
