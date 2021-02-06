/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */
#ifndef BE_BUGENGINE_SETTINGS_HH_
#define BE_BUGENGINE_SETTINGS_HH_
/**************************************************************************************************/
#include <bugengine/stdafx.h>
#include <bugengine/settings/settings.factory.hh>

namespace BugEngine { namespace MainSettings {

struct Log : public Settings::Settings< Log >
{
    bool enableConsoleLog;
    bool enableFileLog;

    Log() : enableConsoleLog(true), enableFileLog(true)
    {
    }
};

}}  // namespace BugEngine::MainSettings

/**************************************************************************************************/
#endif
