/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <sound/stdafx.h>
#include    <sound/world.hh>

#include    <system/filesystem.hh>

#include    <AL/al.h>
#include    <AL/alc.h>


namespace BugEngine { namespace Sound
{

be_metaclass_impl("Sound",World);

World::World() :
    m_system(0),
    m_context(0)
{
}

World::~World()
{
}

refptr<SoundObject> World::createSound(const ifilename& file)
{
    refptr<AbstractMemoryStream> soundfile = FileSystem::instance()->open(file, eReadOnly);
    return new SoundObject(this, soundfile);
}

refptr<SoundObject> World::createMusic(const ifilename& file)
{
    refptr<AbstractMemoryStream> soundfile = FileSystem::instance()->open(file, eReadOnly);
    return new SoundObject(this, soundfile);
}

void World::step() const
{
}


}}
