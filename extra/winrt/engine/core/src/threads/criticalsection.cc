/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/threads/criticalsection.hh>
#include    <windows.h>


namespace BugEngine
{

CriticalSection::CriticalSection()
:   m_data(new CRITICAL_SECTION)
{
    CRITICAL_SECTION* s = (CRITICAL_SECTION*)m_data;
    InitializeCriticalSectionEx(s, 3500, 0);
}

CriticalSection::~CriticalSection()
{
    DeleteCriticalSection((CRITICAL_SECTION*)m_data);
    delete (CRITICAL_SECTION*)m_data;
}

void CriticalSection::enter() const
{
    EnterCriticalSection((CRITICAL_SECTION*)m_data);
}

void CriticalSection::leave() const
{
    LeaveCriticalSection((CRITICAL_SECTION*)m_data);
}

}
