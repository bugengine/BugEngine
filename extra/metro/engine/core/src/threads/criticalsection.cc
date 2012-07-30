/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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

void CriticalSection::enter() const
{
    EnterCriticalSection((CRITICAL_SECTION*)m_data);
}

void CriticalSection::leave() const
{
    LeaveCriticalSection((CRITICAL_SECTION*)m_data);
}

}
