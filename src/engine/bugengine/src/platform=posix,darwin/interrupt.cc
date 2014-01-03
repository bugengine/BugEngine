/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/application.hh>

#include    <signal.h>


namespace BugEngine
{

static Application* s_application = 0;

extern "C" void signalHandler(int /*signal*/)
{
    be_info("interrupted");
    if (s_application)
    {
        s_application->finish();
        s_application = 0;
    }
}

void Application::registerInterruptions()
{
    s_application = this;
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = &signalHandler;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGINT);
    action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action, 0);
}

void Application::unregisterInterruptions()
{
    s_application = 0;
}

}
