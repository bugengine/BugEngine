/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>

namespace BugEngine { namespace Python
{

PythonLibrary::PythonLibrary(const char* pythonLibraryName)
    : m_pythonLibraryName(pythonLibraryName)
    , m_handle(m_pythonLibraryName
                    ? LoadLibraryA(minitl::format<1024u>("%s.dll") | m_pythonLibraryName)
                    : GetModuleHandle(NULL))
    , m_status(m_handle != 0)
{
    if (!m_status)
    {
        be_error("unable to load library %s" | pythonLibraryName);
    }
    else
    {
#   define be_get_func(f)                                                   \
        void* tmp##f = GetProcAddress((HMODULE)m_handle, #f);               \
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
        be_get_func(PyEval_InitThreads);
        be_get_func(PyEval_SaveThread);
        be_get_func(PyEval_AcquireThread);
        be_get_func(PyEval_ReleaseThread);
        be_get_func(PyEval_ReleaseLock);
        be_get_func(PyRun_SimpleString);
#   undef be_get_fun
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
    if (m_handle && m_pythonLibraryName)
    {
        FreeLibrary((HMODULE)m_handle);
    }
}

void PythonLibrary::platformInitialize()
{
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
