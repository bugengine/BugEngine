/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <gobject.hh>
#include <gtk3loader.hh>
#include <gtk3plugin.hh>

namespace BugEngine { namespace Gtk3 {

Gtk3Loader::Gtk3Loader(const Plugin::Context& pluginContext)
    : ILoader()
    , m_resourceManager(pluginContext.resourceManager)
    , m_gtk3Plugin()
{
    // m_resourceManager->attach(GObject::gObjectClass(), this);
}

Gtk3Loader::~Gtk3Loader()
{
    // m_resourceManager->detach(GObject::gObjectClass(), this);
}

void Gtk3Loader::load(weak< const Resource::Description > description, Resource::Resource& resource)
{
    be_forceuse(description);
    be_forceuse(resource);
}

void Gtk3Loader::reload(weak< const Resource::Description > oldDescription,
                        weak< const Resource::Description > newDescription,
                        Resource::Resource&                 resource)
{
    be_forceuse(oldDescription);
    be_forceuse(newDescription);
    be_forceuse(resource);
}

void Gtk3Loader::unload(Resource::Resource& resource)
{
    resource.clearRefHandle();
}

}}  // namespace BugEngine::Gtk3
