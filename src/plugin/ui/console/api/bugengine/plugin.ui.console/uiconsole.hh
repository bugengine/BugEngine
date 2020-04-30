/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_UICONSOLE_HH_
#define BE_UI_CONSOLE_UICONSOLE_HH_
/**************************************************************************************************/
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/loader.hh>

namespace BugEngine {

class be_api(CONSOLE) UIConsole : public minitl::refcountable
{
private:
    weak< Resource::ResourceManager > const m_resourceManager;
    ref< Resource::ILoader > const          m_uiWidgetLoader;

public:
    UIConsole(const Plugin::Context& context);
    ~UIConsole();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
