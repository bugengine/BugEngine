/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_CRITICALSECTION_HH_
#define BE_CORE_THREADS_CRITICALSECTION_HH_
/**************************************************************************************************/

namespace BugEngine
{

class ScopedCriticalSection;

class be_api(CORE) CriticalSection
{
    friend class ScopedCriticalSection;
private:
    void*           m_data;
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
    const CriticalSection&    m_section;
public:
    inline ScopedCriticalSection(const CriticalSection& s) : m_section(s)   { m_section.enter(); }
    inline ~ScopedCriticalSection()                                         { m_section.leave(); }
};

}

/**************************************************************************************************/
#endif
