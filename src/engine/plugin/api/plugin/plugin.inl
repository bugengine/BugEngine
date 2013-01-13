/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/plugin.hh>
#include    <plugin/dynobjectlist.hh>
#include    <core/environment.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>

namespace BugEngine { namespace Plugin
{

#ifdef BE_STATIC
#define _BE_PLUGIN_EXPORT                   static
#define _BE_REGISTER_PLUGIN(id, name)       extern "C" BE_EXPORT BugEngine::Plugin::DynamicObjectList s_plugin_##id (#name);
#define _BE_REGISTER_METHOD(id, type, x)    static bool s_symbol_##id##_##x = s_plugin_##id.registerSymbol<type>(x,#x);
#else
#define _BE_PLUGIN_EXPORT                   extern "C" BE_EXPORT
#define _BE_REGISTER_PLUGIN(id, name)       
#define _BE_REGISTER_METHOD(id, type, x)    
#endif

#define BE_PLUGIN_NAMESPACE_CREATE_(name)                                                                   \
    namespace BugEngine                                                                                     \
    {                                                                                                       \
        BE_EXPORT raw<RTTI::Class> be_##name##_Namespace()                                                  \
        {                                                                                                   \
            static RTTI::ObjectInfo ob = { {0}, {0}, "BugEngine", RTTI::Value() };                          \
            static RTTI::Class ci = { "BugEngine", {0}, {0}, 0, 0, {0}, {0}, {0}, {&ob}, {0}, {0}, 0, 0 };  \
            static raw<const RTTI::ObjectInfo> obptr = {((ob.value = RTTI::Value(&ci)), &ob)};              \
            be_forceuse(obptr);                                                                             \
            raw<RTTI::Class> ptr = {&ci};                                                                   \
            return ptr;                                                                                     \
        }                                                                                                   \
        raw<RTTI::Class> be_##name##_Namespace_BugEngine()                                                  \
        {                                                                                                   \
            return be_##name##_Namespace();                                                                 \
        }                                                                                                   \
    }
#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED__(name, id)                                              \
    BE_PLUGIN_NAMESPACE_CREATE_(id)                                                                 \
    _BE_PLUGIN_EXPORT const BugEngine::RTTI::Class* be_pluginNamespace()                            \
    {                                                                                               \
        return BugEngine::be_##id##_Namespace().operator->();                                       \
    }                                                                                               \
    _BE_REGISTER_PLUGIN(id, name);                                                                  \
    _BE_REGISTER_METHOD(id, const BugEngine::RTTI::Class*(*)(),be_pluginNamespace);

#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(name, id)                                               \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED__(name, id)
#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name)                                                    \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(name, name)
#define BE_PLUGIN_NAMESPACE_REGISTER()                                                              \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(BE_PROJECTNAME, BE_PROJECTID)

#define BE_PLUGIN_REGISTER_NAMED__(name, id, interface, klass)                                      \
    BE_PLUGIN_NAMESPACE_CREATE_(id);                                                                \
    _BE_PLUGIN_EXPORT interface* be_createPlugin (const ::BugEngine::Plugin::Context& context)      \
    {                                                                                               \
        void* m = ::BugEngine::Arena::general().alloc<klass>();                                     \
        return new(m) klass(context);                                                               \
    }                                                                                               \
    _BE_PLUGIN_EXPORT void be_destroyPlugin(klass* cls)                                             \
    {                                                                                               \
        minitl::checked_destroy(cls);                                                               \
        ::BugEngine::Arena::general().free(cls);                                                    \
    }                                                                                               \
    _BE_PLUGIN_EXPORT const BugEngine::RTTI::Class* be_pluginNamespace()                            \
    {                                                                                               \
        return BugEngine::be_##id##_Namespace().operator->();                                       \
    }                                                                                               \
    _BE_REGISTER_PLUGIN(id, name);                                                                  \
    _BE_REGISTER_METHOD(id, interface*(*)(const ::BugEngine::Plugin::Context&),be_createPlugin);    \
    _BE_REGISTER_METHOD(id, void(*)(klass*),be_destroyPlugin);                                      \
    _BE_REGISTER_METHOD(id, const BugEngine::RTTI::Class*(*)(),be_pluginNamespace);
#define BE_PLUGIN_REGISTER_NAMED_(name, id, interface, klass)                                       \
    BE_PLUGIN_REGISTER_NAMED__(name, id, interface, klass)
#define BE_PLUGIN_REGISTER_NAMED(name, interface, klass)                                            \
    BE_PLUGIN_REGISTER_NAMED_(name, name, interface, klass)
#define BE_PLUGIN_REGISTER(interface, klass)                                                        \
    BE_PLUGIN_REGISTER_NAMED_(BE_PROJECTNAME, BE_PROJECTID, interface, klass)


template< typename T >
Plugin<T>::Plugin(const inamespace& pluginName, PreloadType /*preload*/)
    :   m_dynamicObject(new (Arena::general()) DynamicObject(pluginName, "plugins"))
    ,   m_interface(0)
    ,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
}

template< typename T >
Plugin<T>::Plugin(const inamespace& pluginName, const Context& context)
    :   m_dynamicObject(new (Arena::general()) DynamicObject(pluginName, "plugins"))
    ,   m_interface(0)
    ,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
    if (*m_dynamicObject)
    {
        CreateFunction* create = m_dynamicObject->getSymbol<CreateFunction>("be_createPlugin");
        be_assert_recover(create, "could not load method be_createPlugin", return);
        m_interface = (*create)(context);
    }
}

template< typename T >
Plugin<T>::~Plugin()
{
    if (-- *m_refCount == 0)
    {
        if (m_interface)
        {
            DestroyFunction* destroy = m_dynamicObject->getSymbol<DestroyFunction>("be_destroyPlugin");
            be_assert(destroy, "could not load method be_destroyPlugin");
            (*destroy)(m_interface);
        }
        m_dynamicObject->~DynamicObject();
        Arena::general().free(m_refCount);
        Arena::general().free(m_dynamicObject);
    }
}

template< typename T >
Plugin<T>::Plugin(const Plugin& other)
    :   m_dynamicObject(other.m_dynamicObject)
    ,   m_interface(other.m_interface)
    ,   m_refCount(other.m_refCount)
{
    ++ *m_refCount;
}

template< typename T >
Plugin<T>& Plugin<T>::operator=(Plugin other)
{
    other.swap(*this);
}

template< typename T >
void Plugin<T>::swap(Plugin& other)
{
    minitl::swap(m_dynamicObject, other.m_dynamicObject);
    minitl::swap(m_interface, other.m_interface);
    minitl::swap(m_refCount, other.m_refCount);
}

template< typename T >
raw<const RTTI::Class> Plugin<T>::pluginNamespace() const
{
    if (*m_dynamicObject)
    {
        GetPluginNamespace* getNamespace = m_dynamicObject->getSymbol<GetPluginNamespace>("be_pluginNamespace");
        if (getNamespace)
        {
            raw<const RTTI::Class> ci = {(*getNamespace)()};
            return ci;
        }
    }
    raw<const RTTI::Class> ci = {0};
    return ci;

}


}}