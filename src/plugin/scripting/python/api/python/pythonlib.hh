/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PYTHONLIB_H_
#define BE_PYTHON_PYTHONLIB_H_
/**************************************************************************************************/
#include    <python/stdafx.h>
#include    <python/pythontypes.hh>
#include    <plugin/plugin.hh>

namespace BugEngine { namespace Python
{

class be_api(PYTHON) PythonLibrary : public minitl::refcountable
{
public:
    PythonLibrary(const char* pythonLibraryName);
    ~PythonLibrary();

    operator const void* () const   { return (const void*)m_status; }
    bool operator!() const          { return !m_status; }


    void initialize();
    void finalize();

    PyThreadState* createThread();
    void destroyThread(PyThreadState* threadState);

    struct ThreadLock
    {
        ThreadLock(weak<PythonLibrary> library, PyThreadState* thread);
        ~ThreadLock();
    private:
        weak<PythonLibrary> m_library;
        PyThreadState*      m_thread;
    };

private:
    void platformInitialize();
private:
    friend struct ThreadLock;
private:
    const char*                 m_pythonLibraryName;
    void*                       m_handle;
    bool                        m_status;
    PyThreadState*              m_mainThread;
private: // friend ThreadLock
    Py_SetPythonHomeFunc        m_Py_SetPythonHome;
    Py_InitializeExFunc         m_Py_InitializeEx;
    Py_FinalizeFunc             m_Py_Finalize;
    Py_NewInterpreterFunc       m_Py_NewInterpreter;
    Py_EndInterpreterFunc       m_Py_EndInterpreter;
    PyEval_InitThreadsFunc      m_PyEval_InitThreads;
    PyEval_SaveThreadFunc       m_PyEval_SaveThread;
    PyEval_AcquireThreadFunc    m_PyEval_AcquireThread;
    PyEval_ReleaseThreadFunc    m_PyEval_ReleaseThread;
    PyEval_ReleaseLockFunc      m_PyEval_ReleaseLock;
public:
    PyRun_SimpleStringFunc      m_PyRun_SimpleString;
};

}}

/**************************************************************************************************/
#endif
