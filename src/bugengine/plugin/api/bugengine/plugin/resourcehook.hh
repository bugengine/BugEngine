/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PLUGIN_RESOURCEHOOK_HH_
#define BE_PLUGIN_RESOURCEHOOK_HH_
/**************************************************************************************************/
#include <bugengine/plugin/stdafx.h>
#include <bugengine/plugin/hook.hh>
#include <bugengine/plugin/plugin.hh>

namespace BugEngine { namespace Plugin {

template < typename RESOURCE >
struct ResourceHook
{
private:
    ref< const RESOURCE > m_resource;

public:
    ResourceHook(ref< const RESOURCE > resource) : m_resource(resource)
    {
    }
    void onload(const Context& context)
    {
        context.resourceManager->load(m_resource);
    }
    void onunload(weak< Resource::ResourceManager > manager)
    {
        manager->unload(m_resource);
    }
};

}}  // namespace BugEngine::Plugin

/**************************************************************************************************/
#endif
