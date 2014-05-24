/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <context.hh>

namespace BugEngine { namespace Python
{

void Context::pythonInitialise()
{
    const char* programPath = Environment::getEnvironment().getProgramPath();
    size_t programPathLength = strlen(programPath);
    static minitl::Allocator::Block<char> s_pythonHome(Arena::game(),
                                                       programPathLength + 1, 1);
    strcpy(s_pythonHome.begin(), programPath);
    char* path = s_pythonHome.begin();
    while (*path) path++;
    while (*path != '/' && *path != '\\' && path != s_pythonHome.begin()) path--;
    *path = 0;
    Py_SetPythonHome(s_pythonHome.begin());
}

}}

