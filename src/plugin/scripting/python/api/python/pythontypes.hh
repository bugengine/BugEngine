/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PYTHONTYPES_HH_
#define BE_PYTHON_PYTHONTYPES_HH_
/**************************************************************************************************/

extern "C"
{

struct PyThreadState;

}

namespace BugEngine { namespace Python
{

typedef void (*Py_SetPythonHomeFunc)(const char* home);
typedef void (*Py_InitializeExFunc)(int initsigs);
typedef void (*Py_FinalizeFunc)();
typedef PyThreadState* (*Py_NewInterpreterFunc)();
typedef void (*Py_EndInterpreterFunc)(PyThreadState* tstate);
typedef const char* (*Py_GetPathFunc)();
typedef const char* (*Py_GetVersionFunc)();

typedef void (*PyEval_InitThreadsFunc)();
typedef PyThreadState* (*PyEval_SaveThreadFunc)();
typedef void (*PyEval_AcquireThreadFunc)(PyThreadState* tstate);
typedef void (*PyEval_ReleaseThreadFunc)(PyThreadState* tstate);
typedef void (*PyEval_ReleaseLockFunc)();

typedef int (*PyRun_SimpleStringFunc)(const char* command);

}}

/**************************************************************************************************/
#endif
