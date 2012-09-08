/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/plugin.hh>
#include    <core/environment.hh>
#include    <rtti/classinfo.script.hh>



namespace BugEngine
{

static void* loadLibrary(const inamespace& pluginName)
{
}

template< typename Interface >
Plugin<Interface>::Plugin(const inamespace &pluginName, PreloadType /*preload*/)
:   m_handle(loadLibrary(pluginName))
,   m_interface(0)
,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
}

template< typename Interface >
Plugin<Interface>::Plugin(const inamespace &pluginName, const PluginContext& context)
:   m_handle(loadLibrary(pluginName))
,   m_interface(0)
,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
    if  (m_handle)
    {
        Interface* (*_init)(const PluginContext&) = reinterpret_cast<Interface* (*)(const PluginContext&)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_createPlugin")));
        be_assert(_init, "could not find method _init in plugin %s" | pluginName);
        m_interface = (*_init)(context);
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
        if (m_handle && m_interface)
        {
            void (*_fini)(Interface*) = reinterpret_cast<void (*)(Interface*)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_destroyPlugin")));
            if (_fini)
                _fini(m_interface);
            //dlclose(m_handle); crashes on systems with TLS
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
        const RTTI::Class* (*be_pluginNamespace)() = reinterpret_cast<const RTTI::Class* (*)()>(reinterpret_cast<size_t>(dlsym(m_handle, "be_pluginNamespace")));
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

