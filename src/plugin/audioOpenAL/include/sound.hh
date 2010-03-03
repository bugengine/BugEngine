/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_OPENAL_SOUND_HH_
#define BE_AUDIO_OPENAL_SOUND_HH_
/*****************************************************************************/
#include    <audio/isound.hh>

namespace BugEngine { namespace Audio { namespace OpenAL
{

class Source;
class World;

class Sound : public Audio::ISound
{
    friend class Source;
public:
    Sound(weak<World> owner, ref<IMemoryStream> soundfile);
    ~Sound();
};

}}}


/*****************************************************************************/
#endif
