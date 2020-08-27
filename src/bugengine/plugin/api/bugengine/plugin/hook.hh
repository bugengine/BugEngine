/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PLUGIN_HOOK_HH_
#define BE_PLUGIN_HOOK_HH_
/**************************************************************************************************/
#include <bugengine/plugin/stdafx.h>
#include <bugengine/core/preproc.hh>
#include <bugengine/resource/resourcemanager.hh>

namespace BugEngine { namespace Plugin {
struct Context;
class IPluginHook;
}}  // namespace BugEngine::Plugin

extern minitl::intrusive_list< BugEngine::Plugin::IPluginHook > BE_CONCAT(g_pluginHooks_,
                                                                          BE_PROJECTID);

namespace BugEngine { namespace Plugin {

class be_api(PLUGIN) IPluginHook : public minitl::intrusive_list< IPluginHook >::item
{
protected:
    IPluginHook(minitl::intrusive_list< IPluginHook > & owner)
    {
        owner.push_back(*this);
    }
    virtual ~IPluginHook();

public:
    virtual void onload(const Context& context)                      = 0;
    virtual void onunload(weak< Resource::ResourceManager > manager) = 0;
};

template < typename T >
class PluginHook : public IPluginHook
{
private:
    T m_hook;

public:
    PluginHook(const T& t) : IPluginHook(::BE_CONCAT(g_pluginHooks_, BE_PROJECTID)), m_hook(t)
    {
    }
    ~PluginHook()
    {
    }
    virtual void onload(const Context& context) override
    {
        m_hook.onload(context);
    }
    virtual void onunload(weak< Resource::ResourceManager > manager) override
    {
        m_hook.onunload(manager);
    }
};

}}  // namespace BugEngine::Plugin

/**************************************************************************************************/
#endif
