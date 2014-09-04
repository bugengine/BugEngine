/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_WINDOW_HH_
#define BE_UI_CONSOLE_WINDOW_HH_
/**************************************************************************************************/
#include    <resource/description.script.hh>

namespace BugEngine
{

class be_api(CONSOLE) Window : public Resource::Description
{
published:
    const istring name;
published:
    Window(const istring name);
    ~Window();
};

}

/**************************************************************************************************/
#endif
