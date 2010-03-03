/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_IWORLD_HH_
#define BE_AUDIO_IWORLD_HH_
/*****************************************************************************/
#include    <audio/isource.hh>
#include    <system/scheduler/task.hh>

namespace BugEngine { namespace Audio
{

class be_api(AUDIO) IWorld : public Object
{
public:
    IWorld();
    ~IWorld();

    virtual ref<ISound> createSound(const ifilename& file) = 0;
    virtual ref<ISound> createMusic(const ifilename& file) = 0;

    virtual void step() = 0;

    be_metaclass(AUDIO,IWorld,Object)
    be_properties
    be_end
};

class be_api(AUDIO) IWorldManager
{
public:
    virtual ~IWorldManager() { }
    virtual ref<IWorld> createWorld() = 0;
};


}}


/*****************************************************************************/
#endif
