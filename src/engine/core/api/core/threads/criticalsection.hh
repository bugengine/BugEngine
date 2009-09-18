/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_CRITICALSECTION_HH_
#define BE_CORE_THREADS_CRITICALSECTION_HH_
/*****************************************************************************/

namespace BugEngine
{

class COREEXPORT CriticalSection
{
private:
    void*           m_data;
public:
    CriticalSection();
    ~CriticalSection();

    void enter();
    void leave();
};

class ScopedCriticalSection
{
private:
    CriticalSection&    m_section;
public:
    inline ScopedCriticalSection(CriticalSection& s) : m_section(s) { m_section.enter(); }
    inline ~ScopedCriticalSection()                                 { m_section.leave(); }
private:
    ScopedCriticalSection& operator=(const ScopedCriticalSection& other);
    ScopedCriticalSection(const ScopedCriticalSection& other);
};

}

/*****************************************************************************/
#endif
