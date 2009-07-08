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
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/
    
#ifndef BE_SOUND_SOURCE_HH_
#define BE_SOUND_SOURCE_HH_
/*****************************************************************************/
#include    <sound/sound.hh>
#include    <system/action.hh>

namespace BugEngine { namespace Sound
{
    
class SOUNDEXPORT Source : public Object
{
private:
    refptr<SoundObject> m_sound;
    void*               m_data;
    size_t              m_pauseCount;
public:
    Source();
    Source(SoundObject* sound);
    ~Source();

    void play();
    void pause();

    void setSound(SoundObject* sound);

    float volume() const;
    void  setVolume(float volume);

    virtual void step() const;

    be_metaclass(SOUNDEXPORT,Source,Action)
        refptr<Source> createFromSound(SoundObject* sound);
    be_properties
        be_classmethod(createFromSound);

        be_method(setSound);

        be_method(volume);
        be_method(setVolume);
        be_method(play);
        be_method(pause);

        be_property(Volume)
            [be_get(volume)]
            [be_set(setVolume)];
    be_end
};

}}


/*****************************************************************************/
#endif
