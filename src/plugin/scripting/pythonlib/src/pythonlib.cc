/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>

namespace BugEngine { namespace Arena {

minitl::Allocator& python()
{
    return script();
}

}}  // namespace BugEngine::Arena

namespace BugEngine { namespace Python {

PythonLibrary::ThreadLock::ThreadLock(weak< PythonLibrary > library, PyThreadState* thread)
    : m_library(library)
    , m_thread(thread)
{
    (*m_library->m_PyEval_AcquireThread)(m_thread);
    setCurrentContext(m_library);
}

PythonLibrary::ThreadLock::~ThreadLock()
{
    (*m_library->m_PyEval_ReleaseThread)(m_thread);
    clearCurrentContext();
}

void PythonLibrary::initialize()
{
    platformInitialize();
    (*m_Py_InitializeEx)(0);
    (*m_PyEval_InitThreads)();
    m_mainThread = (*m_PyEval_SaveThread)();
}

void PythonLibrary::finalize()
{
    (*m_PyEval_AcquireThread)(m_mainThread);
    (*m_Py_Finalize)();
}

PyThreadState* PythonLibrary::createThread()
{
    (*m_PyEval_AcquireThread)(m_mainThread);
    PyThreadState* result = (*m_Py_NewInterpreter)();
    setupPath();
    (*m_PyEval_ReleaseThread)(result);
    return result;
}

void PythonLibrary::destroyThread(PyThreadState* threadState)
{
    (*m_PyEval_AcquireThread)(threadState);
    (*m_Py_EndInterpreter)(threadState);
    (*m_PyEval_ReleaseLock)();
}

int PythonLibrary::getVersion() const
{
    return m_version;
}

int PythonLibrary::getApi() const
{
    return m_api;
}

}}  // namespace BugEngine::Python
