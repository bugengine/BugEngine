/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_OPENAL_WORLD_HH_
#define BE_AUDIO_OPENAL_WORLD_HH_
/*****************************************************************************/
#include    <audio/iworld.hh>
#include    <audio/isound.hh>
#include    <core/memory/streams.hh>

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

    ref<Audio::ISound> createSound(ref<IMemoryStream> file) override;

    void step() override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}}


/*****************************************************************************/
#endif

