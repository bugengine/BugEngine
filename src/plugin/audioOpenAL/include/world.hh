/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_OPENAL_WORLD_HH_
#define BE_AUDIO_OPENAL_WORLD_HH_
/*****************************************************************************/
#include    <audio/world.hh>
#include    <audio/sound.hh>

namespace BugEngine { namespace Audio { namespace OpenAL
{

class Sound;
class Source;

class World : public Audio::World
{
    friend class Sound;
    friend class Source;
private:
    void*   m_system;
    void*   m_context;
public:
    World(weak<BaseTask::Callback> endJob);
    ~World();

    ref<Audio::Sound> createSound(const ifilename& file) override;
    ref<Audio::Sound> createMusic(const ifilename& file) override;

    void step() override;
public:
    void* operator new(size_t size)                  { return ::operator new(size); }
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
public:
    void  operator delete(void* memory)              { return ::operator delete(memory); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
};

class WorldManager : public Audio::WorldManager
{
protected:
    ref<Audio::World> createWorld(weak<BaseTask::Callback> endJob);
};

}}}


/*****************************************************************************/
#endif

