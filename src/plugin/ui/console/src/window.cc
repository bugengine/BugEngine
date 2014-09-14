/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <console/stdafx.h>
#include    <window.script.hh>


namespace BugEngine
{

Window::CursesWindow::CursesWindow()
    :   m_pipe()
    ,   m_screen(newterm(NULL, m_pipe.m_screenStdOut, stdin))
{
    set_term(m_screen);
    cbreak();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    noecho();
}

Window::CursesWindow::~CursesWindow()
{
    set_term(m_screen);
    endwin();
    delscreen(m_screen);
}

Window::Window(const istring name)
    :   name(name)
{
}

Window::~Window()
{
}

ref<Window::CursesWindow> Window::create() const
{
    return ref<CursesWindow>::create(Arena::resource());
}

}
