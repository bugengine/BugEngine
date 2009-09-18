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
    m_system(alcOpenDevice(0)),
    m_context(alcCreateContext(reinterpret_cast<ALCdevice*>(m_system),0))
{
    alcMakeContextCurrent(reinterpret_cast<ALCcontext*>(m_context));
}

World::~World()
{
    alcDestroyContext(reinterpret_cast<ALCcontext*>(m_context));
    alcCloseDevice(reinterpret_cast<ALCdevice*>(m_system));
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
