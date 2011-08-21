/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <core/environment.hh>

#include    <dlfcn.h>


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

static void* loadLibrary(const istring& pluginName)
{
    minitl::format<> f = (minitl::format<>("lib%s.so") | pluginName.c_str());
    void* handle = dlopen((Environment::getEnvironment().getDataDirectory() + ipath("plugins") + ifilename(f.c_str())).str().c_str(), RTLD_NOW|RTLD_LOCAL);
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
{
}

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName)
:   m_handle(loadLibrary(pluginName))
,   m_interface(0)
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
    if (m_handle && m_interface)
    {
        void (*_fini)(Interface*) = reinterpret_cast<void (*)(Interface*)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_destroyPlugin")));
        if (_fini)
            _fini(m_interface);
        //dlclose(m_handle); crashes on systems with TLS
    }
}

template< typename Interface >
weak<const RTTI::Namespace> Plugin<Interface>::pluginNamespace() const
{
    if (m_handle)
    {
        const RTTI::Namespace* (*be_pluginNamespace)() = reinterpret_cast<const RTTI::Namespace* (*)()>(reinterpret_cast<size_t>(dlsym(m_handle, "be_pluginNamespace")));
        if (be_pluginNamespace)
            return (*be_pluginNamespace)();
        return weak<const RTTI::Namespace>();
    }
}




}

