/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <world.hh>
#include    <sound.hh>

#include    <system/filesystem.hh>


namespace BugEngine { namespace Audio { namespace OpenAL
{

World::World()
:   Audio::World()
,   m_system(0)
,   m_context(0)
{
}

World::~World()
{
}

ref<Audio::Sound> World::createSound(const ifilename& file)
{
    ref<AbstractMemoryStream> soundfile = FileSystem::instance()->open(file, eReadOnly);
    return ref<Sound>::create(this, soundfile);
}

ref<Audio::Sound> World::createMusic(const ifilename& file)
{
    ref<AbstractMemoryStream> soundfile = FileSystem::instance()->open(file, eReadOnly);
    return ref<Sound>::create(this, soundfile);
}

void World::step()
{
}

ref<Audio::World> WorldManager::createWorld()
{
    return ref<World>::create();
}

}}}

