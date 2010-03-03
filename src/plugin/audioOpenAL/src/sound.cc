/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <sound.hh>

#include    <world.hh>

namespace BugEngine { namespace Audio { namespace OpenAL
{


Sound::Sound(weak<World> owner, ref<IMemoryStream> soundfile)
:   Audio::ISound(owner, soundfile)
{
}

Sound::~Sound()
{
}

}}}

