/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <core/environment.hh>

namespace BugEngine { namespace Python
{

void platformInit()
{
    static char s_homeDirectory[4096];
    strncpy(s_homeDirectory,
            Environment::getEnvironment().getDataDirectory().str().name, 
            sizeof(s_homeDirectory));
    Py_SetPythonHome(s_homeDirectory);
    minitl::format<4096u> path = minitl::format<4096u>("import sys\n"
                                                       "sys.path = [\n"
                                                       "    r'%s',\n"
                                                       "    r'%s/python%s.zip',\n"
                                                       "    r'%s/python%s.zip/DLLs',\n"
                                                       "    r'%s/python%s.zip/Lib'\n"
                                                       "]\n"
                                                       )
                                                     | s_homeDirectory
                                                     | s_homeDirectory
                                                     | PY_VERSION
                                                     | s_homeDirectory
                                                     | PY_VERSION
                                                     | s_homeDirectory
                                                     | PY_VERSION;
    PyRun_SimpleString(path);
}

}}
