/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SOUND_SOURCE_HH_
#define BE_SOUND_SOURCE_HH_
/*****************************************************************************/
#include    <sound/sound.hh>
#include    <system/action.hh>

namespace BugEngine { namespace Sound
{
    
class be_api(SOUND) Source : public Object
{
private:
    ref<SoundObject>    m_sound;
    void*               m_data;
    size_t              m_pauseCount;
public:
    Source();
    Source(SoundObject* sound);
    ~Source();

    void play();
    void pause();

    void setSound(ref<SoundObject> sound);

    float volume() const;
    void  setVolume(float volume);

    virtual void step() const;

    be_metaclass(SOUND,Source,Object)
        ref<Source> createFromSound(ref<SoundObject> sound);
    be_properties
        be_classmethod(createFromSound);

        be_method(setSound);

        be_method(volume);
        be_method(setVolume);
        be_method(play);
        be_method(pause);

        be_property(Volume)
            [be_get(volume)]
            [be_set(setVolume)];
    be_end
};

}}


/*****************************************************************************/
#endif
