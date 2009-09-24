/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SOUND_SOUNDOBJECT_HH_
#define BE_SOUND_SOUNDOBJECT_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>

namespace BugEngine { namespace Sound
{

class Source;
class World;

class be_api(SOUND) SoundObject : public Object
{
    friend class Source;
private:
    World*                          m_owner;
    refptr<AbstractMemoryStream>    m_soundFile;
    void*                           m_data;
    bool                            m_locked;
public:
    SoundObject(World* owner, refptr<AbstractMemoryStream> soundfile);
    ~SoundObject();

    World* owner() const;

    void reset();

    bool lock(Source* from);
    void unlock();

    size_t read(void* buffer, size_t size, int& frequency, int& channels) const;

    be_metaclass(SOUND,SoundObject,Object)
    be_properties
    be_end
private:
    SoundObject(const SoundObject& other);
    SoundObject& operator=(const SoundObject& other);
};

}}


/*****************************************************************************/
#endif
