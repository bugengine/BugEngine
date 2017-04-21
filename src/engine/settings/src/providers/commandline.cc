/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <settings/stdafx.h>
#include    <settings/providers/commandline.hh>
#include    <rttiparse/valueparse.hh>


namespace BugEngine { namespace Settings
{

minitl::hashmap<istring, SettingsProvider::SettingsList >
CommandLineSettingsProvider::buildSettings(int argc, const char* argv[])
{
    minitl::hashmap<istring, SettingsProvider::SettingsList> result(Arena::temporary());
    static const char* s_true = "true";
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            const char* nameBegin = argv[i] + 1;
            const char* sep = nameBegin;
            const char* nameEnd = nameBegin;
            const char* optionBegin = 0;
            while(*nameEnd && *nameEnd != '=')
            {
                if (*nameEnd == '.')
                    sep = nameEnd;
                ++nameEnd;
            }
            if (*nameEnd == '=')
            {
                optionBegin = nameEnd + 1;
            }
            else if (i+1 < argc)
            {
                if (*argv[i+1] == '-')
                {
                    optionBegin = s_true;
                }
                else
                {
                    ++i;
                    optionBegin = argv[i];
                }
            }
            else
            {
                optionBegin = s_true;
            }
            const char* optionEnd = optionBegin;
            while(*optionEnd) ++optionEnd;
            if (sep == nameBegin)
            {
                be_error("invalid command line option: %s" | nameBegin);
                continue;
            }
            else
            {
                istring category(nameBegin, sep);
                istring property(sep+1, nameEnd);
                ref<RTTI::Parser::Node> value;
                addSetting(result, category, property, value);
            }
        }
    }
    return result;
}

CommandLineSettingsProvider::CommandLineSettingsProvider(int argc, const char* argv[])
    :   SettingsProvider(buildSettings(argc, argv))
{

}

CommandLineSettingsProvider::~CommandLineSettingsProvider()
{
}

}}
