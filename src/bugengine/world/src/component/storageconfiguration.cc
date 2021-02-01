
/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/world/stdafx.h>
#include <bugengine/world/component/storageconfiguration.script.hh>

#include <bugengine/scheduler/task/method.hh>

namespace BugEngine { namespace World { namespace Component {

StorageConfiguration::StorageConfiguration()
    : m_updateStart(
        ref< Task::Task<
            Task::MethodCaller< StorageConfiguration, &StorageConfiguration::update > > >::
            create(Arena::task(), "storage:update", Colors::make(89, 89, 180),
                   Task::MethodCaller< StorageConfiguration, &StorageConfiguration::update >(this)))
{
}

StorageConfiguration::~StorageConfiguration()
{
}

void StorageConfiguration::update()
{
    be_info("storage update");
}

}}}  // namespace BugEngine::World::Component
