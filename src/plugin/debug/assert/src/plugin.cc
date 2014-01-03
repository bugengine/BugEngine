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
class AssertSetup : public minitl::pointer
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
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

BE_PLUGIN_REGISTER(minitl::pointer, BugEngine::Debug::AssertSetup);
