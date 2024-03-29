/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_CRITICALSECTION_HH_
#define BE_CORE_THREADS_CRITICALSECTION_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>

namespace BugEngine {

class ScopedCriticalSection;

class be_api(CORE) CriticalSection
{
    friend class ScopedCriticalSection;

private:
    void* m_data;

public:
    CriticalSection();
    ~CriticalSection();

private:
    void enter() const;
    void leave() const;
};

class ScopedCriticalSection
{
    BE_NOCOPY(ScopedCriticalSection);

private:
    const CriticalSection& m_section;

public:
    inline ScopedCriticalSection(const CriticalSection& s) : m_section(s)
    {
        m_section.enter();
    }
    inline ~ScopedCriticalSection()
    {
        m_section.leave();
    }
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
