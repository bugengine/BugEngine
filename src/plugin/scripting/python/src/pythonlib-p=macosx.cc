/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>

#include    <core/environment.hh>

#include    <dlfcn.h>

namespace BugEngine { namespace Python
{

PythonLibrary::PythonLibrary(const char* pythonLibraryName)
{
#       define be_get_func(f)                                                   \
            if (!m_##f)                                                         \
            {                                                                   \
                be_error("could not locate function %s in module %s"            \
                            | f                                                 \
                            | pythonLibraryName ? pythonLibraryName : "root");  \
                m_status = false;                                               \
            }
        be_get_func(Py_SetPythonHome);
        be_get_func(Py_InitializeEx);
        be_get_func(Py_Finalize);
        be_get_func(Py_NewInterpreter);
        be_get_func(Py_EndInterpreter);
        be_get_func(PyEval_InitThreads);
        be_get_func(PyEval_SaveThread);
        be_get_func(PyEval_AcquireThread);
        be_get_func(PyEval_ReleaseThread);
        be_get_func(PyEval_ReleaseLock);
        be_get_func(PyRun_SimpleString);
#       undef be_get_func
    }
}

PythonLibrary::~PythonLibrary()
{
}

void PythonLibrary::platformInitialize()
{
    const char* programPath = Environment::getEnvironment().getProgramPath();
    size_t programPathLength = strlen(programPath);
    static minitl::Allocator::Block<char> s_pythonHome(Arena::python(),
                                                       programPathLength + 1, 1);
    strcpy(s_pythonHome.begin(), programPath);
    char* path = s_pythonHome.begin();
    while (*path) path++;
    while (*path != '/' && *path != '\\' && path != s_pythonHome.begin()) path--;
    *path = 0;
    Py_SetPythonHome(s_pythonHome.begin());
}

}}
