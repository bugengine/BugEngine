/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/plugin.hh>
#include    <core/environment.hh>
#include    <rtti/classinfo.script.hh>

namespace BugEngine { namespace Plugin
{

#define BE_PLUGIN_NAMESPACE_CREATE_(name)                                                                   \
    namespace BugEngine                                                                                     \
    {                                                                                                       \
        BE_EXPORT raw<RTTI::Class> be_##name##_Namespace()                                                  \
        {                                                                                                   \
            static RTTI::Class::ObjectInfo ob = { {0}, {0}, "BugEngine", RTTI::Value() };                   \
            static RTTI::Class ci = { "BugEngine", {0}, {0}, 0, 0, {0}, {0}, {0}, {&ob}, {0}, {0}, 0, 0 };  \
            static raw<const RTTI::Class::ObjectInfo> obptr = {((ob.value = RTTI::Value(&ci)), &ob)};       \
            be_forceuse(obptr);                                                                             \
            raw<RTTI::Class> ptr = {&ci};                                                                   \
            return ptr;                                                                                     \
        }                                                                                                   \
        raw<RTTI::Class> be_##name##_Namespace_BugEngine()                                                  \
        {                                                                                                   \
            return be_##name##_Namespace();                                                                 \
        }                                                                                                   \
    }
#define BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name)                                                    \
    BE_PLUGIN_NAMESPACE_CREATE_(name)                                                               \
    _BE_PLUGIN_EXPORT const BugEngine::RTTI::Class* be_pluginNamespace()                            \
    {                                                                                               \
        return BugEngine::be_##name##_Namespace().operator->();                                     \
    }                                                                                               \
    _BE_REGISTER_PLUGIN(name);                                                                      \
    _BE_REGISTER_METHOD(name, const BugEngine::RTTI::Class*(*)(),"be_pluginNamespace");

#define BE_PLUGIN_NAMESPACE_REGISTER_(name)                                                         \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name)
#define BE_PLUGIN_NAMESPACE_REGISTER()                                                              \
    BE_PLUGIN_NAMESPACE_REGISTER_(BE_PROJECTSHORTNAME)

#define BE_PLUGIN_REGISTER_NAMED(name, interface, klass)                                            \
    BE_PLUGIN_NAMESPACE_REGISTER_NAMED(name);                                                       \
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
    _BE_REGISTER_PLUGIN(name);                                                                      \
    _BE_REGISTER_METHOD(name, interface*(*)(const ::BugEngine::Plugin::Context&),"be_createPlugin");\
    _BE_REGISTER_METHOD(name, void(*)(klass*),"be_destroyPlugin");                                  \
    _BE_REGISTER_METHOD(name, const BugEngine::RTTI::Class*(*)(),"be_pluginNamespace");
#define BE_PLUGIN_REGISTER_NAMED_(name, interface, klass)                                           \
    BE_PLUGIN_REGISTER_NAMED(name, interface, klass)
#define BE_PLUGIN_REGISTER(interface, klass)                                                        \
    BE_PLUGIN_REGISTER_NAMED_(BE_PROJECTSHORTNAME, interface, klass)


template< typename T >
Plugin<T>::Plugin(const inamespace& pluginName, PreloadType preload)
    :   DynamicObject(pluginName, "plugins")
{
    be_forceuse(preload);
}

template< typename T >
Plugin<T>::Plugin(const inamespace& pluginName, const Context& context)
    :   DynamicObject(pluginName, "plugins")
{
    be_forceuse(context);
}

template< typename T >
Plugin<T>::~Plugin()
{
    
}


}}