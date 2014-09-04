/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <console/stdafx.h>
#include    <window.script.hh>


namespace BugEngine
{

Window::Window(const istring name)
    :   name(name)
    ,   m_pipe()
    ,   m_screen(newterm(NULL, m_pipe.m_screenStdOut, stdin))
    ,   m_window(0)
{
    set_term(m_screen);
    m_window = initscr();
    cbreak();
    nonl();
    intrflush(m_window, FALSE);
    keypad(m_window, TRUE);
    noecho();
}

Window::~Window()
{
    delscreen(m_screen);
}

}
