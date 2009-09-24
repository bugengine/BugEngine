/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_PLUGIN_HH_
#define BE_SYSTEM_PLUGIN_HH_
/*****************************************************************************/

namespace BugEngine
{


class be_api(SYSTEM) Plugin
{
private:
    typedef void* Handle;
    #ifdef WIN32
    # ifdef _WIN64
        typedef __int64 __rval;
    # else
        typedef int     __rval;
    # endif
    #else
        typedef int     __rval;
    #endif
    typedef __rval (* generic)(void);
private:
    Handle FPluginHandle;
private:
    generic _get(const std::string& name);
public:
    Plugin(const istring &pluginName);
    virtual ~Plugin(void);

    template< typename callback >
    callback get(const std::string& name)
    {
        return reinterpret_cast<callback>(_get(name));
    }

    operator const void*() const { return FPluginHandle; }
private:
    Plugin();
    Plugin(const Plugin& other);
    Plugin& operator=(const Plugin& other);
};

}

/*****************************************************************************/
#endif
