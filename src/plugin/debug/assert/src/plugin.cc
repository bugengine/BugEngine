/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <plugin/plugin.hh>

namespace BugEngine { namespace Debug
{

minitl::AssertionResult AssertionCallback(const char* file,
                                          int         line,
                                          const char* expr,
                                          const char* message);
class AssertSetup : public minitl::refcountable
{
private:
    minitl::AssertionCallback_t   m_previousAssertionCallback;
public:
    AssertSetup(const BugEngine::Plugin::Context& /*context*/)
        :   m_previousAssertionCallback(minitl::setAssertionCallback(&AssertionCallback))
    {
        be_debug("installed assert callback");
    }
    ~AssertSetup()
    {
        minitl::setAssertionCallback(m_previousAssertionCallback);
    }
private:
    AssertSetup(const AssertSetup& other);
    AssertSetup& operator=(const AssertSetup& other);
};

}}

BE_PLUGIN_REGISTER(BugEngine::Debug::AssertSetup);
