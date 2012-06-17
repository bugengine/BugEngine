/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <system/file/folder.script.hh>
#include    <system/resource/resourcemanager.hh>
#include    <cstring>

namespace BugEngine
{

PluginContext::PluginContext(weak<ResourceManager> manager, ref<Folder> folder)
    :   resourceManager(manager)
    ,   dataFolder(folder)
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
