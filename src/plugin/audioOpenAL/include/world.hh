/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_OPENAL_WORLD_HH_
#define BE_AUDIO_OPENAL_WORLD_HH_
/*****************************************************************************/
#include    <audio/iworld.hh>
#include    <audio/isound.hh>

namespace BugEngine { namespace Audio { namespace OpenAL
{

class Sound;
class Source;

class World : public Audio::IWorld
{
    friend class Sound;
    friend class Source;
private:
    void*   m_system;
    void*   m_context;
public:
    World();
    ~World();

    ref<Audio::ISound> createSound(const ifilename& file) override;
    ref<Audio::ISound> createMusic(const ifilename& file) override;

    void step() override;
public:
    void* operator new(size_t size)                  { return ::operator new(size); }
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
public:
    void  operator delete(void* memory)              { return ::operator delete(memory); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
};

class WorldManager : public Audio::IWorldManager
{
protected:
    ref<Audio::IWorld> createWorld();
};

}}}


/*****************************************************************************/
#endif

