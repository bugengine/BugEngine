/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdexcept>
#include    <core/environment.hh>


#define BE_PLUGIN_REGISTER(name, klass, params, args)                               \
    extern "C" FORCEEXPORT klass* be_createPlugin params { return new klass args; } \
    extern "C" FORCEEXPORT void be_destroyPlugin(klass* cls) { delete cls; }

namespace BugEngine
{

static HANDLE loadPlugin(const istring &pluginName)
{
    SetLastError(0);
    std::string pluginDir = Environment::getEnvironment().getDataDirectory().str();
    HANDLE h = LoadLibrary( (pluginDir + "/plugins/" + pluginName.c_str() + ".dll").c_str());
    if(!h)
    {
        char *errorMessage;
        ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ::GetLastError(),
            0,
            reinterpret_cast<LPTSTR>(&errorMessage),
            0,
            NULL);
        be_error(errorMessage);
        ::LocalFree(errorMessage);
    }
    return h;
}

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName)
:   m_handle(loadPlugin(pluginName))
,   m_interface(0)
{
    if(m_handle)
    {
        Interface* (*be_pluginCreate)(void) = reinterpret_cast<Interface* (*)(void)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_createPlugin"));
        if(!be_pluginCreate)
        {
            char *errorMessage;
            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                ::GetLastError(),
                0,
                reinterpret_cast<LPTSTR>(&errorMessage),
                0,
                NULL);
            be_error(errorMessage);
            ::LocalFree(errorMessage);
        }
        else
        {
            m_interface = (*be_pluginCreate)();
        }
    }
}

template< typename Interface >
template< typename T1 >
Plugin<Interface>::Plugin(const istring &pluginName, T1 param1)
:   m_handle(loadPlugin(pluginName))
,   m_interface(0)
{
    if(m_handle)
    {
        Interface* (*be_pluginCreate)(T1) = reinterpret_cast<Interface* (*)(T1)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_createPlugin"));
        if(!be_pluginCreate)
        {
            char *errorMessage;
            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                ::GetLastError(),
                0,
                reinterpret_cast<LPTSTR>(&errorMessage),
                0,
                NULL);
            be_error(errorMessage);
            ::LocalFree(errorMessage);
        }
        else
        {
            m_interface = (*be_pluginCreate)(param1);
        }
    }
}

template< typename Interface >
template< typename T1, typename T2 >
Plugin<Interface>::Plugin(const istring &pluginName, T1 param1, T2 param2)
:   m_handle(loadPlugin(pluginName))
,   m_interface(0)
{
    if(m_handle)
    {
        Interface* (*be_pluginCreate)(T1, T2) = reinterpret_cast<Interface* (*)(T1, T2)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_createPlugin"));
        if(!be_pluginCreate)
        {
            char *errorMessage;
            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                ::GetLastError(),
                0,
                reinterpret_cast<LPTSTR>(&errorMessage),
                0,
                NULL);
            be_error(errorMessage);
            ::LocalFree(errorMessage);
        }
        else
        {
            m_interface = (*be_pluginCreate)(param1, param2);
        }
    }
}

template< typename Interface >
Plugin<Interface>::~Plugin(void)
{
    if(m_handle)
    {
        void (*be_pluginDestroy)(Interface*) = reinterpret_cast<void (*)(Interface*)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_destroyPlugin"));
        if(be_pluginDestroy)
            (*be_pluginDestroy)(m_interface); 
        FreeLibrary(static_cast<HMODULE>(m_handle));
    }
}

}

