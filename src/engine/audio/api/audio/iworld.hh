/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_IWORLD_HH_
#define BE_AUDIO_IWORLD_HH_
/*****************************************************************************/
#include    <audio/isource.hh>
#include    <system/scheduler/task/task.hh>
#include    <core/memory/streams.hh>

namespace BugEngine { namespace Audio
{

class be_api(AUDIO) IWorld : public minitl::refcountable
{
public:
    IWorld();
    ~IWorld();

    virtual ref<ISound> createSound(ref<IMemoryStream> soundFile) = 0;

    virtual void step() = 0;
public:
    enum { Arena = BugEngine::Arena::General };
};

}}


/*****************************************************************************/
#endif
