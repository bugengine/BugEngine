/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <audio/stdafx.h>
#include    <audio/source.hh>
#include    <audio/world.hh>

namespace BugEngine { namespace Audio
{

be_abstractmetaclass_impl("Audio",Source);

Source::Source(ref<Sound> sound)
:   m_sound(sound)
{
}

Source::~Source()
{
}

}}

