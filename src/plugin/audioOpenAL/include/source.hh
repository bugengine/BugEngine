/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_OPENAL_SOURCE_HH_
#define BE_AUDIO_OPENAL_SOURCE_HH_
/*****************************************************************************/
#include    <sound.hh>
#include    <audio/source.hh>
#include    <system/action.hh>

namespace BugEngine { namespace Audio { namespace OpenAL
{

class Source : public Audio::Source 
{
private:
    size_t      m_pauseCount;
public:
    Source(ref<Sound> sound);
    ~Source();

    void play() override;
    void pause() override;

    float volume() const override;
    void  setVolume(float volume) override;

    virtual void step() const override;
};

}}}


/*****************************************************************************/
#endif
