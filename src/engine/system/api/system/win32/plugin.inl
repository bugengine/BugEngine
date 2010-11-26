/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdexcept>
#include    <core/environment.hh>

#include    <winerror.h>

#define BE_PLUGIN_REGISTER(name, klass, params, args)                               \
    extern "C" FORCEEXPORT klass* be_createPlugin params { void* m = BugEngine::Memory<klass::Arena>::allocArray<klass>(1); return new(m) klass args; } \
    extern "C" FORCEEXPORT void be_destroyPlugin(klass* cls) { minitl::checked_destroy(cls); BugEngine::Memory<klass::Arena>::free(cls); }

namespace BugEngine
{

static HANDLE loadPlugin(const istring &pluginName)
{
    SetLastError(0);
    minitl::format<> plugingFile = minitl::format<>("%s.dll") | pluginName;
    const ipath& pluginDir = Environment::getEnvironment().getDataDirectory();
    static const ipath pluginSubdir = ipath("plugins");
    minitl::format<ifilename::MaxFilenameLength> pluginPath = (pluginDir + pluginSubdir + ifilename(plugingFile.c_str())).str();
    HANDLE h = LoadLibrary(pluginPath.c_str());
    if(!h)
    {
        char *errorMessage = 0;
        int errorCode = ::GetLastError();
        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&errorMessage),
            0,
            NULL);
        errorCode = ::GetLastError();
        be_error("%s : %s"|pluginName|errorMessage);
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

