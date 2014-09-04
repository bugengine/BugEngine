/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <console/stdafx.h>
#include    <window.script.hh>

#define raw raw2
#include    <curses.h>
#undef raw

namespace BugEngine
{

Window::Window(const istring name)
    :   name(name)
{
}

Window::~Window()
{
}

}
