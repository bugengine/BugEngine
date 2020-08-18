/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/stdafx.h>
#include <bugengine/application.hh>

#include <bugengine/core/threads/mutex.hh>

#include <cstdio>
#include <windows.h>

namespace BugEngine {

static Application* s_application;

static BOOL WINAPI HandleControlEvent(DWORD eventType)
{
    switch(eventType)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
        be_info("interrupt event");
        if(s_application) s_application->finish();
        return TRUE;
    default: return FALSE;
    }
}

void Application::registerInterruptions()
{
    s_application = this;
    if(GetConsoleWindow())
    {
        SetConsoleCtrlHandler(&HandleControlEvent, TRUE);
    }
}

void Application::unregisterInterruptions()
{
    if(GetConsoleWindow())
    {
        SetConsoleCtrlHandler(&HandleControlEvent, FALSE);
    }
    s_application = 0;
}

}  // namespace BugEngine
