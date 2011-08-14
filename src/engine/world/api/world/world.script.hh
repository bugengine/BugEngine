/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_WORLD_HH_
#define BE_WORLD_WORLD_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <system/scheduler/task/itask.hh>
#include    <system/resource/resource.script.hh>

namespace BugEngine
{

be_tag(ResourceLoaders())
class be_api(WORLD) World : public Resource
{
    friend class WorldScene;
private:
    minitl::vector< ref<ITask> >                m_tasks;
    minitl::vector<ITask::CallbackConnection>   m_callbacks;
private:
    void copyWorld();
    void updateWorld();
public:
    weak<ITask> updateWorldTask() const;
published:
    World(float3 worldExtents);
    ~World();
};

}


/*****************************************************************************/
#endif
