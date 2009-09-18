/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/criticalsection.hh>


namespace BugEngine
{

CriticalSection::CriticalSection()
:   m_data(new CRITICAL_SECTION)
{
    CRITICAL_SECTION* s = (CRITICAL_SECTION*)m_data;
    InitializeCriticalSection(s);
}

CriticalSection::~CriticalSection()
{
    delete (CRITICAL_SECTION*)m_data;
}

void CriticalSection::enter()
{
    EnterCriticalSection((CRITICAL_SECTION*)m_data);
}

void CriticalSection::leave()
{
    LeaveCriticalSection((CRITICAL_SECTION*)m_data);
}

}
