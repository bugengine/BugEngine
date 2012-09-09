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
class Plugin : DynamicObject
{
private:
    Interface*      m_interface;
public:
    enum PreloadType { Preload };
    Plugin(const inamespace &pluginName, PreloadType preload);
    Plugin(const inamespace &pluginName, const Context& context);
    ~Plugin();

    Interface* operator->()             { return m_interface; }
    const Interface* operator->() const { return m_interface; }

    raw<const RTTI::Class> pluginNamespace() const;
};

}}

#include    <plugin/plugin.inl>

/*****************************************************************************/
#endif
