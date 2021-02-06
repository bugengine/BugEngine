/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_GUI_GTK3_GTK3LOADER_HH_
#define BE_GUI_GTK3_GTK3LOADER_HH_
/**************************************************************************************************/
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/loader.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <gtk3plugin.hh>

namespace BugEngine { namespace Gtk3 {

class Gtk3Plugin;

class Gtk3Loader : public Resource::ILoader
{
private:
    weak< Resource::ResourceManager > m_resourceManager;
    Gtk3Plugin                        m_gtk3Plugin;

public:
    Gtk3Loader(const Plugin::Context& pluginContext);
    ~Gtk3Loader();

    virtual void load(weak< const Resource::Description > script,
                      Resource::Resource&                 resource) override;
    virtual void reload(weak< const Resource::Description > oldScript,
                        weak< const Resource::Description > newScript,
                        Resource::Resource&                 resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}}  // namespace BugEngine::Gtk3

/**************************************************************************************************/
#endif
