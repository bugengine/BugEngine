/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <plugin/plugin.hh>

BugEngine::Plugin::Plugin<void> s_pyPlugin;
static minitl::refcountable* create(const BugEngine::Plugin::Context &context)
{
    static const char* versions[] = {
        "35",
        "34",
        "33",
        "32",
        "31",
        "30",
        "27",
        "26"
    };
    for (size_t i = 0; i < sizeof(versions)/sizeof(versions[0]); ++i)
    {
        minitl::format<1024u> pluginName = minitl::format<1024u>("plugin.scripting.python%s")
                | versions[i];
        s_pyPlugin = BugEngine::Plugin::Plugin<void>(pluginName.c_str(), context);
        if (s_pyPlugin)
        {
            be_info("Loaded Python version %c.%c" | versions[i][0] | versions[i][1]);
            return static_cast<minitl::refcountable*>(s_pyPlugin.operator->());
        }
    }
    return 0;
}

BE_PLUGIN_REGISTER_CREATE_RAW(&create);
