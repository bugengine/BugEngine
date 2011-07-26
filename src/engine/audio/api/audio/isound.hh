/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_ISOUND_HH_
#define BE_AUDIO_ISOUND_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>

namespace BugEngine { namespace Audio
{

class ISource;
class IWorld;

class be_api(AUDIO) ISound : public minitl::refcountable
{
    friend class ISource;
    BE_NOCOPY(ISound);
private:
    weak<IWorld>        m_owner;
    ref<IMemoryStream>  m_soundFile;
    void*               m_data;
    bool                m_locked;
public:
    ISound(weak<IWorld> owner, ref<IMemoryStream> soundfile);
    ~ISound();

    weak<IWorld> owner() const;

    void reset();

    bool lock(weak<ISource> from);
    void unlock();

    size_t read(void* buffer, size_t size, int& frequency, int& channels) const;
};

}}


/*****************************************************************************/
#endif
