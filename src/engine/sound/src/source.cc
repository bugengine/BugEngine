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
#include    <sound/source.hh>
#include    <sound/world.hh>

#include    <system/sequencer.hh>

#include    <AL/al.h>

namespace BugEngine { namespace Sound
{

METACLASS_IMPL("Sound",Source);

#if 0

namespace
{

const int gAlBufferSize = 8192;
const int gAlBufferNum  = 16;

class _ALData
{
private:
    ALuint          m_audioSource;
    ALuint          m_ALBuffers[gAlBufferNum];
    bool            m_initialized;
public:
    _ALData() :
        m_initialized(false)
    {
        alGenSources(1, &m_audioSource);
        alGenBuffers(gAlBufferNum, m_ALBuffers);
    }
    ~_ALData()
    {
        alDeleteBuffers(4,m_ALBuffers);
        alDeleteSources(1,&m_audioSource);
    }
    void init(const SoundObject* sound)
    {
        void* buffer = malloca(gAlBufferSize);
        for(size_t i = 0; i < gAlBufferNum; ++i)
        {
            int freq;
            int channels;
            size_t size = sound->read(buffer, gAlBufferSize, freq, channels);
            ALenum format;
            switch(channels)
            {
            case 1:
                format = AL_FORMAT_MONO16;
                break;
            case 2:
                format = AL_FORMAT_STEREO16;
                break;
            case 4:
                format = alGetEnumValue("AL_FORMAT_QUAD16");
                break;
            case 6:
                format = alGetEnumValue("AL_FORMAT_51CHN16");
                break;
            default:
                AssertNotReached();
                format = AL_FORMAT_STEREO16;
            }
            alBufferData(m_ALBuffers[i], format, buffer, checked_numcast<ALsizei>(size), freq);
            alSourceQueueBuffers(m_audioSource, 1, &m_ALBuffers[i]);
        }
        alSourcePlay(m_audioSource);
        freea(buffer);
        m_initialized = true;
    }
    void play()
    {
        alSourcePlay(m_audioSource);
    }
    void pause()
    {
        alSourcePause(m_audioSource);
    }
    float volume()
    {
        float result;
        alGetSourcef(m_audioSource, AL_GAIN, &result);
        return result;
    }
    void setVolume(float volume)
    {
        alSourcef(m_audioSource, AL_GAIN, volume);
    }
    bool update(const SoundObject* sound)
    {
        if(!m_initialized)
        {
            init(sound);
            Assert(m_initialized);
            return false;
        }
        else
        {
            int processed_buffers;
            alGetSourcei(m_audioSource, AL_BUFFERS_PROCESSED, &processed_buffers);
            while (processed_buffers)
            {
                unsigned int bufferid = 0;

                alSourceUnqueueBuffers(m_audioSource, 1, &bufferid);

                int freq;
                int channels;
                void* buffer = malloca(gAlBufferSize);
                size_t size = sound->read(buffer, gAlBufferSize, freq, channels);
                if(size)
                {
                    ALenum format;
                    switch(channels)
                    {
                    case 1:
                        format = AL_FORMAT_MONO16;
                        break;
                    case 2:
                        format = AL_FORMAT_STEREO16;
                        break;
                    case 4:
                        format = alGetEnumValue("AL_FORMAT_QUAD16");
                        break;
                    case 6:
                        format = alGetEnumValue("AL_FORMAT_51CHN16");
                        break;
                    default:
                        AssertNotReached();
                        format = AL_FORMAT_STEREO16;
                    }
                    alBufferData(bufferid, format, buffer, checked_numcast<ALsizei>(size), freq);
                    alSourceQueueBuffers(m_audioSource, 1, &bufferid);
                    processed_buffers--;
                }
                freea(buffer);
            }

            int state;
            alGetSourcei(m_audioSource, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING)
            {
                int buffers_queued;
                alGetSourcei(m_audioSource, AL_BUFFERS_QUEUED, &buffers_queued);
                if(buffers_queued == 0)
                    return true;
                else
                {
                    alSourcePlay(m_audioSource);
                    return false;
                }
            }
            else
                return false;
        }
    }
};

}

#endif

Source::Source()
:   m_sound(0)
//, m_data(new _ALData())
,    m_pauseCount(0)
{
}

Source::~Source()
{
}

void Source::step() const
{
}

void Source::play()
{
    Assert(m_pauseCount > 0);
//    if(!--m_pauseCount)
//      static_cast<_ALData*>(m_data)->play();
}

void Source::pause()
{
//    if(!m_pauseCount++)
//      static_cast<_ALData*>(m_data)->pause();
}

float Source::volume() const
{
//    return static_cast<_ALData*>(m_data)->volume();
    return 0.0f;
}

void Source::setVolume(float volume)
{
//  static_cast<_ALData*>(m_data)->setVolume(volume);
}

void Source::setSound(SoundObject* sound)
{
/*    Sequencer* current = 0;
    if(m_sound)
    {
        current = m_sound->owner();
        m_sound->unlock();
    }
    bool lock = sound->lock(this);
    Assert(lock);
    m_sound = sound;
    m_sound->reset();
    if(current && m_sound->owner() != current)
    {
        current->stop(this);
    }
    m_sound->owner()->run(this);*/
}

refptr<Source> Source::MetaClass::createFromSound(SoundObject* sound)
{
    Source* src = new Source;
    src->setSound(sound);
    return src;
}

}}
