/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <core/environment.hh>
#include    <rtti/classinfo.script.hh>

#include    <dlfcn.h>


#define BE_PLUGIN_REGISTER(name, klass, params, args)                                                                                   \
    BE_PLUGIN_NAMESPACE_REGISTER(name);                                                                                                 \
    extern "C" BE_EXPORT const BugEngine::RTTI::ClassInfo* be_pluginNamespace()                                                         \
    {                                                                                                                                   \
        return BugEngine::be_##name##_Namespace();                                                                                      \
    }                                                                                                                                   \
    extern "C" BE_EXPORT klass* be_createPlugin params { void* m = ::BugEngine::gameArena().alloc<klass>(); return new(m) klass args; } \
    extern "C" BE_EXPORT void be_destroyPlugin(klass* cls) { minitl::checked_destroy(cls); ::BugEngine::gameArena().free(cls); }

namespace BugEngine
{

static void* loadLibrary(const istring& pluginName)
{
    minitl::format<> plugingFile = minitl::format<>("lib%s.so") | pluginName;
    const ipath& pluginDir = Environment::getEnvironment().getDataDirectory();
    static const ipath pluginSubdir = ipath("plugins");
    minitl::format<ifilename::MaxFilenameLength> pluginPath = (pluginDir + pluginSubdir + ifilename(plugingFile.c_str())).str();
    be_info("loading plugin %s (%s)" | pluginName | pluginPath);
    void* handle = dlopen(pluginPath.c_str(), RTLD_NOW|RTLD_LOCAL);
    if (!handle)
    {
        be_error(dlerror());
    }
    return handle;
}

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName, PreloadType preload)
:   m_handle(loadLibrary(pluginName))
,   m_interface(0)
,   m_refCount(new (gameArena()) i_u32(1))
{
}

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName)
:   m_handle(loadLibrary(pluginName))
,   m_interface(0)
,   m_refCount(new (gameArena()) i_u32(1))
{
    if  (m_handle)
    {
        Interface* (*_init)(void) = reinterpret_cast<Interface* (*)(void)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_createPlugin")));
        be_assert(_init, "could not find method _init in plugin %s" | pluginName.c_str());
        m_interface = (*_init)();
    }
}

template< typename Interface >
template< typename T1 >
Plugin<Interface>::Plugin(const istring &pluginName, T1 param1)
:   m_handle(loadLibrary(pluginName))
,   m_interface(0)
,   m_refCount(new (gameArena()) i_u32(1))
{
    if (m_handle)
    {
        Interface* (*_init)(T1) = reinterpret_cast<Interface* (*)(T1)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_createPlugin")));
        be_assert(_init, "could not find method _init in plugin %s" | pluginName.c_str());
        m_interface = (*_init)(param1);
    }
}

template< typename Interface >
template< typename T1, typename T2 >
Plugin<Interface>::Plugin(const istring &pluginName, T1 param1, T2 param2)
:   m_handle(loadLibrary(pluginName))
,   m_interface(0)
,   m_refCount(new (gameArena()) i_u32(1))
{
    if (m_handle)
    {
        Interface* (*_init)(T1, T2) = reinterpret_cast<Interface* (*)(T1, T2)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_createPlugin")));
        be_assert(_init, "could not find method _init in plugin %s" | pluginName.c_str());
        m_interface = (*_init)(param1, param2);
    }
}
template< typename Interface >
Plugin<Interface>::~Plugin(void)
{
    if (!--*m_refCount)
    {
        if (m_handle && m_interface)
        {
            void (*_fini)(Interface*) = reinterpret_cast<void (*)(Interface*)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_destroyPlugin")));
            if (_fini)
                _fini(m_interface);
            //dlclose(m_handle); crashes on systems with TLS
        }
        minitl::checked_destroy(m_refCount);
        gameArena().free(m_refCount);
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
        if (m_handle && m_interface)
        {
            void (*_fini)(Interface*) = reinterpret_cast<void (*)(Interface*)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_destroyPlugin")));
            if (_fini)
                _fini(m_interface);
            //dlclose(m_handle); crashes on systems with TLS
        }
        minitl::checked_destroy(m_refCount);
        gameArena().free(m_refCount);
    }
    m_refCount = other.m_refCount;
    m_handle = other.m_handle;
    m_interface = other.m_interface;
    return *this;
}

template< typename Interface >
const RTTI::ClassInfo* Plugin<Interface>::pluginNamespace() const
{
    if (m_handle)
    {
        const RTTI::ClassInfo* (*be_pluginNamespace)() = reinterpret_cast<const RTTI::ClassInfo* (*)()>(reinterpret_cast<size_t>(dlsym(m_handle, "be_pluginNamespace")));
        if (be_pluginNamespace)
            return (*be_pluginNamespace)();
    }
    return 0;
}




}

