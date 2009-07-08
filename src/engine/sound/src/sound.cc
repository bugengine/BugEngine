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
#include    <sound/sound.hh>

#include    <sound/world.hh>

#ifdef BE_COMPILER_MSVC
#pragma warning(push,1)
#endif
#include    <vorbis/vorbisfile.h>
#ifdef BE_COMPILER_MSVC
#pragma warning(pop)
#endif

namespace BugEngine { namespace Sound
{


static size_t vorbis_read(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    AbstractMemoryStream* stream = static_cast<AbstractMemoryStream*>(datasource);
    return checked_numcast<size_t>(stream->read(ptr,size*nmemb));
}

static int vorbis_seek(void *datasource, ogg_int64_t offset, int whence)
{
    AbstractMemoryStream* stream = static_cast<AbstractMemoryStream*>(datasource);
    stream->seek(AbstractMemoryStream::SeekMethod(whence), offset);
    return 0;
}

static long vorbis_tell(void *datasource)
{
    AbstractMemoryStream* stream = static_cast<AbstractMemoryStream*>(datasource);
    return checked_numcast<long>(stream->offset());
}

static ov_callbacks be_callbacks_vorbis = { vorbis_read, vorbis_seek, 0, vorbis_tell };

be_abstractmetaclass_impl("Sound",SoundObject);

SoundObject::SoundObject(World* owner, refptr<AbstractMemoryStream> soundfile) :
    m_owner(owner),
    m_soundFile(soundfile),
    m_data(0),
    m_locked(false)
{
}

SoundObject::~SoundObject()
{
    if(m_data)
    {
        ov_clear(static_cast<OggVorbis_File*>(m_data));
        delete static_cast<OggVorbis_File*>(m_data);
    }
}

World* SoundObject::owner() const
{
    return m_owner;
}

size_t SoundObject::read(void* buffer, size_t size, int& frequency, int& channels) const
{
    int bitstream;
    size_t result = ov_read(static_cast<OggVorbis_File*>(m_data), (char*)buffer, checked_numcast<int>(size), 0, 2, 1, &bitstream);
    vorbis_info* info = ov_info(static_cast<OggVorbis_File*>(m_data), bitstream);
    frequency = info->rate;
    channels = info->channels;
    return result;
}

void SoundObject::reset()
{
    if(m_data)
    {
        ov_clear(static_cast<OggVorbis_File*>(m_data));
        delete static_cast<OggVorbis_File*>(m_data);
    }
    m_data = new OggVorbis_File;
    ov_open_callbacks(m_soundFile.get(), static_cast<OggVorbis_File*>(m_data), 0, 0, be_callbacks_vorbis);
}

bool SoundObject::lock(Source* from)
{
    UNUSED(from);
    if(m_locked)
        return false;
    m_locked = true;
    return true;
}

void SoundObject::unlock()
{
    Assert(m_locked);
    m_locked = false;
}

}}
