/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>


static ref<BugEngine::Python::PythonLibrary> s_library;

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
    PyObject* list = s_library->m_PyList_New(1);
    PyObject* str = s_library->m_PyString_FromString("bugengine");
    s_library->m_PyList_SetItem(list, 0, str);
    Py_DECREF(str);
    module->py_type->tp_setattr(module, "__path__", list);
    Py_DECREF(list);
}

extern "C" BE_EXPORT void initpy_bugengine()
{
    /* python 2.x module initialisation */
    be_info("loading module py_bugengine");
    s_library = ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    PyObject* module;
    if (s_library->m_Py_InitModule4)
    {
        be_assert(sizeof(minitl::size_type) == 4, "Python is 32bits but BugEngine is 64bits");
        module = (*s_library->m_Py_InitModule4)("py_bugengine", s_methods, "", NULL, s_library->getVersion());
    }
    else if (s_library->m_Py_InitModule4_64)
    {
        be_assert(sizeof(minitl::size_type) == 8, "Python is 64bits but BugEngine is 32bits");
        module = (*s_library->m_Py_InitModule4_64)("py_bugengine", s_methods, "", NULL, s_library->getVersion());
    }
    else
    {
        be_unimplemented();
        return;
    }
    setupModule(module);
}

extern "C" BE_EXPORT void PyInit_py_bugengine()
{
    /* python 3.x module initialisation */
    be_info("loading module py_bugengine");
    s_library = ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    PyObject* module = (*s_library->m_PyModule_Create2)(&s_module, s_library->getVersion());
    setupModule(module);
}
