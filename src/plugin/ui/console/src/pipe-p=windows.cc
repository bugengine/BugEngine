/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <console/stdafx.h>
#include    <window.script.hh>
#include    <io.h>
#include    <fcntl.h>
#include    <stdio.h>

namespace BugEngine
{

Window::CursesWindow::ScreenPipe::ScreenPipe()
{
    HANDLE pipeRead, pipeWrite;
    BOOL result = CreatePipe(&pipeRead, &pipeWrite, NULL, 0);
    be_assert(result != FALSE, "CreatePipe failed with error %d" | GetLastError());
    int descRead = _open_osfhandle((intptr_t)pipeRead, _O_RDONLY);
    m_screenStdOut = _fdopen(descRead, "r");
    int descWrite = _open_osfhandle((intptr_t)pipeWrite, _O_WRONLY);
    m_fileOutput = _fdopen(descWrite, "w");

    SetStdHandle(STD_OUTPUT_HANDLE, m_screenStdOut);
}

Window::CursesWindow::ScreenPipe::~ScreenPipe()
{
    fclose(m_fileOutput);
    fclose(m_screenStdOut);
}

}
