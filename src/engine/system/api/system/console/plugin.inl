/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <core/environment.hh>

namespace BugEngine { namespace impl
{

class be_api(SYSTEM) PluginList
{
    BE_NOCOPY(PluginList);
private:
    static const size_t s_maxPlugins = 64;
    static size_t       s_currentPlugin;
    static PluginList*  s_plugins[s_maxPlugins];
public:
    typedef void*(*Create)();
    typedef void(*Destroy)(void*);
public:
    const char* const   name;
    Create const        create;
    Destroy const       destroy;
public:
    PluginList(const char* name, Create c, Destroy d);
    ~PluginList();
    static PluginList* findPlugin(const char *name);
};

}}

#define BE_PLUGIN_REGISTER(name, klass, params, args)                                                                                   \
    static klass* be_createPlugin params { void* m = BugEngine::gameArena().alloc<klass>(); return new(m) klass args; }                 \
    static void be_destroyPlugin(klass* cls) { minitl::checked_destroy(cls); BugEngine::gameArena().free(cls); }                        \
    static BugEngine::impl::PluginList s_##name##Plugin( #name,                                                                         \
                                                         reinterpret_cast<BugEngine::impl::PluginList::Create>(be_createPlugin),        \
                                                         reinterpret_cast<BugEngine::impl::PluginList::Destroy>(be_destroyPlugin));

namespace BugEngine
{

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName, PreloadType preload)
:   m_handle(impl::PluginList::findPlugin(pluginName.c_str()))
,   m_interface(0)
{
}

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName)
:   m_handle(impl::PluginList::findPlugin(pluginName.c_str()))
,   m_interface(0)
{
    if (m_handle)
    {
        m_interface = (reinterpret_cast<Interface*(*)()>(static_cast<const impl::PluginList*>(m_handle)->create))();
    }
}

template< typename Interface >
template< typename T1 >
Plugin<Interface>::Plugin(const istring &pluginName, T1 param1)
:   m_handle(impl::PluginList::findPlugin(pluginName.c_str()))
,   m_interface(0)
{
    if  (m_handle)
    {
        m_interface = (reinterpret_cast<Interface*(*)(T1)>(static_cast<const impl::PluginList*>(m_handle)->create))(param1);
    }
}

template< typename Interface >
template< typename T1, typename T2 >
Plugin<Interface>::Plugin(const istring &pluginName, T1 param1, T2 param2)
:   m_handle(impl::PluginList::findPlugin(pluginName.c_str()))
,   m_interface(0)
{
    if (m_handle)
    {
        m_interface = (reinterpret_cast<Interface*(*)(T1, T2)>(static_cast<const impl::PluginList*>(m_handle)->create))(param1, param2);
    }
}

template< typename Interface >
Plugin<Interface>::~Plugin(void)
{
    if (m_handle)
    {
        (reinterpret_cast<void(*)(Interface*)>(static_cast<const impl::PluginList*>(m_handle)->destroy))(m_interface);
    }
}

template< typename Interface >
weak<const RTTI::Namespace> Plugin<Interface>::pluginNamespace() const
{
    return weak<const RTTI::Namespace>();
}


}

