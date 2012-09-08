/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/plugin.hh>
#include    <filesystem/folder.script.hh>
#include    <scheduler/scheduler.hh>
#include    <resource/resourcemanager.hh>
#include    <cstring>

namespace BugEngine { namespace Plugin
{

Context::Context(weak<Resource::ResourceManager> manager, ref<Folder> folder, weak<Scheduler> scheduler)
    :   resourceManager(manager)
    ,   dataFolder(folder)
    ,   scheduler(scheduler)
{
}

}
