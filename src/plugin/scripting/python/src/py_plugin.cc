/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_plugin.hh>

namespace BugEngine { namespace Python
{

static PyTypeObject s_bugenginePluginType =
{
    { { 0, 0 }, 0 },
    "py_bugengine.Plugin",
    sizeof(PyBugPlugin),
    0,
    &PyBugPlugin::dealloc,
    0,
    &PyBugPlugin::getattr,
    &PyBugPlugin::setattr,
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
    Py_TPFLAGS_DEFAULT,
    "Wrapper class for the C++ class BugEngine::Plugin::Plugin",
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
    &PyBugPlugin::init,
    0,
    &PyBugPlugin::create,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

PyObject* PyBugPlugin::create(PyTypeObject* type, PyObject* /*args*/, PyObject* /*kwds*/)
{
    PyBugPlugin* self = reinterpret_cast<PyBugPlugin*>(type->tp_alloc(type, 0));
    new (&self->value) Plugin::Plugin<void>("", Plugin::Plugin<void>::Preload);
    return reinterpret_cast<PyObject*>(self);
}

int PyBugPlugin::init(PyObject* self, PyObject* args, PyObject* /*kwds*/)
{
    PyBugPlugin* self_ = reinterpret_cast<PyBugPlugin*>(self);
    const char* name = 0;
    if (s_library->m__PyArg_ParseTuple_SizeT(args, "s", &name))
    {
        self_->value = Plugin::Plugin<void>(name, Plugin::Plugin<void>::Preload);
        return 0;
    }
    else
    {
        return -1;
    }
}

PyObject* PyBugPlugin::getattr(PyObject* self, const char* name)
{
    be_forceuse(self);
    be_forceuse(name);
    return 0;
}

int PyBugPlugin::setattr(PyObject* self, const char* name, PyObject* value)
{
    be_forceuse(self);
    be_forceuse(name);
    be_forceuse(value);
    return 0;
}

void PyBugPlugin::dealloc(PyObject* self)
{
    be_forceuse(self);
    PyBugPlugin* self_ = reinterpret_cast<PyBugPlugin*>(self);
    self_->value.~Plugin();
    self->py_type->tp_free(self);
}

void PyBugPlugin::registerType(PyObject* module)
{
    int result = s_library->m_PyType_Ready(&s_bugenginePluginType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
    Py_INCREF(&s_bugenginePluginType);
    result = (*s_library->m_PyModule_AddObject)(module, "Plugin", (PyObject*)&s_bugenginePluginType);
    be_assert(result >= 0, "unable to register type");
    be_forceuse(result);
}

}}
