/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_UIWIDGET_HH_
#define BE_UI_CONSOLE_UIWIDGET_HH_
/**************************************************************************************************/
#include    <resource/description.script.hh>

namespace BugEngine
{

class be_api(CONSOLE) UIWidget : public Resource::Description
{
public:
    UIWidget();
    ~UIWidget();
};

}

/**************************************************************************************************/
#endif
