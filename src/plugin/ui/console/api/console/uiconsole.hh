/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_UICONSOLE_HH_
#define BE_UI_CONSOLE_UICONSOLE_HH_
/**************************************************************************************************/
#include    <resource/loader.hh>
#include    <plugin/plugin.hh>

namespace BugEngine
{

class be_api(CONSOLE) UIConsole : public minitl::refcountable
{
private:
    ref<Resource::ILoader> const    m_uiWidgetManager;
public:
    UIConsole(const Plugin::Context& context);
    ~UIConsole();
};

}

/**************************************************************************************************/
#endif
