/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/threads/criticalsection.hh>

namespace BugEngine {

CriticalSection::CriticalSection() : m_data(new CRITICAL_SECTION)
{
    CRITICAL_SECTION* s = (CRITICAL_SECTION*)m_data;
    InitializeCriticalSection(s);
}

CriticalSection::~CriticalSection()
{
    DeleteCriticalSection((CRITICAL_SECTION*)m_data);
    delete(CRITICAL_SECTION*)m_data;
}

void CriticalSection::enter() const
{
    EnterCriticalSection((CRITICAL_SECTION*)m_data);
}

void CriticalSection::leave() const
{
    LeaveCriticalSection((CRITICAL_SECTION*)m_data);
}

}  // namespace BugEngine
