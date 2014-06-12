/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>

#include    <core/environment.hh>

#include    <dlfcn.h>

namespace BugEngine { namespace Python
{

PythonLibrary::PythonLibrary(const char* pythonLibraryName)
    :   m_pythonLibraryName(pythonLibraryName)
    ,   m_handle(m_pythonLibraryName
                    ?   dlopen(minitl::format<1024u>("lib%s.dylib") | m_pythonLibraryName,
                        RTLD_LAZY | RTLD_GLOBAL)
                    :   RTLD_DEFAULT)
    ,   m_status(dlerror() == 0)
    ,   m_version(1013)
{
    if (!m_status)
    {
        be_error("unable to load library %s: %s" | pythonLibraryName | dlerror());
    }
    else
    {
#       define be_get_func_opt(f)                                               \
            void* tmp##f = dlsym(m_handle, #f);                                 \
            memcpy(&m_##f, &tmp##f, sizeof(f##Type));
#       define be_get_func(f)                                                   \
            be_get_func_opt(f)                                                  \
            if (!m_##f)                                                         \
            {                                                                   \
                be_error("could not locate function %s in module %s"            \
                            | #f                                                \
                            | (pythonLibraryName ? pythonLibraryName : "root"));\
                m_status = false;                                               \
            }
        be_get_func(Py_SetPythonHome);
        be_get_func(Py_InitializeEx);
        be_get_func(Py_Finalize);
        be_get_func(Py_NewInterpreter);
        be_get_func(Py_EndInterpreter);
        be_get_func(Py_GetPath);
        be_get_func(Py_GetVersion);
        be_get_func_opt(Py_InitModule4);
        be_get_func_opt(Py_InitModule4_64);
        be_get_func_opt(PyModule_Create2);
        be_get_func(PyEval_InitThreads);
        be_get_func(PyEval_SaveThread);
        be_get_func(PyEval_AcquireThread);
        be_get_func(PyEval_ReleaseThread);
        be_get_func(PyEval_ReleaseLock);
        be_get_func(PyRun_SimpleString);
        be_get_func(_Py_NoneStruct);
#       undef be_get_fun
#       undef be_get_fun_opt
        if (m_status && pythonLibraryName)
        {
            initialize();
        }
    }
}

PythonLibrary::~PythonLibrary()
{
    if (m_status && m_pythonLibraryName)
    {
        finalize();
    }
    if (m_handle)
    {
        dlclose(m_handle);
    }
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
    (*m_Py_SetPythonHome)(s_pythonHome.begin());
}

void PythonLibrary::setupPath()
{
    const char* programPath = Environment::getEnvironment().getProgramPath();
    size_t programPathLength = strlen(programPath);
    minitl::Allocator::Block<char> pythonHome(Arena::python(),
                                              programPathLength + 1, 1);
    strcpy(pythonHome.begin(), programPath);
    char* path = pythonHome.begin();
    while (*path) path++;
    while (*path != '/' && *path != '\\' && path != pythonHome.begin()) path--;
    *path = 0;
    (*m_PyRun_SimpleString)(minitl::format<4096>("import sys; sys.path.append(\"%s\")") | pythonHome.data());
}

}}
