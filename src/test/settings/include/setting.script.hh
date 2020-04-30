/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */
#ifndef BE_TEST_SETTINGS_SETTINGS_HH_
#define BE_TEST_SETTINGS_SETTINGS_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/settings/settings.factory.hh>

namespace BugEngine { namespace TestSettings {

enum EnumSetting
{
    EnumSettingValue1,
    EnumSettingValue2
};

struct TestSettings : public Settings::Settings< TestSettings >
{
    EnumSetting enumSetting;

    TestSettings() : enumSetting(EnumSettingValue1)
    {
    }
};

}}  // namespace BugEngine::TestSettings

/**************************************************************************************************/
#endif
