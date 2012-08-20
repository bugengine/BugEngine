/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/environment.hh>
#include    <rtti/classinfo.script.hh>

#include    <windows.h>
#include    <winerror.h>

#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name)                                                \
    BE_PLUGIN_NAMESPACE_CREATE_(name)                                                           \
    extern "C" BE_EXPORT const BugEngine::RTTI::Class* be_pluginNamespace()                     \
    {                                                                                           \
        return BugEngine::be_##name##_Namespace().operator->();                                 \
    }
#define BE_PLUGIN_NAMESPACE_REGISTER_(name)                                                     \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name)
#define BE_PLUGIN_NAMESPACE_REGISTER()                                                          \
    BE_PLUGIN_NAMESPACE_REGISTER_(BE_PROJECTSHORTNAME)

#define BE_PLUGIN_REGISTER_NAMED(name, interface, klass)                                        \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name);                                                   \
    extern "C" BE_EXPORT interface* be_createPlugin (const ::BugEngine::PluginContext& context) \
    {                                                                                           \
        void* m = ::BugEngine::Arena::general().alloc<klass>();                                 \
        return new(m) klass(context);                                                           \
    }                                                                                           \
    extern "C" BE_EXPORT void be_destroyPlugin(klass* cls)                                      \
    {                                                                                           \
        minitl::checked_destroy(cls);                                                           \
        ::BugEngine::Arena::general().free(cls);                                                \
    }
#define BE_PLUGIN_REGISTER_NAMED_(name, interface, klass)                                       \
    BE_PLUGIN_REGISTER_NAMED(name, interface, klass)
#define BE_PLUGIN_REGISTER(interface, klass)                                                    \
    BE_PLUGIN_REGISTER_NAMED_(BE_PROJECTSHORTNAME, interface, klass)

namespace BugEngine
{

static HMODULE loadPlugin(const inamespace &pluginName)
{
    SetLastError(0);
    minitl::format<1024u> plugingFile = minitl::format<1024u>("%s.dll") | pluginName;
    const ipath& pluginDir = Environment::getEnvironment().getDataDirectory();
    static const ipath pluginSubdir = ipath("plugins");
    ifilename::Filename pluginPath = (pluginDir + pluginSubdir + ifilename(plugingFile.c_str())).str();
    be_info("loading plugin %s (%s)" | pluginName | pluginPath.name);

    int length = be_checked_numcast<int>(strlen(pluginPath.name));
    int count = MultiByteToWideChar(0, 0, pluginPath.name, length, 0, 0);
    minitl::Allocator::Block<wchar_t> wPath(Arena::stack(), count);
    count = MultiByteToWideChar(0, 0, pluginPath.name, length, wPath, count);

    HMODULE h = ::LoadPackagedLibrary(wPath, 0);
    if (!h)
    {
        char errorMessage[1024];
        int errorCode = ::GetLastError();
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            errorMessage,
            sizeof(errorMessage),
            NULL);
        be_error("%s : %s"|pluginName|errorMessage);
    }
    return h;
}

template< typename Interface >
Plugin<Interface>::Plugin(const inamespace &pluginName, PreloadType /*preload*/)
:   m_handle(loadPlugin(pluginName))
,   m_interface(0)
,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
}

template< typename Interface >
Plugin<Interface>::Plugin(const inamespace &pluginName, const PluginContext& context)
:   m_handle(loadPlugin(pluginName))
,   m_interface(0)
,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
    if (m_handle)
    {
        Interface* (*be_pluginCreate)(const PluginContext&) = reinterpret_cast<Interface* (*)(const PluginContext&)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_createPlugin"));
        if (!be_pluginCreate)
        {
            char errorMessage[1024];
            ::FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                ::GetLastError(),
                0,
                errorMessage,
                sizeof(errorMessage),
                NULL);
            be_error(errorMessage);
        }
        else
        {
            m_interface = (*be_pluginCreate)(context);
        }
    }
}

template< typename Interface >
Plugin<Interface>::~Plugin(void)
{
    if (! --*m_refCount)
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
        minitl::checked_destroy(m_refCount);
        Arena::general().free(m_refCount);
    }
}

template< typename Interface >
Plugin<Interface>::Plugin(const Plugin<Interface>& other)
    :   m_handle(other.m_handle)
    ,   m_interface(other.m_interface)
    ,   m_refCount(other.m_refCount)
{
    (*m_refCount)++;
}

template< typename Interface >
Plugin<Interface>& Plugin<Interface>::operator =(const Plugin<Interface>& other)
{
    *(other->m_refCount)++;
    if (! --*m_refCount)
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
        minitl::checked_destroy(m_refCount);
        Arena::general().free(m_refCount);
    }
    m_refCount = other.m_refCount;
    m_handle = other.m_handle;
    m_interface = other.m_interface;
    return *this;
}

template< typename Interface >
raw<const RTTI::Class> Plugin<Interface>::pluginNamespace() const
{
    if (m_handle)
    {
        const RTTI::Class* (*be_pluginNamespace)() = reinterpret_cast<const RTTI::Class* (*)()>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_pluginNamespace"));
        if (be_pluginNamespace)
        {
            raw<const RTTI::Class> ci = {(*be_pluginNamespace)()};
            return ci;
        }
    }
    raw<const RTTI::Class> ci = {0};
    return ci;
}


}

