/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SOUND_SOURCE_HH_
#define BE_SOUND_SOURCE_HH_
/*****************************************************************************/
#include    <sound/sound.hh>
#include    <system/action.hh>

namespace BugEngine { namespace Sound
{
    
class SOUNDEXPORT Source : public Object
{
private:
    refptr<SoundObject> m_sound;
    void*               m_data;
    size_t              m_pauseCount;
public:
    Source();
    Source(SoundObject* sound);
    ~Source();

    void play();
    void pause();

    void setSound(SoundObject* sound);

    float volume() const;
    void  setVolume(float volume);

    virtual void step() const;

    be_metaclass(SOUNDEXPORT,Source,Object)
        refptr<Source> createFromSound(SoundObject* sound);
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
