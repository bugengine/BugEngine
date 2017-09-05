/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/plugin.hh>
#include    <plugin/dynobjectlist.hh>
#include    <core/environment.hh>
#include    <rtti/classinfo.script.hh>


namespace BugEngine { namespace Plugin
{

#define BE_PLUGIN_NAMESPACE_CREATE_(name)                                                           \
    namespace BugEngine                                                                             \
    {                                                                                               \
        BE_EXPORT raw<RTTI::Class> be_##name##_Namespace()                                          \
        {                                                                                           \
            static RTTI::ObjectInfo ob = { {0}, {&RTTI::staticarray<const RTTI::Tag>::s_null},      \
                                          "BugEngine", RTTI::Value() };                             \
            static RTTI::Class ci = { "BugEngine", 0, 0, RTTI::ClassType_Namespace, {0}, {0},       \
                                      {&ob}, {&RTTI::staticarray<const RTTI::Tag>::s_null},         \
                                      {&RTTI::staticarray<const RTTI::Property>::s_null},           \
                                      {&RTTI::staticarray<const RTTI::Method>::s_null}, {0}, 0, 0 };\
            static raw<const RTTI::ObjectInfo> obptr = {((ob.value = RTTI::Value(&ci)), &ob)};      \
            be_forceuse(obptr);                                                                     \
            raw<RTTI::Class> ptr = {&ci};                                                           \
            return ptr;                                                                             \
        }                                                                                           \
        raw<RTTI::Class> be_##name##_Namespace_BugEngine()                                          \
        {                                                                                           \
            return be_##name##_Namespace();                                                         \
        }                                                                                           \
    }
#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED__(name, id)                                              \
    BE_PLUGIN_NAMESPACE_CREATE_(id)                                                                 \
    _BE_PLUGIN_EXPORT const BugEngine::RTTI::Class* be_pluginNamespace()                            \
    {                                                                                               \
        return BugEngine::be_##id##_Namespace().operator->();                                       \
    }                                                                                               \
    _BE_REGISTER_PLUGIN(id, name);                                                                  \
    _BE_REGISTER_METHOD(id, be_pluginNamespace);

#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(name, id)                                               \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED__(name, id)
#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name)                                                    \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(name, name)
#define BE_PLUGIN_NAMESPACE_REGISTER()                                                              \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED_(BE_PROJECTNAME, BE_PROJECTID)

#define BE_PLUGIN_REGISTER_NAMED__(name, id, create)                                                \
    BE_PLUGIN_NAMESPACE_CREATE_(id);                                                                \
    _BE_PLUGIN_EXPORT                                                                               \
    minitl::refcountable* be_createPlugin (const ::BugEngine::Plugin::Context& context)             \
    {                                                                                               \
        ref<minitl::refcountable> r = (*create)(context);                                           \
        if (r)                                                                                      \
            r->addref();                                                                            \
        return r.operator->();                                                                      \
    }                                                                                               \
    _BE_PLUGIN_EXPORT void be_destroyPlugin(minitl::refcountable* cls)                              \
    {                                                                                               \
        if (cls)                                                                                    \
            cls->decref();                                                                          \
    }                                                                                               \
    _BE_PLUGIN_EXPORT const BugEngine::RTTI::Class* be_pluginNamespace()                            \
    {                                                                                               \
        return BugEngine::be_##id##_Namespace().operator->();                                       \
    }                                                                                               \
    _BE_REGISTER_PLUGIN(id, name);                                                                  \
    _BE_REGISTER_METHOD(id, be_createPlugin);                                                       \
    _BE_REGISTER_METHOD(id, be_destroyPlugin);                                                      \
    _BE_REGISTER_METHOD(id, be_pluginNamespace);
#define BE_PLUGIN_REGISTER_NAMED_(name, id, create)                                                 \
    BE_PLUGIN_REGISTER_NAMED__(name, id, create)
#define BE_PLUGIN_REGISTER_NAMED(name, create)                                                      \
    BE_PLUGIN_REGISTER_NAMED_(name, name, create)
#define BE_PLUGIN_REGISTER_CREATE(create)                                                           \
    BE_PLUGIN_REGISTER_NAMED_(BE_PROJECTNAME, BE_PROJECTID, create)
#define BE_PLUGIN_REGISTER(klass)                                                                   \
    static ref<klass> create(const BugEngine::Plugin::Context& context)                             \
    {                                                                                               \
        return ref<klass>::create(BugEngine::Arena::game(), context);                               \
    }                                                                                               \
    BE_PLUGIN_REGISTER_CREATE(&create)

template< typename T >
Plugin<T>::Plugin()
    :   m_name("")
    ,   m_dynamicObject()
    ,   m_interface(0)
    ,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
}

template< typename T >
Plugin<T>::Plugin(const inamespace& pluginName, PreloadType /*preload*/)
    :   m_name(pluginName)
    ,   m_dynamicObject(new (Arena::general()) DynamicObject(pluginName, "plugin"))
    ,   m_interface(0)
    ,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
}

template< typename T >
Plugin<T>::Plugin(const inamespace& pluginName, const Context& context)
    :   m_name(pluginName)
    ,   m_dynamicObject(new (Arena::general()) DynamicObject(pluginName, "plugin"))
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
        if (m_dynamicObject)
        {
            m_dynamicObject->~DynamicObject();
            Arena::general().free(m_dynamicObject);
        }
        Arena::general().free(m_refCount);
    }
}

template< typename T >
Plugin<T>::Plugin(const Plugin& other)
    :   m_name(other.m_name)
    ,   m_dynamicObject(other.m_dynamicObject)
    ,   m_interface(other.m_interface)
    ,   m_refCount(other.m_refCount)
{
    ++ *m_refCount;
}

template< typename T >
Plugin<T>& Plugin<T>::operator=(Plugin other)
{
    other.swap(*this);
    return *this;
}

template< typename T >
void Plugin<T>::swap(Plugin& other)
{
    minitl::swap(m_name, other.m_name);
    minitl::swap(m_dynamicObject, other.m_dynamicObject);
    minitl::swap(m_interface, other.m_interface);
    minitl::swap(m_refCount, other.m_refCount);
}

template< typename T >
raw<const RTTI::Class> Plugin<T>::pluginNamespace() const
{
    if (m_dynamicObject && *m_dynamicObject)
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
