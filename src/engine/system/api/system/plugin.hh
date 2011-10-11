/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_PLUGIN_HH_
#define BE_SYSTEM_PLUGIN_HH_
/*****************************************************************************/

namespace BugEngine
{

    namespace RTTI
    {
        struct ClassInfo;
    }

template< typename Interface >
class be_api(SYSTEM) Plugin
{
private:
    typedef void*   Handle;
private:
    Handle          m_handle;
    Interface*      m_interface;
    i_u32*          m_refCount;
public:
    enum PreloadType { Preload };
    Plugin(const istring &pluginName, PreloadType preload);
    Plugin(const istring &pluginName);
    template< typename T1 >
    Plugin(const istring &pluginName, T1 param1);
    template< typename T1, typename T2 >
    Plugin(const istring &pluginName, T1 param1, T2 param2);
    ~Plugin(void);
    Plugin(const Plugin& other);
    Plugin& operator=(const Plugin& other);

    Interface* operator->()             { return m_interface; }
    const Interface* operator->() const { return m_interface; }
    operator const void*() const        { return m_handle; }
    bool operator!() const              { return m_handle == 0; }

    const RTTI::ClassInfo* pluginNamespace() const;
private:
    Plugin();
};

}

#ifdef BE_STATIC
# include "console/plugin.inl"
#elif defined(BE_PLATFORM_WIN32)
# include "win32/plugin.inl"
#elif defined(BE_PLATFORM_POSIX) || defined(BE_PLATFORM_MACOS)
# include "posix/plugin.inl"
#else
# error Plugin subsystem not implemented...
#endif

/*****************************************************************************/
#endif
