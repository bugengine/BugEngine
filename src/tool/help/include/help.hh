/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_HELP_HELP_HH_
#define BE_HELP_HELP_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/application.hh>
#include <bugengine/plugin.scripting.package/package.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/loader.hh>

namespace BugEngine {

class Help : public Application
{
private:
    Plugin::Plugin< Resource::ILoader > const m_packageManager;
    Plugin::Plugin< Resource::ILoader > const m_ui;
    ref< const Package > const                m_mainPackage;

public:
    Help(const Plugin::Context& context);
    ~Help();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
