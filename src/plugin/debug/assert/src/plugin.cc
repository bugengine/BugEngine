/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti/classinfo.script.hh>

namespace BugEngine { namespace Debug {

minitl::AssertionResult AssertionCallback(const char* file, int line, const char* expr,
                                          const char* message);
class AssertSetup : public minitl::refcountable
{
private:
    minitl::AssertionCallback_t m_previousAssertionCallback;

public:
    AssertSetup(const BugEngine::Plugin::Context& /*context*/)
        : m_previousAssertionCallback(minitl::setAssertionCallback(&AssertionCallback))
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

}}  // namespace BugEngine::Debug

BE_PLUGIN_REGISTER(BugEngine::Debug::AssertSetup);
