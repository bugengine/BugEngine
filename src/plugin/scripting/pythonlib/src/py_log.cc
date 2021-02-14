/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <py_log.hh>

namespace BugEngine { namespace Python {

static PyMethodDef s_logMethods[] = {{"write", &PyBugLog::write, METH_VARARGS, "sys.std*.write"},
                                     {"flush", &PyBugLog::flush, METH_VARARGS, "sys.std*.flush"},
                                     {0, 0, 0, 0}};

PyTypeObject PyBugLog::s_pyType = {{{0, 0}, 0},
                                   "py_bugengine.Log",
                                   sizeof(PyBugLog),
                                   0,
                                   &PyBugLog::dealloc,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
                                   "BugEngine logging",
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   s_logMethods,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   &PyBugLog::init,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0,
                                   0};

int PyBugLog::init(PyObject* self, PyObject* args, PyObject* kwds)
{
    be_forceuse(self);
    be_forceuse(args);
    be_forceuse(kwds);
    return 0;
}

void PyBugLog::dealloc(PyObject* self)
{
    PyBugLog* self_ = reinterpret_cast< PyBugLog* >(self);
    self_->logger.~ref();
    self->py_type->tp_free(self);
}

static char  logBuffer[65535];
static char* logCurrent = logBuffer;
PyObject*    PyBugLog::write(PyObject* self, PyObject* args)
{
    PyBugLog* log = reinterpret_cast< PyBugLog* >(self);
    char*     str;
    LogLevel  level = log->type == logTypeStdOut ? logInfo : logError;
    if(s_library->m__PyArg_ParseTuple_SizeT(args, "s", &str))
    {
        u32 written = 0;
        for(; *str; ++str, ++written)
        {
            if(*str == '\n')
            {
                *logCurrent = 0;
                log->logger->log(level, "TODO", 0, logBuffer);
                logCurrent = logBuffer;
            }
            else
            {
                *logCurrent = *str;
                logCurrent++;
                if(logCurrent >= logBuffer + sizeof(logBuffer) - 1)
                {
                    *logCurrent = 0;
                    log->logger->log(level, "TODO", 0, logBuffer);
                    logCurrent = logBuffer;
                }
            }
        }
        if(s_library->getVersion() >= 30)
            return s_library->m_PyLong_FromUnsignedLongLong(written);
        else
            return s_library->m_PyInt_FromLong((long)written);
    }
    else
    {
        return 0;
    }
}

PyObject* PyBugLog::flush(PyObject* self, PyObject* args)
{
    be_forceuse(self);
    be_forceuse(args);
    Py_INCREF(s_library->m__Py_NoneStruct);
    return s_library->m__Py_NoneStruct;
}

void PyBugLog::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_pyType);
    be_assert(result >= 0, "unable to register type");
    Py_INCREF(&s_pyType);

    PyBugLog* ob = reinterpret_cast< PyBugLog* >(s_pyType.tp_alloc(&s_pyType, 0));
    new(&ob->logger) ref< Logger >(Logger::instance("scripting.python"));
    ob->type = logTypeStdOut;
    result   = s_library->m_PySys_SetObject("stdout", (PyObject*)ob);
    be_assert(result >= 0, "unable to register stdout");
    Py_DECREF(ob);

    ob = reinterpret_cast< PyBugLog* >(s_pyType.tp_alloc(&s_pyType, 0));
    new(&ob->logger) ref< Logger >(Logger::instance("scripting.python"));
    ob->type = logTypeStdErr;
    result   = s_library->m_PySys_SetObject("stderr", (PyObject*)ob);
    be_assert(result >= 0, "unable to register stderr");
    Py_DECREF(ob);
    be_forceuse(result);
    be_forceuse(module);
}

}}  // namespace BugEngine::Python
