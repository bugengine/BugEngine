/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_WORLD_HH_
#define BE_AUDIO_WORLD_HH_
/*****************************************************************************/
#include    <audio/source.hh>

namespace BugEngine { namespace Audio
{

class Sound;
class Source;

class be_api(AUDIO) World : public Object
{
    friend class Sound;
    friend class Source;
public:
    World();
    ~World();

    virtual ref<Sound> createSound(const ifilename& file) = 0;
    virtual ref<Sound> createMusic(const ifilename& file) = 0;

    virtual void step() const = 0;

    be_metaclass(AUDIO,World,Object)
    be_properties
    be_end
};

}}


/*****************************************************************************/
#endif
