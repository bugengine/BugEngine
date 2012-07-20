/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_DEBUG_PROFILER_HH_
#define BE_DEBUG_PROFILER_HH_
/*****************************************************************************/

namespace BugEngine { namespace Profiler
{

class be_api(DEBUG) Profile
{
private:
    const char *m_name;
    Profile*    m_parent;
public:
    Profile(const char* name)
        :   m_name(name)
    {
    }
    ~Profile();

    void start();
    void stop();

    static Profile*  getCurrentProfile();
};

class be_api(DEBUG) ScopedProfile
{
private:
    Profile&    m_profile;
public:
    ScopedProfile(Profile& profile)
        :   m_profile(profile)
    {
        m_profile.start();
    }
    ~ScopedProfile()
    {
        m_profile.stop();
    }
};

#define BE_SCOPE_PROFILE(name)      \
    static Profile profile(name);   \
    ScopedProfile(profile)

}}

/*****************************************************************************/
#endif
