/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>

namespace BugEngine { namespace Python
{

PythonLibrary::ThreadLock::ThreadLock(weak<PythonLibrary> library, PyThreadState *thread)
    :   m_library(library)
    ,   m_thread(thread)
{
    (*m_library->m_PyEval_AcquireThread)(m_thread);
}

PythonLibrary::ThreadLock::~ThreadLock()
{
    (*m_library->m_PyEval_ReleaseThread)(m_thread);
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
    (*m_PyEval_ReleaseThread)(result);
    return result;
}

void PythonLibrary::destroyThread(PyThreadState* threadState)
{
    (*m_PyEval_AcquireThread)(threadState);
    (*m_Py_EndInterpreter)(threadState);
    (*m_PyEval_ReleaseLock)();
}


}}
