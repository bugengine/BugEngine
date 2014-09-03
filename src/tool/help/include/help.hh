/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_HELP_HELP_HH_
#define BE_HELP_HELP_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <bugengine/application.hh>
#include    <plugin/plugin.hh>
#include    <resource/loader.hh>
#include    <package/package.script.hh>


namespace BugEngine
{

class Help : public Application
{
private:
    Plugin::Plugin<Resource::ILoader>   const   m_packageManager;
    ref<const Package>                  const   m_mainPackage;

public:
    Help(const Plugin::Context& context);
    ~Help();
};

}

/**************************************************************************************************/
#endif
