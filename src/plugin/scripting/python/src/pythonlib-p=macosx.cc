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
                    :   dlopen(NULL, RTLD_LAZY | RTLD_GLOBAL))
    ,   m_status(m_handle != 0)
{
    if (!m_status)
    {
        be_error("unable to load library %s: %s" | pythonLibraryName | dlerror());
    }
    else
    {
#       define be_get_func(f)                                                   \
            void* tmp##f = dlsym(m_handle, #f);                                 \
            memcpy(&m_##f, &tmp##f, sizeof(f##Func));                           \
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
        be_get_func(PyEval_InitThreads);
        be_get_func(PyEval_SaveThread);
        be_get_func(PyEval_AcquireThread);
        be_get_func(PyEval_ReleaseThread);
        be_get_func(PyEval_ReleaseLock);
        be_get_func(PyRun_SimpleString);
#       undef be_get_func
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

}}
