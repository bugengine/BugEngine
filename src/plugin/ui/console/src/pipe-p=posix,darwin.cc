/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <console/stdafx.h>
#include    <window.script.hh>
#include    <unistd.h>
#include    <errno.h>
#include    <stdio.h>

namespace BugEngine
{

Window::CursesWindow::ScreenPipe::ScreenPipe()
{
    int pipeDescriptors[2];
    int result = pipe(pipeDescriptors);
    be_assert(result == 0, "CreatePipe failed with error %s" | strerror(errno));
    be_forceuse(result);
    m_screenStdOut = fdopen(pipeDescriptors[0], "rt");
    m_fileOutput = fdopen(pipeDescriptors[1], "wt");
}

Window::CursesWindow::ScreenPipe::~ScreenPipe()
{
    fclose(m_fileOutput);
    fclose(m_screenStdOut);
}

}
