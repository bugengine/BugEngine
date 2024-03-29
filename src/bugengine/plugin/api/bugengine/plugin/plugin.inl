/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin/stdafx.h>
#include <bugengine/core/environment.hh>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/helper/staticarray.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/value.hh>
#include <bugengine/plugin/dynobjectlist.hh>
#include <bugengine/plugin/hook.hh>
#include <bugengine/plugin/plugin.hh>

namespace BugEngine { namespace Plugin {

#define BE_PLUGIN_NAMESPACE_CREATE_(name)                                                          \
    namespace BugEngine {                                                                          \
    raw< Meta::Class > be_##name##_Namespace()                                                     \
    {                                                                                              \
        static Meta::ObjectInfo              ob    = {{0}, {0}, "BugEngine", Meta::Value()};       \
        static Meta::Class                   ci    = {istring("BugEngine"),                        \
                                 0,                                           \
                                 0,                                           \
                                 Meta::ClassType_Namespace,                   \
                                 {0},                                         \
                                 {0},                                         \
                                 {&ob},                                       \
                                 {0},                                         \
                                 {0, 0},                                      \
                                 {0, 0},                                      \
                                 {0},                                         \
                                 {0},                                         \
                                 0,                                           \
                                 0};                                          \
        static raw< const Meta::ObjectInfo > obptr = {((ob.value = Meta::Value(&ci)), &ob)};       \
        be_forceuse(obptr);                                                                        \
        raw< Meta::Class > ptr = {&ci};                                                            \
        return ptr;                                                                                \
    }                                                                                              \
    }

#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED__(name, id)                                             \
    BE_PLUGIN_NAMESPACE_CREATE_(id)                                                                \
    _BE_PLUGIN_EXPORT const BugEngine::Meta::Class* be_pluginNamespace()                           \
    {                                                                                              \
        return BugEngine::be_##id##_Namespace().operator->();                                      \
    }                                                                                              \
    _BE_REGISTER_PLUGIN(id, name);                                                                 \
    _BE_REGISTER_METHOD(id, be_pluginNamespace);

#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(name, id) BE_PLUGIN_NAMESPACE_REGISTER_NAMED__(name, id)

#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name) BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(name, name)

#define BE_PLUGIN_NAMESPACE_REGISTER()                                                             \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(BE_PROJECTNAME, BE_PROJECTID)

#define BE_PLUGIN_REGISTER_NAMED__(name, id, create)                                               \
    BE_PLUGIN_NAMESPACE_CREATE_(id);                                                               \
    minitl::intrusive_list< BugEngine::Plugin::IPluginHook > g_pluginHooks_##id;                   \
    _BE_PLUGIN_EXPORT                                                                              \
    minitl::refcountable* be_createPlugin(const ::BugEngine::Plugin::Context& context)             \
    {                                                                                              \
        for(minitl::intrusive_list< BugEngine::Plugin::IPluginHook >::iterator it                  \
            = g_pluginHooks_##id.begin();                                                          \
            it != g_pluginHooks_##id.end(); ++it)                                                  \
            it->onload(context);                                                                   \
        ref< minitl::refcountable > r = (*create)(context);                                        \
        if(r) r->addref();                                                                         \
        return r.operator->();                                                                     \
    }                                                                                              \
    _BE_PLUGIN_EXPORT void be_destroyPlugin(minitl::refcountable*                        cls,      \
                                            weak< BugEngine::Resource::ResourceManager > manager)  \
    {                                                                                              \
        if(cls) cls->decref();                                                                     \
        for(minitl::intrusive_list< BugEngine::Plugin::IPluginHook >::iterator it                  \
            = g_pluginHooks_##id.begin();                                                          \
            it != g_pluginHooks_##id.end(); ++it)                                                  \
            it->onunload(manager);                                                                 \
    }                                                                                              \
    _BE_PLUGIN_EXPORT const BugEngine::Meta::Class* be_pluginNamespace()                           \
    {                                                                                              \
        return BugEngine::be_##id##_Namespace().operator->();                                      \
    }                                                                                              \
    _BE_REGISTER_PLUGIN(id, name);                                                                 \
    _BE_REGISTER_METHOD(id, be_createPlugin);                                                      \
    _BE_REGISTER_METHOD(id, be_destroyPlugin);                                                     \
    _BE_REGISTER_METHOD(id, be_pluginNamespace);

#define BE_PLUGIN_REGISTER_NAMED_(name, id, create) BE_PLUGIN_REGISTER_NAMED__(name, id, create)

#define BE_PLUGIN_REGISTER_NAMED(name, create) BE_PLUGIN_REGISTER_NAMED_(name, name, create)

#define BE_PLUGIN_REGISTER_CREATE(create)                                                          \
    BE_PLUGIN_REGISTER_NAMED_(BE_PROJECTNAME, BE_PROJECTID, create)

#define BE_PLUGIN_REGISTER__(klass, project)                                                       \
    static ref< klass > create(const BugEngine::Plugin::Context& context)                          \
    {                                                                                              \
        return ref< klass >::create(BugEngine::Arena::game(), context);                            \
    }                                                                                              \
    BE_PLUGIN_REGISTER_CREATE(&create)

#define BE_PLUGIN_REGISTER_(klass, project) BE_PLUGIN_REGISTER__(klass, project)

#define BE_PLUGIN_REGISTER(klass) BE_PLUGIN_REGISTER_(klass, BE_PROJECTID)

template < typename T >
Plugin< T >::Plugin()
    : m_name("")
    , m_dynamicObject()
    , m_interface(0)
    , m_refCount(new(Arena::general()) i_u32(i_u32::create(1)))
{
}

template < typename T >
Plugin< T >::Plugin(const inamespace& pluginName, PreloadType /*preload*/)
    : m_name(pluginName)
    , m_dynamicObject(new(Arena::general()) DynamicObject(pluginName, ipath("plugin")))
    , m_interface(0)
    , m_refCount(new(Arena::general()) i_u32(i_u32::create(1)))
{
}

template < typename T >
Plugin< T >::Plugin(const inamespace& pluginName, const Context& context)
    : m_name(pluginName)
    , m_resourceManager(context.resourceManager)
    , m_dynamicObject(new(Arena::general()) DynamicObject(pluginName, ipath("plugin")))
    , m_interface(0)
    , m_refCount(new(Arena::general()) i_u32(i_u32::create(1)))
{
    if(*m_dynamicObject)
    {
        CreateFunction* create = m_dynamicObject->getSymbol< CreateFunction >("be_createPlugin");
        be_assert_recover(create, "could not load method be_createPlugin", return );
        m_interface = (*create)(context);
    }
}

template < typename T >
Plugin< T >::~Plugin()
{
    if(--*m_refCount == 0)
    {
        if(m_interface)
        {
            DestroyFunction* destroy
                = m_dynamicObject->getSymbol< DestroyFunction >("be_destroyPlugin");
            be_assert(destroy, "could not load method be_destroyPlugin");
            (*destroy)(m_interface, m_resourceManager);
        }
        if(m_dynamicObject)
        {
            m_dynamicObject->~DynamicObject();
            Arena::general().free(m_dynamicObject);
        }
        Arena::general().free(m_refCount);
    }
}

template < typename T >
Plugin< T >::Plugin(const Plugin& other)
    : m_name(other.m_name)
    , m_dynamicObject(other.m_dynamicObject)
    , m_interface(other.m_interface)
    , m_refCount(other.m_refCount)
{
    ++*m_refCount;
}

template < typename T >
Plugin< T >& Plugin< T >::operator=(Plugin other)
{
    other.swap(*this);
    return *this;
}

template < typename T >
void Plugin< T >::swap(Plugin& other)
{
    minitl::swap(m_name, other.m_name);
    minitl::swap(m_dynamicObject, other.m_dynamicObject);
    minitl::swap(m_interface, other.m_interface);
    minitl::swap(m_refCount, other.m_refCount);
}

template < typename T >
raw< const Meta::Class > Plugin< T >::pluginNamespace() const
{
    if(m_dynamicObject && *m_dynamicObject)
    {
        GetPluginNamespace* getNamespace
            = m_dynamicObject->getSymbol< GetPluginNamespace >("be_pluginNamespace");
        if(getNamespace)
        {
            raw< const Meta::Class > ci = {(*getNamespace)()};
            return ci;
        }
    }
    raw< const Meta::Class > ci = {0};
    return ci;
}

}}  // namespace BugEngine::Plugin
