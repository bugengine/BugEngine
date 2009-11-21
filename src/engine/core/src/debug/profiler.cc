/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/debug/profiler.hh>

namespace BugEngine
{

static BE_THREAD_LOCAL Profile*    g_currentProfile;

Profile::~Profile()
{
}

void Profile::start()
{
    m_parent = g_currentProfile;
}

void Profile::stop()
{
    g_currentProfile = m_parent;
}

}
