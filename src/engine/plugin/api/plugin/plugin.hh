/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PLUGIN_PLUGIN_HH_
#define BE_PLUGIN_PLUGIN_HH_
/*****************************************************************************/
#include    <plugin/dynobject.hh>

namespace BugEngine
{

namespace RTTI
{
    struct Class;
}

namespace Resource
{
class ResourceManager;
}
class Folder;
class Scheduler;

namespace Plugin
{

struct be_api(PLUGIN) Context
{
    weak<Resource::ResourceManager> resourceManager;
    ref<Folder>                     dataFolder;
    weak<Scheduler>                 scheduler;
    Context(weak<Resource::ResourceManager> manager, ref<Folder> dataFolder, weak<Scheduler> scheduler);
};

template< typename Interface >
class Plugin
{
private:
    DynamicObject*  m_dynamicObject;
    Interface*      m_interface;
    i_u32*          m_refCount;
private:
    typedef Interface* (CreateFunction)(const Context& context);
    typedef void (DestroyFunction)(Interface* instance);
    typedef const RTTI::Class* (GetPluginNamespace)();
public:
    enum PreloadType { Preload };
public:
    Plugin(const inamespace &pluginName, PreloadType preload);
    Plugin(const inamespace &pluginName, const Context& context);
    Plugin(const Plugin& other);
    ~Plugin();
    Plugin& operator=(Plugin other);

    Interface* operator->()             { return m_interface; }
    const Interface* operator->() const { return m_interface; }
    operator const void*() const        { return (const void*)m_dynamicObject; }
    bool operator!() const              { return !m_dynamicObject; }

    raw<const RTTI::Class> pluginNamespace() const;

    void swap(Plugin& other);
};

}}

#ifdef BE_STATIC
#define _BE_PLUGIN_EXPORT                   static
#define _BE_REGISTER_PLUGIN(name)           
#define _BE_REGISTER_METHOD(name,type,x)    
#else
#define _BE_PLUGIN_EXPORT                   extern "C" BE_EXPORT
#define _BE_REGISTER_PLUGIN(name)           
#define _BE_REGISTER_METHOD(name,type,x)    
#endif

#include    <plugin/plugin.inl>

/*****************************************************************************/
#endif
