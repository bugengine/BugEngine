/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_SOUND_HH_
#define BE_AUDIO_SOUND_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>

namespace BugEngine { namespace Audio
{

class Source;
class World;

class be_api(AUDIO) Sound : public Object
{
    friend class Source;
private:
    weak<World>                 m_owner;
    ref<AbstractMemoryStream>   m_soundFile;
    void*                       m_data;
    bool                        m_locked;
public:
    Sound(weak<World> owner, ref<AbstractMemoryStream> soundfile);
    ~Sound();

    weak<World> owner() const;

    void reset();

    bool lock(weak<Source> from);
    void unlock();

    size_t read(void* buffer, size_t size, int& frequency, int& channels) const;

    be_metaclass(AUDIO,Sound,Object)
    be_properties
    be_end
};

}}


/*****************************************************************************/
#endif
