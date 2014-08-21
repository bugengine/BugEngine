/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <console/stdafx.h>
#include    <console/uiconsole.hh>

#define raw raw2
#include    <curses.h>
#undef raw

namespace BugEngine
{

struct NCursesInitialisation
{
    NCursesInitialisation()
    {
        initscr();
        cbreak();
        noecho();
        nonl();
        intrflush(stdscr, FALSE);
        keypad(stdscr, TRUE);
    }
    ~NCursesInitialisation()
    {
        endwin();
    }
};

BE_EXPORT NCursesInitialisation s_initialiseCurses;

UIConsole::UIConsole(const Plugin::Context& /*context*/)
    :   m_uiWidgetManager()
{
}

UIConsole::~UIConsole()
{
}

}
