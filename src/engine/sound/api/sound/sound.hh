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
    
#ifndef BE_SOUND_SOUNDOBJECT_HH_
#define BE_SOUND_SOUNDOBJECT_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>

namespace BugEngine { namespace Sound
{

class Source;
class World;
    
class SOUNDEXPORT SoundObject : public Object
{
    friend class Source;
private:
    World*                          m_owner;
    refptr<AbstractMemoryStream>    m_soundFile;
    void*                           m_data;
    bool                            m_locked;
public:
    SoundObject(World* owner, refptr<AbstractMemoryStream> soundfile);
    ~SoundObject();

    World* owner() const;

    void reset();

    bool lock(Source* from);
    void unlock();

    size_t read(void* buffer, size_t size, int& frequency, int& channels) const;

    be_metaclass(SOUNDEXPORT,SoundObject,Object)
    be_properties
    be_end
private:
    SoundObject(const SoundObject& other);
    SoundObject& operator=(const SoundObject& other);
};

}}


/*****************************************************************************/
#endif
