/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_WINDOW_HH_
#define BE_UI_CONSOLE_WINDOW_HH_
/**************************************************************************************************/
#include    <resource/description.script.hh>
#include    <cstdio>

#define raw raw2
#include    <curses.h>
#undef raw


namespace BugEngine
{

class be_api(CONSOLE) Window : public Resource::Description
{
published:
    const istring name;
private:
    struct ScreenPipe
    {
        friend class Window;
    private:
        FILE*   m_fileOutput;
        FILE*   m_screenStdOut;
    public:
        ScreenPipe();
        ~ScreenPipe();
    };
    ScreenPipe  m_pipe;
    SCREEN*     m_screen;
    WINDOW*     m_window;
published:
    Window(const istring name);
    ~Window();
};

}

/**************************************************************************************************/
#endif
