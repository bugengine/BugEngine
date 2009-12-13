/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <core/environment.hh>

#include    <stdexcept>

namespace BugEngine { namespace _
{

class be_api(SYSTEM) PluginList
{
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

#define BE_PLUGIN_REGISTER(name, klass, args)                                                                           \
    static klass* be_createPlugin() { return new klass args; }                                                          \
    static void be_destroyPlugin(klass* cls) { delete cls; }                                                            \
    static BugEngine::_::PluginList s_##name##Plugin( #name,                                                \
                                              reinterpret_cast<BugEngine::_::PluginList::Create>(be_createPlugin),      \
                                              reinterpret_cast<BugEngine::_::PluginList::Destroy>(be_destroyPlugin));

namespace BugEngine
{

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName)
:   m_handle(_::PluginList::findPlugin(pluginName.c_str()))
,   m_interface(0)
{
    if(m_handle)
    {
        m_interface = (reinterpret_cast<Interface*(*)()>(static_cast<const _::PluginList*>(m_handle)->create))();
    }
}

template< typename Interface >
Plugin<Interface>::~Plugin(void)
{
    if(m_handle)
    {
        (reinterpret_cast<void(*)(Interface*)>(static_cast<const _::PluginList*>(m_handle)->destroy))(m_interface);
    }
}

}

