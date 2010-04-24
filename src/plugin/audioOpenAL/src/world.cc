/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <world.hh>
#include    <sound.hh>

namespace BugEngine { namespace Audio { namespace OpenAL
{

World::World()
:   Audio::IWorld()
,   m_system(0)
,   m_context(0)
{
}

World::~World()
{
}

ref<Audio::ISound> World::createSound(ref<IMemoryStream> soundfile)
{
    return ref<Sound>::create(this, soundfile);
}

void World::step()
{
}

}}}

