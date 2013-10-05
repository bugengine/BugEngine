/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/application.hh>

namespace BugEngine
{

static Application* s_application = 0;

extern "C" void signalHandler(int /*signal*/)
{
}

void Application::registerInterruptions()
{
}

void Application::unregisterInterruptions()
{
}

}
