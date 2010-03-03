/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <audio/stdafx.h>
#include    <audio/isource.hh>
#include    <audio/iworld.hh>

namespace BugEngine { namespace Audio
{

be_abstractmetaclass_impl("Audio",ISource);

ISource::ISource(ref<ISound> sound)
:   m_sound(sound)
{
}

ISource::~ISource()
{
}

}}

