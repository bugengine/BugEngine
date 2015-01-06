/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>
#include    <py_object.hh>
#include    <py_plugin.hh>


namespace BugEngine { namespace Python
{

ref<BugEngine::Python::PythonLibrary> s_library;

static PyMethodDef s_methods[] =
{
    {NULL, NULL, 0, NULL}
};

static PyModuleDef s_module =
{
    PyModuleDef_HEAD_INIT,
    "py_bugengine",
    "",
    0,
    s_methods,
    NULL,
    NULL,
    NULL,
    NULL
};

void setupModule(PyObject* module)
{
    PyBugPlugin::registerType(module);
    PyBugObject::registerType(module);
}

}}

extern "C" BE_EXPORT void initpy_bugengine()
{
    using namespace BugEngine::Python;
    /* python 2.x module initialisation */
    be_info("loading module py_bugengine (Python 2)");
    s_library = ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    PyObject* module;
    if (s_library->m_Py_InitModule4)
    {
        be_assert(sizeof(minitl::size_type) == 4, "Python is 32bits but BugEngine is 64bits");
        module = (*s_library->m_Py_InitModule4)("py_bugengine", s_methods, "", NULL, s_library->getApi());
    }
    else if (s_library->m_Py_InitModule4_64)
    {
        be_assert(sizeof(minitl::size_type) == 8, "Python is 64bits but BugEngine is 32bits");
        module = (*s_library->m_Py_InitModule4_64)("py_bugengine", s_methods, "", NULL, s_library->getApi());
    }
    else
    {
        be_unimplemented();
        return;
    }
    setupModule(module);
}

extern "C" BE_EXPORT BugEngine::Python::PyObject* PyInit_py_bugengine()
{
    using namespace BugEngine::Python;
    /* python 3.x module initialisation */
    be_info("loading module py_bugengine (Python 3)");
    s_library = ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    PyObject* module = (*s_library->m_PyModule_Create2)(&s_module, s_library->getApi());
    setupModule(module);
    return module;
}
