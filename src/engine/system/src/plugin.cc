/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <cstring>

namespace BugEngine { namespace impl
{

#ifdef BE_STATIC

size_t       PluginList::s_currentPlugin;

PluginList*  PluginList::s_plugins[s_maxPlugins];

PluginList::PluginList(const char* name, Create c, Destroy d)
    :   name(name)
    ,   create(c)
    ,   destroy(d)
{
    be_assert(s_currentPlugin < s_maxPlugins, "too many plugins registered; increase the value of PluginList::s_maxPlugins to more than %d" | s_maxPlugins);
    s_plugins[s_currentPlugin++] = this;
}

PluginList::~PluginList()
{
    for(size_t i = 0; i < s_currentPlugin; ++i)
    {
        if(s_plugins[i] == this)
        {
            s_plugins[i] = s_plugins[--s_currentPlugin];
            return;
        }
    }
    be_notreached();
}

PluginList* PluginList::findPlugin(const char *name)
{
    for(size_t i = 0; i < s_currentPlugin; ++i)
    {
        if(strcmp(name, s_plugins[i]->name) ==0)
        {
            return s_plugins[i];
        }
    }
    return 0;
}

#endif

}}
