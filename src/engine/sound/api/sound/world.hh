/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SOUND_WORLD_HH_
#define BE_SOUND_WORLD_HH_
/*****************************************************************************/
#include    <sound/source.hh>

namespace BugEngine { namespace Sound
{
    
class SOUNDEXPORT World : public Object
{
    friend class SoundObject;
    friend class Source;
private:
    void*   m_system;
    void*   m_context;
public:
    World();
    ~World();

    refptr<SoundObject> createSound(const ifilename& file);
    refptr<SoundObject> createMusic(const ifilename& file);

    void step() const;

    be_metaclass(SOUNDEXPORT,World,Object)
    be_properties
    be_end
};

}}


/*****************************************************************************/
#endif
