/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_WORLD_HH_
#define BE_AUDIO_WORLD_HH_
/*****************************************************************************/
#include    <audio/source.hh>
#include    <system/scheduler/task.hh>

namespace BugEngine { namespace Audio
{

class be_api(AUDIO) World : public Object
{
private:
    ref< BaseTask::Callback >                   m_start;
    weak< BaseTask::Callback >                  m_end;
    std::vector< ref< BaseTask > >              m_tasks;
    std::vector< ref< BaseTask::Callback > >    m_edges;
public:
    World(weak<BaseTask::Callback> endJob);
    ~World();

    virtual ref<Sound> createSound(const ifilename& file) = 0;
    virtual ref<Sound> createMusic(const ifilename& file) = 0;

    virtual void step() = 0;

    weak< BaseTask::Callback > getStart() { return m_start; }
    weak< BaseTask::Callback > getEnd()   { return m_end; }

    be_metaclass(AUDIO,World,Object)
    be_properties
    be_end
};

class be_api(AUDIO) WorldManager
{
public:
    virtual ~WorldManager() { }
    virtual ref<World> createWorld(weak<BaseTask::Callback> endJob) = 0;
};


}}


/*****************************************************************************/
#endif
