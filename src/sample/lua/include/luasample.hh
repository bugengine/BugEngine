/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLE_LUASAMPLE_HH_
#define BE_SAMPLE_LUASAMPLE_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/application.hh>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin.scripting.package/package.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/resourcemanager.hh>

namespace BugEngine {

class LuaSample : public Application
{
private:
    Plugin::Plugin< Resource::ILoader > const m_packageManager;
    Plugin::Plugin< Resource::ILoader > const m_luaManager;
    ref< const Package > const                m_mainPackage;

public:
    LuaSample(const Plugin::Context& context);
    ~LuaSample();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
