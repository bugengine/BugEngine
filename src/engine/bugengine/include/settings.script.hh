/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */
#ifndef BE_BUGENGINE_SETTINGS_HH_
#define BE_BUGENGINE_SETTINGS_HH_
/**************************************************************************************************/
#include    <bugengine/stdafx.h>
#include    <settings/settings.factory.hh>


namespace BugEngine { namespace MainSettings
{

struct Log : public Settings::Settings<Log>
{
    bool enableConsoleLog;
    bool enableFileLog;

    Log()
        :   enableConsoleLog(true)
        ,   enableFileLog(true)
    {
    }
};

}}

/**************************************************************************************************/
#endif
