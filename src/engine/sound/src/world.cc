/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <sound/stdafx.h>
#include    <sound/world.hh>

#include    <system/filesystem.hh>

#include    <AL/al.h>
#include    <AL/alc.h>


namespace BugEngine { namespace Sound
{

METACLASS_IMPL("Sound",World);

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
