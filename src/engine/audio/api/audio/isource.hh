/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_ISOURCE_HH_
#define BE_AUDIO_ISOURCE_HH_
/*****************************************************************************/
#include    <audio/isound.hh>
#include    <system/action.hh>

namespace BugEngine { namespace Audio
{
    
class be_api(AUDIO) ISource : public Object
{
protected:
    ref<ISound> m_sound;
public:
    ISource(ref<ISound> sound);
    ~ISource();

    virtual void play() = 0;
    virtual void pause() = 0;

    virtual float volume() const = 0;
    virtual void  setVolume(float volume) = 0;

    virtual void step() const = 0;

    be_metaclass(AUDIO,ISource,Object)
    be_properties
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
