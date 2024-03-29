/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_PLUGIN_HH_
#define BE_PYTHONLIB_PY_PLUGIN_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythontypes.hh>
#include <bugengine/plugin/plugin.hh>

namespace BugEngine { namespace Python {

struct PyBugPlugin
{
    PyObject               py_object;
    Plugin::Plugin< void > value;

    static void registerType(PyObject* module);

    static PyObject* create(PyTypeObject* type, PyObject* args, PyObject* kwds);
    static int       init(PyObject* self, PyObject* args, PyObject* kwds);
    static PyObject* getattr(PyObject* self, const char* name);
    static int       setattr(PyObject* self, const char* name, PyObject* value);
    static PyObject* repr(PyObject* self);
    static void      dealloc(PyObject* self);
};

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
