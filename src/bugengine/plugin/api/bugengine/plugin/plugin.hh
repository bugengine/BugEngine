/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PLUGIN_PLUGIN_HH_
#define BE_PLUGIN_PLUGIN_HH_
/**************************************************************************************************/
#include <bugengine/plugin/stdafx.h>
#include <bugengine/plugin/dynobject.hh>
#include <bugengine/plugin/hook.hh>

#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/resource/resourcemanager.hh>
#include <bugengine/scheduler/scheduler.hh>

namespace BugEngine {

namespace Meta {
struct Class;
}

namespace Resource {
class ResourceManager;
}
class Folder;
class Scheduler;

namespace Plugin {

struct be_api(PLUGIN) Context
{
    weak< Resource::ResourceManager > resourceManager;
    ref< Folder >                     dataFolder;
    weak< Scheduler >                 scheduler;
    Context(weak< Resource::ResourceManager > manager, ref< Folder > dataFolder,
            weak< Scheduler > scheduler);
};

template < typename Interface >
class Plugin
{
private:
    inamespace                        m_name;
    weak< Resource::ResourceManager > m_resourceManager;
    DynamicObject*                    m_dynamicObject;
    Interface*                        m_interface;
    i_u32*                            m_refCount;

private:
    typedef Interface*(CreateFunction)(const Context& context);
    typedef void(DestroyFunction)(Interface* instance, weak< Resource::ResourceManager > manager);
    typedef const Meta::Class*(GetPluginNamespace)();

public:
    enum PreloadType
    {
        Preload
    };

public:
    Plugin();
    Plugin(const inamespace& pluginName, PreloadType preload);
    Plugin(const inamespace& pluginName, const Context& context);
    Plugin(const Plugin& other);
    ~Plugin();
    Plugin& operator=(Plugin other);

    Interface* operator->()
    {
        return m_interface;
    }
    const Interface* operator->() const
    {
        return m_interface;
    }
    operator const void*() const
    {
        return m_dynamicObject ? m_dynamicObject->operator const void*() : 0;
    }
    bool operator!() const
    {
        return !m_dynamicObject || !*m_dynamicObject;
    }

    raw< const Meta::Class > pluginNamespace() const;
    const inamespace&        name() const
    {
        return m_name;
    }

    void swap(Plugin& other);
};

}  // namespace Plugin
}  // namespace BugEngine

#include <bugengine/plugin/plugin.inl>

/**************************************************************************************************/
#endif
