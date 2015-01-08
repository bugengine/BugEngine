/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PY_PLUGIN_HH_
#define BE_PYTHON_PY_PLUGIN_HH_
/**************************************************************************************************/
#include    <python/stdafx.h>
#include    <python/pythontypes.hh>
#include    <plugin/plugin.hh>

namespace BugEngine { namespace Python
{

struct PyBugPlugin
{
    PyObject                py_object;
    Plugin::Plugin<void>    value;

    static void registerType(PyObject* module);
    
    static PyObject* create(PyTypeObject* type, PyObject* args, PyObject* kwds);
    static int init(PyObject* self, PyObject* args, PyObject* kwds);
    static PyObject* getattr(PyObject* self, const char* name);
    static int setattr(PyObject* self, const char* name, PyObject* value);
    static PyObject* repr(PyObject *self);
    static void dealloc(PyObject* self);
};

}}

/**************************************************************************************************/
#endif
