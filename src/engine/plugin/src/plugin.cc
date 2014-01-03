/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/plugin.hh>
#include    <filesystem/folder.script.hh>
#include    <scheduler/scheduler.hh>
#include    <resource/resourcemanager.hh>

namespace BugEngine { namespace Plugin
{

Context::Context(weak<Resource::ResourceManager> manager, ref<Folder> folder, weak<Scheduler> scheduler)
    :   resourceManager(manager)
    ,   dataFolder(folder)
    ,   scheduler(scheduler)
{
}

}}
