/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_PLUGIN_HH_
#define BE_SYSTEM_PLUGIN_HH_
/*****************************************************************************/

namespace BugEngine
{

template< typename Interface >
class be_api(SYSTEM) Plugin
{
private:
    typedef void* Handle;
private:
    Handle      m_handle;
    Interface*  m_interface;
public:
    Plugin(const istring &pluginName);
    virtual ~Plugin(void);

    Interface* operator->()             { return m_interface; }
    const Interface* operator->() const { return m_interface; }
    operator const void*() const        { return m_interface; }
    bool operator!() const              { return m_interface == 0; }
private:
    Plugin();
    Plugin(const Plugin& other);
    Plugin& operator=(const Plugin& other);
};

}

#if defined(BE_PLATFORM_WIN32)
# include "win32/plugin.inl"
#elif defined(BE_PLATFORM_POSIX)
# include "posix/plugin.inl"
#elif defined(BE_PLATFORM_CONSOLE)
# include "console/plugin.inl"
#endif

/*****************************************************************************/
#endif
