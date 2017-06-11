/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLE_LUASAMPLE_HH_
#define BE_SAMPLE_LUASAMPLE_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <filesystem/folder.script.hh>
#include    <bugengine/application.hh>
#include    <resource/resourcemanager.hh>
#include    <plugin/plugin.hh>
#include    <package/package.script.hh>

namespace BugEngine
{

class LuaSample : public Application
{
private:
    Plugin::Plugin<Resource::ILoader>   const   m_packageManager;
    Plugin::Plugin<Resource::ILoader>   const   m_luaManager;
    ref<const Package>                  const   m_mainPackage;
public:
    LuaSample(const Plugin::Context& context);
    ~LuaSample();
};

}

/**************************************************************************************************/
#endif
