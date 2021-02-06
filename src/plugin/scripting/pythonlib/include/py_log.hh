/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_LOG_HH_
#define BE_PYTHONLIB_PY_LOG_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/core/logger.hh>
#include <bugengine/plugin.scripting.pythonlib/pythontypes.hh>

namespace BugEngine { namespace Python {

struct PyBugLog
{
    enum LogType
    {
        logTypeStdOut,
        logTypeStdErr
    };

    PyObject      py_object;
    ref< Logger > logger;
    LogType       type;

    static void registerType(PyObject* module);

    static int  init(PyObject* self, PyObject* args, PyObject* kwds);
    static void dealloc(PyObject* self);

    static PyObject* write(PyObject* self, PyObject* args);
    static PyObject* flush(PyObject* self, PyObject* args);

    static PyTypeObject s_pyType;
};

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
