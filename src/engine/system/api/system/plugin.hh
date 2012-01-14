/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_PLUGIN_HH_
#define BE_SYSTEM_PLUGIN_HH_
/*****************************************************************************/


namespace BugEngine
{

namespace RTTI
{
    struct ClassInfo;
}

class ResourceManager;
struct be_api(SYSTEM) PluginContext
{
    weak<ResourceManager>   resourceManager;
    PluginContext(weak<ResourceManager> manager);
};

template< typename Interface >
class be_api(SYSTEM) Plugin
{
private:
    typedef void*   Handle;
private:
    Handle          m_handle;
    Interface*      m_interface;
    i_u32*          m_refCount;
public:
    enum PreloadType { Preload };
    Plugin(const inamespace &pluginName, PreloadType preload);
    Plugin(const inamespace &pluginName, const PluginContext& context);
    ~Plugin(void);
    Plugin(const Plugin& other);
    Plugin& operator=(const Plugin& other);

    Interface* operator->()             { return m_interface; }
    const Interface* operator->() const { return m_interface; }
    operator const void*() const        { return m_handle; }
    bool operator!() const              { return m_handle == 0; }

    raw<const RTTI::ClassInfo> pluginNamespace() const;
private:
    Plugin();
};

}

#define BE_PLUGIN_NAMESPACE_CREATE_(name)                                                                                               \
    namespace BugEngine                                                                                                                 \
    {                                                                                                                                   \
        BE_EXPORT raw<RTTI::ClassInfo> be_##name##_Namespace()                                                                          \
        {                                                                                                                               \
            static RTTI::ClassInfo::ObjectInfo ob = { {0}, "BugEngine", Value() };                                                      \
            static RTTI::ClassInfo ci = { "BugEngine", {0}, 0, 0, {0}, {0}, {0}, {&ob}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }} };             \
            static raw<const RTTI::ClassInfo::ObjectInfo> obptr = {((ob.value = Value(&ci)), &ob)};                                     \
            be_forceuse(obptr);                                                                                                         \
            raw<RTTI::ClassInfo> ptr = {&ci};                                                                                           \
            return ptr;                                                                                                                 \
        }                                                                                                                               \
        raw<RTTI::ClassInfo> be_##name##_Namespace_BugEngine()                                                                          \
        {                                                                                                                               \
            return be_##name##_Namespace();                                                                                             \
        }                                                                                                                               \
    }
#ifdef BE_STATIC
# include "static/plugin.inl"
#elif defined(PLUGIN_H)
# include PLUGIN_H
#else
# error Plugin subsystem not implemented... define PLUGIN_H in platform_xxx.hh
#endif


/*****************************************************************************/
#endif
