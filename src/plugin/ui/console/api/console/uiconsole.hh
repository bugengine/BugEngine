/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_UICONSOLE_HH_
#define BE_UI_CONSOLE_UICONSOLE_HH_
/**************************************************************************************************/
#include    <console/stdafx.h>
#include    <resource/loader.hh>

namespace BugEngine
{

class UIConsole : public minitl::refcountable
{
private:
    Plugin::Plugin<Resource::ILoader>   const   m_uiWidgetManager;
public:
    UIConsole(const Plugin::Context& context);
    ~UIConsolee();
};

}

/**************************************************************************************************/
#endif
