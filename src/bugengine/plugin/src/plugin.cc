/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin/stdafx.h>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/resourcemanager.hh>
#include <bugengine/scheduler/scheduler.hh>

namespace BugEngine { namespace Plugin {

Context::Context(weak< Resource::ResourceManager > manager, ref< Folder > folder,
                 weak< Scheduler > scheduler)
    : resourceManager(manager)
    , dataFolder(folder)
    , scheduler(scheduler)
{
}

}}  // namespace BugEngine::Plugin
