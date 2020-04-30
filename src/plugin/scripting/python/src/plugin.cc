/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin/plugin.hh>

class PythonVersion : public minitl::refcountable
{
    BugEngine::Plugin::Plugin< minitl::refcountable > m_pyPlugin;

public:
    PythonVersion(const BugEngine::Plugin::Plugin< minitl::refcountable >& plugin)
        : m_pyPlugin(plugin)
    {
    }

    ~PythonVersion()
    {
    }
};

static ref< PythonVersion > create(const BugEngine::Plugin::Context& context)
{
    static const char* versions[]
       = {"38", "37", "36", "35", "34", "33", "32", "31", "30", "27", "26"};
    for(size_t i = 0; i < sizeof(versions) / sizeof(versions[0]); ++i)
    {
        minitl::format< 1024u > pluginName
           = minitl::format< 1024u >("plugin.scripting.python%s") | versions[i];
        BugEngine::Plugin::Plugin< minitl::refcountable > p
           = BugEngine::Plugin::Plugin< minitl::refcountable >(pluginName.c_str(), context);
        if(p)
        {
            be_info("Loaded Python version %c.%c" | versions[i][0] | versions[i][1]);
            return ref< PythonVersion >::create(BugEngine::Arena::general(), p);
        }
    }
    return ref< PythonVersion >();
}

BE_PLUGIN_REGISTER_CREATE(&create);
