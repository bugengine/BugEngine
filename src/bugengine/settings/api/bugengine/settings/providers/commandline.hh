/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SETTINGS_PROVIDERS_COMMANDLINE_HH_
#define BE_SETTINGS_PROVIDERS_COMMANDLINE_HH_
/**************************************************************************************************/
#include <bugengine/settings/stdafx.h>
#include <bugengine/settings/settingsprovider.hh>

namespace BugEngine { namespace Settings {

class be_api(SETTINGS) CommandLineSettingsProvider : public SettingsProvider
{
private:
    minitl::hashmap< istring, SettingsProvider::SettingsList > buildSettings(int         argc,
                                                                             const char* argv[]);

public:
    CommandLineSettingsProvider(int argc, const char* argv[], ref< Folder > folder);
    ~CommandLineSettingsProvider();
};

}}  // namespace BugEngine::Settings

/**************************************************************************************************/
#endif
