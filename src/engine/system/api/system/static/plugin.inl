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
    typedef void*(*Create)(const PluginContext& context);
    typedef void(*Destroy)(void*);
    typedef raw<RTTI::Class> (*Namespace)();
public:
    const char* const   name;
    Create const        create;
    Destroy const       destroy;
    Namespace const     ns;
public:
    PluginList(const char* name, Create c, Destroy d, Namespace n);
    ~PluginList();
    static PluginList* findPlugin(const char *name);
};

}}

#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED(fullname, name)                              \
    BE_PLUGIN_NAMESPACE_CREATE_(name)                                                   \
    BE_EXPORT BugEngine::impl::PluginList s_##name##Plugin(                             \
            #fullname,                                                                  \
            0,                                                                          \
            0,                                                                          \
            BugEngine::be_##name##_Namespace);
#define BE_PLUGIN_NAMESPACE_REGISTER_(fullname, name)                                   \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED(fullname, name)
#define BE_PLUGIN_NAMESPACE_REGISTER()                                                  \
    BE_PLUGIN_NAMESPACE_REGISTER_(BE_PROJECTNAME, BE_PROJECTSHORTNAME)


#define BE_PLUGIN_REGISTER_NAMED(fullname, name, interface, klass)                      \
    BE_PLUGIN_NAMESPACE_CREATE_(name)                                                   \
    static interface* be_createPlugin (const ::BugEngine::PluginContext& context)       \
    {                                                                                   \
        void* m = ::BugEngine::gameArena().alloc<klass>();                              \
        return new(m) klass(context);                                                   \
    }                                                                                   \
    static void be_destroyPlugin(klass* cls)                                            \
    {                                                                                   \
        minitl::checked_destroy(cls);                                                   \
        ::BugEngine::gameArena().free(cls);                                             \
    }                                                                                   \
    BE_EXPORT BugEngine::impl::PluginList s_##name##Plugin(                             \
            #fullname,                                                                  \
            reinterpret_cast<BugEngine::impl::PluginList::Create>(be_createPlugin),     \
            reinterpret_cast<BugEngine::impl::PluginList::Destroy>(be_destroyPlugin),   \
            BugEngine::be_##name##_Namespace);
#define BE_PLUGIN_REGISTER_NAMED_(fullname, name, interface, klass)                     \
    BE_PLUGIN_REGISTER_NAMED(fullname, name, interface, klass)
#define BE_PLUGIN_REGISTER(interface, klass)                                            \
    BE_PLUGIN_REGISTER_NAMED_(BE_PROJECTNAME, BE_PROJECTSHORTNAME, interface, klass)


namespace BugEngine
{

template< typename Interface >
Plugin<Interface>::Plugin(const inamespace &pluginName, PreloadType preload)
:   m_handle(impl::PluginList::findPlugin(pluginName.str().c_str()))
,   m_interface(0)
,   m_refCount(new (gameArena()) i_u32(1))
{
}

template< typename Interface >
Plugin<Interface>::Plugin(const inamespace &pluginName, const PluginContext& context)
:   m_handle(impl::PluginList::findPlugin(pluginName.str().c_str()))
,   m_interface(0)
,   m_refCount(new (gameArena()) i_u32(1))
{
    if (m_handle)
    {
        m_interface = (reinterpret_cast<Interface*(*)(const PluginContext&)>(static_cast<const impl::PluginList*>(m_handle)->create))(context);
    }
    else
    {
        be_error("Could not load plugin %s" | pluginName);
    }
}

template< typename Interface >
Plugin<Interface>::~Plugin(void)
{
    if (!--*m_refCount)
    {
        if (m_handle && m_interface)
        {
            (reinterpret_cast<void(*)(Interface*)>(static_cast<const impl::PluginList*>(m_handle)->destroy))(m_interface);
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
            (reinterpret_cast<void(*)(Interface*)>(static_cast<const impl::PluginList*>(m_handle)->destroy))(m_interface);
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
raw<const RTTI::Class> Plugin<Interface>::pluginNamespace() const
{
    return (*static_cast<const impl::PluginList*>(m_handle)->ns)();
}


}

