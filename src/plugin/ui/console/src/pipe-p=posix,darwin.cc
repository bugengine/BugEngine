/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.ui.console/stdafx.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <window.script.hh>

namespace BugEngine {

Window::CursesWindow::ScreenPipe::ScreenPipe()
{
    int pipeDescriptors[2];
    int result = pipe(pipeDescriptors);
    be_assert(result == 0, "CreatePipe failed with error %s" | strerror(errno));
    be_forceuse(result);
    m_screenStdOut = fdopen(pipeDescriptors[0], "rt");
    m_fileOutput   = fdopen(pipeDescriptors[1], "wt");
}

Window::CursesWindow::ScreenPipe::~ScreenPipe()
{
    fclose(m_fileOutput);
    fclose(m_screenStdOut);
}

}  // namespace BugEngine
