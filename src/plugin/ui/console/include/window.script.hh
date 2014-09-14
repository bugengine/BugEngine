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
public:
    class CursesWindow : public minitl::refcountable
    {
    private:
        struct ScreenPipe
        {
            friend class CursesWindow;
        private:
            FILE*   m_fileOutput;
            FILE*   m_screenStdOut;
        public:
            ScreenPipe();
            ~ScreenPipe();
        private:
            ScreenPipe(const ScreenPipe& other);
            ScreenPipe& operator=(const ScreenPipe& oher);
        };
        ScreenPipe  m_pipe;
        SCREEN*     m_screen;
    public:
        CursesWindow();
        ~CursesWindow();
    };
published:
    Window(const istring name);
    ~Window();

public:
    ref<CursesWindow> create() const;
};

}

/**************************************************************************************************/
#endif
