/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PROFILER_HH_
#define BE_CORE_PROFILER_HH_
/*****************************************************************************/

namespace BugEngine
{

class Profile
{
private:
    const char *m_name;
    Profile*    m_parent;
public:
    template< size_t size >
    Profile(const char name[size])
        :   m_name(name)
    {
    }
    ~Profile();

    void start();
    void stop();

    static Profile*  getCurrentProfile();
};

class ScopedProfiling
{
private:
    Profile&    m_profile;
public:
    ScopedProfiling(Profile& profile)
        :   m_profile(profile)
    {
        m_profile.start();
    }
    ~ScopedProfiling()
    {
        m_profile.stop();
    }
private:
    ScopedProfiling& operator=(const ScopedProfiling& other);
    ScopedProfiling(const ScopedProfiling& other);
};

#define BE_SCOPE_PROFILE(name)      \
    static Profile profile(name);   \
    ScopedProfiling(profile)

}

/*****************************************************************************/
#endif
