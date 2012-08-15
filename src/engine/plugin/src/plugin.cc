/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/plugin.hh>
#include    <filesystem/folder.script.hh>
#include    <scheduler/scheduler.hh>
#include    <resource/resourcemanager.hh>
#include    <cstring>

namespace BugEngine
{

PluginContext::PluginContext(weak<ResourceManager> manager, ref<Folder> folder, weak<Scheduler> scheduler)
    :   resourceManager(manager)
    ,   dataFolder(folder)
    ,   scheduler(scheduler)
{
}


namespace impl
{

#ifdef BE_STATIC

size_t       PluginList::s_currentPlugin;

PluginList*  PluginList::s_plugins[s_maxPlugins];

PluginList::PluginList(const char* name, Create c, Destroy d, Namespace n)
    :   name(name)
    ,   create(c)
    ,   destroy(d)
    ,   ns(n)
{
    be_assert(s_currentPlugin < s_maxPlugins, "too many plugins registered; increase the value of PluginList::s_maxPlugins to more than %d" | s_maxPlugins);
	be_info("Registering built-in plugin %s" | name);
    s_plugins[s_currentPlugin++] = this;
}

PluginList::~PluginList()
{
    for (size_t i = 0; i < s_currentPlugin; ++i)
    {
        if (s_plugins[i] == this)
        {
            s_plugins[i] = s_plugins[--s_currentPlugin];
            return;
        }
    }
    be_notreached();
}

PluginList* PluginList::findPlugin(const char *name)
{
    be_info("loading plugin %s (built-in)" | name);
    for (size_t i = 0; i < s_currentPlugin; ++i)
    {
        if (strcmp(name, s_plugins[i]->name) ==0)
        {
            return s_plugins[i];
        }
    }
    return 0;
}

#endif

}}
