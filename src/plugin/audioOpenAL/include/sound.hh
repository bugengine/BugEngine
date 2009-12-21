/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_OPENAL_SOUND_HH_
#define BE_AUDIO_OPENAL_SOUND_HH_
/*****************************************************************************/
#include    <audio/sound.hh>

namespace BugEngine { namespace Audio { namespace OpenAL
{

class Source;
class World;

class Sound : public Audio::Sound
{
    friend class Source;
public:
    Sound(weak<World> owner, ref<AbstractMemoryStream> soundfile);
    ~Sound();
};

}}}


/*****************************************************************************/
#endif
