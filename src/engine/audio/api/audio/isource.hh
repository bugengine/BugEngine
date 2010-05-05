/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_ISOURCE_HH_
#define BE_AUDIO_ISOURCE_HH_
/*****************************************************************************/
#include    <audio/isound.hh>

namespace BugEngine { namespace Audio
{

class be_api(AUDIO) ISource : public minitl::refcountable
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
};

}}


/*****************************************************************************/
#endif
