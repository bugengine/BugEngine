/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/environment.hh>

#include    <winerror.h>

#define BE_PLUGIN_NAMESPACE_REGISTER(name)                                                                                   \
    namespace BugEngine                                                                                                                 \
    {                                                                                                                                   \
    weak<const BugEngine::RTTI::Namespace> be_Namespace()                                                                               \
    {                                                                                                                                   \
        static ref<const ::BugEngine::RTTI::Namespace> ns = ref<const ::BugEngine::RTTI::Namespace>::create(::BugEngine::rttiArena());  \
        return ns.operator->();                                                                                                         \
    }                                                                                                                                   \
    }                                                                                                                                   \
    extern "C" BE_EXPORT const BugEngine::RTTI::Namespace* be_pluginNamespace()                                                         \
    {                                                                                                                                   \
        return BugEngine::be_Namespace().operator->();                                                                                  \
    }
#define BE_PLUGIN_REGISTER(name, klass, params, args)                                                                                   \
    BE_PLUGIN_NAMESPACE_REGISTER(name);                                                                                                 \
    extern "C" BE_EXPORT klass* be_createPlugin params { void* m = ::BugEngine::gameArena().alloc<klass>(); return new(m) klass args; } \
    extern "C" BE_EXPORT void be_destroyPlugin(klass* cls) { minitl::checked_destroy(cls); ::BugEngine::gameArena().free(cls); }

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
    if (!h)
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
Plugin<Interface>::Plugin(const istring &pluginName, PreloadType /*preload*/)
:   m_handle(loadPlugin(pluginName))
,   m_interface(0)
{
}

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName)
:   m_handle(loadPlugin(pluginName))
,   m_interface(0)
{
    if (m_handle)
    {
        Interface* (*be_pluginCreate)(void) = reinterpret_cast<Interface* (*)(void)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_createPlugin"));
        if (!be_pluginCreate)
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
    if (m_handle)
    {
        Interface* (*be_pluginCreate)(T1) = reinterpret_cast<Interface* (*)(T1)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_createPlugin"));
        if (!be_pluginCreate)
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
    if (m_handle)
    {
        Interface* (*be_pluginCreate)(T1, T2) = reinterpret_cast<Interface* (*)(T1, T2)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_createPlugin"));
        if (!be_pluginCreate)
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
    if (m_handle)
    {
        if (m_interface)
        {
            void (*be_pluginDestroy)(Interface*) = reinterpret_cast<void (*)(Interface*)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_destroyPlugin"));
            if (be_pluginDestroy)
                (*be_pluginDestroy)(m_interface);
        }
        FreeLibrary(static_cast<HMODULE>(m_handle));
    }
}

template< typename Interface >
weak<const RTTI::Namespace> Plugin<Interface>::pluginNamespace() const
{
    if (m_handle)
    {
        const RTTI::Namespace* (*be_pluginNamespace)() = reinterpret_cast<const RTTI::Namespace* (*)()>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_pluginNamespace"));
        if (be_pluginNamespace)
            return (*be_pluginNamespace)(); 
    }
    return weak<const RTTI::Namespace>();
}


}

