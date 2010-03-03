/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <source.hh>
#include    <world.hh>

#include    <system/sequencer.hh>

namespace BugEngine { namespace Audio { namespace OpenAL
{


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
                be_notreached();
                format = AL_FORMAT_STEREO16;
            }
            alBufferData(m_ALBuffers[i], format, buffer, be_checked_numcast<ALsizei>(size), freq);
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
                        be_notreached();
                        format = AL_FORMAT_STEREO16;
                    }
                    alBufferData(bufferid, format, buffer, be_checked_numcast<ALsizei>(size), freq);
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

Source::Source(ref<Sound> sound)
:   Audio::ISource(sound)
,   m_pauseCount(0)
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
    be_assert(m_pauseCount > 0, "sound was not paused, cannot resume");
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
    UNUSED(volume);
//  static_cast<_ALData*>(m_data)->setVolume(volume);
}

}}}

